#include "AssetDatabase/AssetDatabase.h"

#include <limits>
#include <cassert>
#include <fstream>

#include <Logger/Logger.h>
#include <Loaders/LoaderCommon.h>
#include <Loaders/MeshLoader.h>

using namespace std;

// private namespace, cannot be used outside this source file
namespace
{
	/**
	 * FNV-1a
	 * Fowler–Noll–Vo hash function
	 * @param text to hash
	 * @return 64 bit hash
	 */
	inline uint64_t Hash(const std::string& text)
	{
		// 64 bit params
		uint64_t constexpr fnv_prime = 1099511628211ULL;
		uint64_t constexpr fnv_offset_basis = 14695981039346656037ULL;

		uint64_t hash = fnv_offset_basis;

		for(auto c: text)
		{
			hash ^= c;
			hash *= fnv_prime;
		}

		return hash;
	}
}

namespace PlatinumEngine
{
	//--------------------------------------------------------------------------------------------------------------
	// Static
	//--------------------------------------------------------------------------------------------------------------

	std::ostream& operator<<(std::ostream& output, AssetDatabase& assetDatabase)
	{
		output << AssetDatabase::FIRST_LINE << std::endl;
		for (const auto& asset: assetDatabase._database)
		{
			output
					<< asset.id << ','
					<< asset.hash << ','
					<< asset.doesExist << ','
					<< AssetDatabase::StandardisePath(asset.path) << std::endl;
		}
		return output;
	}

	std::istream& operator>>(std::istream& input, AssetDatabase& assetDatabase)
	{
		// enforce a empty object
		assetDatabase._database.clear();
		assetDatabase._assetIDsMap.clear();

		{
			std::string firstLine;
			std::getline(input, firstLine);
			if (firstLine != AssetDatabase::FIRST_LINE)
			{
				PLATINUM_WARNING("AssetDatabase stream input: unexpected first line");
			}
		}

		Asset asset;
		std::string filePath;
		size_t line = 1;
		while (input >> asset.id && input.get() &&
			   input >> asset.hash && input.get() &&
			   input >> asset.doesExist && input.get() &&
			   std::getline(input, filePath))
		{
			// construct path object, could exist or not exist
			asset.path = filesystem::path(filePath);

			// find id
			auto i = assetDatabase._assetIDsMap.find(asset.id);
			// if id is unique
			if (i == assetDatabase._assetIDsMap.end())
			{
				assetDatabase._database.push_back(asset);
				assert(assetDatabase._assetIDsMap.insert({ asset.id, assetDatabase._database.size() - 1 }).second);
			}
			else
			{
				// index of existing AssetID in database
				size_t databaseIndex = i->second;
				PLATINUM_WARNING_STREAM
						<< "AssetDatabase input data is bad. Same AssetID was declared for multiple files: "
						<< assetDatabase._database.at(databaseIndex).path
						<< " and "
						<< asset.path
						<< " at line " << line;
			}

			++line;
		}

		if (!input.eof())
			PLATINUM_WARNING_STREAM << "AssetDatabase input stream has format error at line " << line;

		return input;
	}

	HashType AssetDatabase::HashFile(const filesystem::path& path)
	{
		if (filesystem::file_size(path) < BIG_FILE_THRESHOLD_BYTES)
		{
			// small file
			// hash file's content
			std::ifstream fileStream(path);
			if (fileStream.is_open())
			{
				std::stringstream buffer;
				buffer << fileStream.rdbuf();
				return Hash(buffer.str());
			}
			else
			{
				// if file cannot open, just hash the same way as BIG FILE
				PLATINUM_WARNING_STREAM << "Cannot hash file content because it can't open: " << path;
			}
		}

		// BIG FILE or cannot open file
		// hash file name
		return Hash(path.filename().string());
	}

	std::string AssetDatabase::StandardisePath(const filesystem::path& path)
	{
		std::stringstream buffer;
		char lastC = 0;
		for (char c: path.string())
		{
			// skip consecutive slashes
			if (lastC == '/' && (c == '/' || c == '\\'))
				continue;

			// convert \ into /
			if (c == '\\')
			{
				buffer << '/';
				lastC = '/';
			}
			else
			{
				buffer << c;
				lastC = c;
			}
		}
		return buffer.str();
	}

	//--------------------------------------------------------------------------------------------------------------
	// Constructors/destructors
	//--------------------------------------------------------------------------------------------------------------

	AssetDatabase::AssetDatabase() :
			_generator(std::random_device()()),
			_anyNumber(std::numeric_limits<HashType>::min(), std::numeric_limits<HashType>::max()),
			_assetsFolderPath("./Assets"),
			_assetDatabasePath("./Assets/AssetDatabase.csv")
	{
	}

	AssetDatabase::AssetDatabase(std::string assetsFolderPath, std::string assetDatabasePath) :
			AssetDatabase()
	{
		_assetsFolderPath = std::move(assetsFolderPath);
		_assetDatabasePath = std::move(assetDatabasePath);
	}

	AssetDatabase::~AssetDatabase()
	{
		DeleteLoadedAssets();
	}

	//--------------------------------------------------------------------------------------------------------------
	// Getters
	//--------------------------------------------------------------------------------------------------------------

	bool AssetDatabase::TryGetAsset(AssetID id, Asset& outAsset)
	{
		auto i = _assetIDsMap.find(id);
		if (i == _assetIDsMap.end())
			return false;
		else
		{
			// range-checked indexing
			outAsset = _database.at(i->second);
			return true;
		}
	}

	bool AssetDatabase::TryGetAsset(const std::string& filePath, Asset& outAsset)
	{
		for (auto& i: _database)
		{
			if (i.path == filePath)
			{
				outAsset = i;
				return true;
			}
		}
		return false;
	}

	const std::vector<Asset>& AssetDatabase::GetDatabase() const
	{
		return _database;
	}

	//--------------------------------------------------------------------------------------------------------------
	// Database controls
	//--------------------------------------------------------------------------------------------------------------

	void AssetDatabase::CountHashesInMap(
			std::multimap<HashType, size_t>& hashesMap,
			HashType hash,
			size_t& outExistCount,
			size_t& outNonExistCount,
			size_t& outLastDatabaseIndex)
	{
		outExistCount = 0;
		outNonExistCount = 0;
		outLastDatabaseIndex = -1;
		auto findHashes = hashesMap.equal_range(hash);
		auto findHashesBegin = findHashes.first;
		auto findHashesEnd = findHashes.second;
		for (auto findHash = findHashesBegin; findHash != findHashesEnd; ++findHash)
		{
			size_t databaseIndex = findHash->second;
			if (_database.at(databaseIndex).doesExist)
			{
				++outExistCount;
				outLastDatabaseIndex = databaseIndex;
				break;
			}
			else
			{
				++outNonExistCount;
				outLastDatabaseIndex = databaseIndex;
				if (1 < outNonExistCount)
					break;
			}
		}
	}

	void AssetDatabase::PurgeNonExistAssets()
	{
		// just weird c++ code to remove if some condition is met
		_database.erase(
				std::remove_if(
						_database.begin(),
						_database.end(),
						[](Asset& asset)
						{
							return !asset.doesExist;
						}),
				_database.end());

		RebuildAssetIDsMap();
	}

	void AssetDatabase::Update()
	{
		if (filesystem::exists(_assetDatabasePath))
		{
			std::ifstream inputFile(_assetDatabasePath);
			if (inputFile.is_open())
				inputFile >> *this;
			else
				PLATINUM_ERROR_STREAM << "AssetDatabase cannot be opened: " << _assetDatabasePath;
		}

		{
			std::vector<filesystem::path> currentExistingPaths;
			for (filesystem::recursive_directory_iterator i(_assetsFolderPath), end; i != end; ++i)
			{
				if (!is_directory(i->path()))
				{
					currentExistingPaths.push_back(i->path());
				}
			}
			UpdateWithCurrentPaths(currentExistingPaths);
		}

		{
			std::ofstream outputFile(_assetDatabasePath);
			if (outputFile.is_open())
				outputFile << *this;
			else
				PLATINUM_ERROR_STREAM << "AssetDatabase cannot be opened: " << _assetDatabasePath;
		}

		ReloadAssets();
	}

	void AssetDatabase::ReloadAssets()
	{
		// each entry corresponds to the loader in the ALLOWED_EXTENSIONS
		std::vector<std::function<void*(const std::filesystem::path &filePath)>> loaders{
				// remember to add the allocated data to its individual list
				[&](const std::filesystem::path& filePath) -> void*
				{
					Mesh* allocateMesh = new Mesh;
					*allocateMesh = Loaders::LoadMesh(filePath.string());
					_loadedMeshAssets.push_back(allocateMesh);
					allocateMesh->fileName = filePath.filename().string();
					return allocateMesh;
				},

				[&](const std::filesystem::path& filePath) -> void*
				{
					PixelData pixelData;
					Texture* allocateTexture = new Texture;
					pixelData.Create(filePath.string());
					allocateTexture->Create(pixelData.width, pixelData.height, (const void*)pixelData.pixelData);
					_loadedTextureAssets.emplace_back(allocateTexture);
					allocateTexture->fileName = filePath.filename().string();
					return allocateTexture;
				},
		};

		// check there's an entry for each ALLOWED_EXTENSIONS
		assert(loaders.size() == ALLOWED_EXTENSIONS.size());

		std::map<std::string, size_t> fileExtensionToLoader;
		for (size_t i = 0; i < ALLOWED_EXTENSIONS.size(); ++i)
		{
			// this will fail if there's duplicates in ALLOWED_EXTENSIONS
			assert(fileExtensionToLoader.insert({ ALLOWED_EXTENSIONS[i], i }).second);
		}

		DeleteLoadedAssets();
		_loadedAssets.resize(_database.size());

		for (size_t i = 0; i < _database.size(); ++i)
		{
			if (_database[i].doesExist)
			{
				auto filePath = _database[i].path;
				auto findFilePath = fileExtensionToLoader.find(GetExtension(filePath));

				if (findFilePath == fileExtensionToLoader.end())
				{
					PLATINUM_WARNING_STREAM << "AssetDatabase could not load asset, extension not supported: "
											<<
											_database[i].path;
				}
				else
				{
					size_t loadersIndex = findFilePath->second;
					_loadedAssets.at(i) = loaders.at(loadersIndex)(filePath);
				}
			}
		}
	}

	//--------------------------------------------------------------------------------------------------------------
	// Get Loaded Asset functions
	//--------------------------------------------------------------------------------------------------------------

	std::vector<MeshAssetID> AssetDatabase::GetMeshAssetIDs(bool requireExist) const
	{
		std::vector<MeshAssetID> results;
		for (const auto& asset: _database)
		{
			if (requireExist && !asset.doesExist)
				continue;

			if (GetExtension(asset.path.string()) == "obj")
			{
				results.push_back({ asset.id });
			}
		}
		return results;
	}

	Mesh* AssetDatabase::GetLoadedMeshAsset(MeshAssetID meshAssetID)
	{
		return (*this)[meshAssetID];
	}

	Mesh* AssetDatabase::operator[](MeshAssetID meshAssetID)
	{
		auto findID = _assetIDsMap.find(meshAssetID.id);
		if (findID == _assetIDsMap.end())
			return nullptr;

		size_t databaseIndex = findID->second;
		if (_database[databaseIndex].doesExist)
		{
			if (GetExtension(_database[databaseIndex].path.string()) == "obj")
				return static_cast<Mesh*>(_loadedAssets[databaseIndex]);
			else
				PLATINUM_WARNING_STREAM << "AssetDatabase: can't load get loaded asset because the type you want "
										   "is different to the type stored";
		}

		return nullptr;
	}

	std::vector<TextureAssetID> AssetDatabase::GetTextureAssetIDs(bool requireExist) const
	{
		std::vector<TextureAssetID> results;
		for (const auto& asset: _database)
		{
			if (requireExist && !asset.doesExist)
				continue;

			if (GetExtension(asset.path.string()) == "png")
			{
				results.push_back({ asset.id });
			}
		}
		return results;
	}

	Texture* AssetDatabase::GetLoadedTextureAsset(TextureAssetID textureAssetID)
	{
		return (*this)[textureAssetID];
	}

	Texture* AssetDatabase::operator[](TextureAssetID textureAssetID)
	{
		auto findID = _assetIDsMap.find(textureAssetID.id);
		if (findID == _assetIDsMap.end())
			return nullptr;

		size_t databaseIndex = findID->second;
		if (_database[databaseIndex].doesExist)
		{
			if (GetExtension(_database[databaseIndex].path.string()) == "png")
				return static_cast<Texture*>(_loadedAssets[databaseIndex]);
			else
				PLATINUM_WARNING_STREAM << "AssetDatabase: can't load get loaded asset because the type you want "
										   "is different to the type stored";
		}

		return nullptr;
	}

	//--------------------------------------------------------------------------------------------------------------
	// Internal
	//--------------------------------------------------------------------------------------------------------------

	AssetID AssetDatabase::GenerateAssetID()
	{
		AssetID assetID;
		do
		{
			assetID = _anyNumber(_generator);
		} while (0 < _assetIDsMap.count(assetID));
		return assetID;
	}

	void AssetDatabase::RebuildAssetIDsMap()
	{
		_assetIDsMap.clear();
		for (size_t i = 0; i < _database.size(); ++i)
		{
			if (!_assetIDsMap.insert({ _database[i].id, i }).second)
				throw std::exception();
		}
	}

	void AssetDatabase::UpdateWithCurrentPaths(
			const std::vector<filesystem::path>& currentExistingPaths,
			bool debugMessages)
	{
		// build map from paths to database index, 1-to-1
		std::map<filesystem::path, size_t> pathsMap;
		// build map from hashes to database index, 1-to-many
		std::multimap<HashType, size_t> hashesMap;

		{
			size_t databaseIndex = 0;
			for (auto& asset: _database)
			{
				// ensure current database is update to date
				asset.doesExist = filesystem::exists(asset.path);
				// if asset no longer exists, keep it in the database still,
				// asset might "come back" later
				// "come back" could be moved/renamed

				// if path insert failed because path is not unique
				assert(pathsMap.insert({ asset.path, databaseIndex }).second);
				hashesMap.insert({ asset.hash, databaseIndex });

				++databaseIndex;
			}
		}

		filesystem::path assetDatabasePath(_assetDatabasePath);

		// add any new assets in currentPaths
		for (auto& path: currentExistingPaths)
		{
			if (!filesystem::exists(path))
				// programmer made mistake with the input
				continue;
			if (!PlatinumEngine::ExtensionAllowed(PlatinumEngine::GetExtension(path.string())))
				// not "allowed"
				continue;
			if (path == assetDatabasePath)
				// don't store the assets database file
				continue;

			HashType hash = HashFile(path);

			auto findPath = pathsMap.find(path);
			if (findPath == pathsMap.end())
			{
				// no path
				size_t existCount;
				size_t nonExistCount;
				size_t lastDatabaseIndex;
				CountHashesInMap(hashesMap, hash, existCount, nonExistCount, lastDatabaseIndex);

				if (debugMessages)
				{
					if (existCount == 0 && nonExistCount == 0)
						PLATINUM_INFO_STREAM << "Detected new asset, add " << path;
					else if (existCount == 0 && nonExistCount == 1)
						PLATINUM_INFO_STREAM << "Detected asset was renamed/moved, edit " << path;
					else
						PLATINUM_INFO_STREAM << "Detected duplicated asset, add " << path;
				}

				if (existCount == 0 && nonExistCount == 1)
				{
					// asset previously existed, it was renamed/moved
					// update path in existing entry
					_database.at(lastDatabaseIndex).doesExist = true;
					_database.at(lastDatabaseIndex).path = path;
				}
				else
				{
					// probably new asset or
					// asset duplicated in another path

					// add new asset
					// looks like Vulkan ...
					Asset asset{
							GenerateAssetID(),
							hash,
							true,
							path
					};

					_database.push_back(asset);
					assert(_assetIDsMap.insert({ asset.id, _database.size() - 1 }).second);
					assert(pathsMap.insert({ path, _database.size() - 1 }).second);
					hashesMap.insert({ asset.hash, _database.size() - 1 });
				}
			}
			else
			{
				// found path
				size_t databaseIndex = findPath->second;
				// compare hash of current path with existing Asset
				if (hash == _database.at(databaseIndex).hash)
				{
					// good, nothing to do
					if (debugMessages)
					{
						PLATINUM_INFO_STREAM << "Detected existing asset, no change " << path;
					}
				}
				else
				{
					// hash is different to existing Asset
					size_t existCount;
					size_t nonExistCount;
					size_t lastDatabaseIndex;
					CountHashesInMap(hashesMap, hash, existCount, nonExistCount, lastDatabaseIndex);

					if (debugMessages)
					{
						if (existCount == 0 && nonExistCount == 0)
							PLATINUM_INFO_STREAM << "Existing asset edited, edit " << path;
						else if (existCount == 0 && nonExistCount == 1)
							PLATINUM_INFO_STREAM << "Detected asset was renamed/moved 2, edit " << path;
						else
							PLATINUM_INFO_STREAM << "Detected duplicated asset 2, edit " << path;
					}

					if (existCount == 0 && nonExistCount == 1)
					{
						// asset previously existed, it was renamed/moved
						// update path in existing entry
						_database.at(databaseIndex).path = path;
						_database.at(databaseIndex).doesExist = true;
					}
					else
					{
						// probably existing asset edited or
						// asset duplicated in another path

						// update existing entry
						_database.at(databaseIndex).hash = hash;
						_database.at(databaseIndex).doesExist = true;
					}
				}
			}
		}
	}

	void AssetDatabase::DeleteLoadedAssets()
	{
		for (auto mesh: _loadedMeshAssets)
		{
			delete mesh;
		}

		_loadedAssets.clear();
		_loadedMeshAssets.clear();
	}
}
