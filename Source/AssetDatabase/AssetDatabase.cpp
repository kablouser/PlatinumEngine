#include <AssetDatabase/AssetDatabase.h>

#include <limits>
#include <cassert>
#include <fstream>

#include <Logger/Logger.h>
#include <Loaders/LoaderCommon.h>
#include <Loaders/MeshLoader.h>
#include <TypeDatabase/TypeDatabase.h>
#include <Audio/AudioClip.h>

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

		for (auto c: text)
		{
			hash ^= c;
			hash *= fnv_prime;
		}

		return hash;
	}
}

namespace PlatinumEngine
{
	std::type_index Asset::GetTypeIndex() const
	{
		auto findExtension = Loaders::EXTENSION_TO_TYPE.find(path.extension().string());
		if (findExtension == Loaders::EXTENSION_TO_TYPE.end())
			return std::type_index(typeid(void));
		else
			return findExtension->second;
	}

	//--------------------------------------------------------------------------------------------------------------
	// Static
	//--------------------------------------------------------------------------------------------------------------

	std::ostream& operator<<(std::ostream& output, AssetDatabase& assetDatabase)
	{
		output << AssetDatabase::FIRST_LINE << std::endl;
		for (const auto& asset: assetDatabase._assets)
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
		// enforce an empty object,
		// AssetDatabase only deals with inputs. There is no reason to write out changes in it's data structure.
		assetDatabase._assets.clear();
		assetDatabase._assetIndicesByType.clear();

		{
			std::string firstLine;
			std::getline(input, firstLine);
			if (firstLine != AssetDatabase::FIRST_LINE)
				PLATINUM_WARNING("AssetDatabase stream input: unexpected first line");
		}

		Asset asset;
		std::string filePath;
		size_t line = 1;
		while (input >> asset.id && input.get() &&
			   input >> asset.hash && input.get() &&
			   input >> asset.doesExist && input.get() &&
			   std::getline(input, filePath))
		{
			// don't check asset here, checks come later
			asset.path = filesystem::path(AssetDatabase::StandardisePath(filePath));
			assetDatabase._assets.push_back(asset);
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
			std::ifstream fileStream(path, std::ios::binary);
			if (fileStream.is_open())
			{
				std::ostringstream buffer;
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

	//--------------------------------------------------------------------------------------------------------------
	// Getters
	//--------------------------------------------------------------------------------------------------------------

	std::pair<bool, const Asset*> AssetDatabase::GetAsset(std::filesystem::path withPath)
	{
		for (Asset& asset: _assets)
		{
			if (std::filesystem::equivalent(asset.path, withPath))
				return { true, &asset };
		}
		return { false, nullptr };
	}

	const std::vector<Asset>& AssetDatabase::GetAssets() const
	{
		return _assets;
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
			if (_assets.at(databaseIndex).doesExist)
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
		_assets.erase(
				std::remove_if(
						_assets.begin(),
						_assets.end(),
						[](Asset& asset)
						{
							return !asset.doesExist;
						}),
				_assets.end());

		RebuildStoredMap();
	}

	void AssetDatabase::Update(IDSystem& idSystem, Scene& scene)
	{
		// Remove current assets from ID System
		// Not strictly necessary to make things work
		// But nice to let user know when stuff if no longer in the AssetDatabase
		RemoveAssets(idSystem);

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
				if (!is_directory(i->path()))
					currentExistingPaths.push_back(i->path());
			UpdateWithCurrentPaths(currentExistingPaths, idSystem);
		}

		{
			std::ofstream outputFile(_assetDatabasePath);
			if (outputFile.is_open())
				outputFile << *this;
			else
				PLATINUM_ERROR_STREAM << "AssetDatabase cannot be opened: " << _assetDatabasePath;
		}

		// Add new assets to id system
		CreateAssets(idSystem);
		// SavedReferences will have old pointers, update them all to the new loaded data
		scene.OnIDSystemUpdate();
		// finally, rebuild map structure
		RebuildStoredMap();
	}

	void AssetDatabase::CreateAssets(IDSystem& idSystem)
	{
		// TODO only load assets that are in-use, otherwise leave them alone
		for (Asset& asset: _assets)
		{
			if (!asset.doesExist || asset.id == 0)
				continue;

			PLATINUM_INFO_STREAM << "Creating asset: " << asset.path;

			auto findExtension = Loaders::EXTENSION_TO_TYPE.find(asset.path.extension().string());
			if (findExtension == Loaders::EXTENSION_TO_TYPE.end())
			{
				PLATINUM_WARNING_STREAM << "AssetDatabase could not load asset, extension not supported: "
										<< asset.path;
				continue;
			}

			if (findExtension->second == typeid(Mesh))
			{
				auto[success, mesh]= Loaders::LoadMesh(asset.path);
				if (!success)
					continue;
				idSystem.Overwrite<Mesh>(asset.id, std::move(mesh));
			}
			else if (findExtension->second == typeid(Texture))
			{
				PixelData pixelData;
				if (!pixelData.Create(asset.path.string()))
					continue;
				Texture texture;
				texture.fileName = asset.path.filename().string();
				texture.Create(pixelData);
				idSystem.Overwrite<Texture>(asset.id, std::move(texture));
			}
			else if (findExtension->second == typeid(AudioClip))
			{
				AudioClip audioClip(asset.path.string().c_str());
				if (!audioClip.IsValid())
					continue;
				idSystem.Overwrite<AudioClip>(asset.id, std::move(audioClip));
			}
			else
			{
				PLATINUM_WARNING_STREAM << "AssetDatabase could not load asset, extension not supported: "
										<< asset.path;
				continue;
			}
		}
		PLATINUM_INFO("Finished creating assets.");
	}

	//--------------------------------------------------------------------------------------------------------------
	// Internal
	//--------------------------------------------------------------------------------------------------------------

	void AssetDatabase::RebuildStoredMap()
	{
		_assetIndicesByType.clear();
		for (size_t i = 0; i < _assets.size(); ++i)
		{
			if (!_assets[i].doesExist)
				continue; // exclude non-existent assets
			_assetIndicesByType[_assets[i].GetTypeIndex()].push_back(i);
		}
	}

	void AssetDatabase::RemoveAssets(IDSystem& fromIDSystem)
	{
		for (Asset& asset: _assets)
		{
			if (!asset.doesExist)
				continue;

			bool isRemoved = fromIDSystem.RemoveInternal(asset.id, asset.GetTypeIndex());
			if (!isRemoved)
				PLATINUM_WARNING_STREAM << "Asset was expected to be in ID System. Something has gone wrong. "
										<< asset.path;
		}

		_assets.clear();
		_assetIndicesByType.clear();
	}

	void AssetDatabase::UpdateWithCurrentPaths(
			const std::vector<filesystem::path>& currentExistingPaths,
			IDSystem& idSystem,
			bool debugMessages)
	{
		// build map from paths to database index, 1-to-1
		std::map<filesystem::path, size_t> pathsMap;
		// build map from hashes to database index, 1-to-many
		std::multimap<HashType, size_t> hashesMap;

		{
			size_t databaseIndex = 0;
			for (auto& asset: _assets)
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
			if (!PlatinumEngine::Loaders::ExtensionAllowed(path))
				// not "allowed"
				continue;
			if (path == assetDatabasePath)
				// don't store the asset database file
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
					_assets.at(lastDatabaseIndex).doesExist = true;
					_assets.at(lastDatabaseIndex).path = path;
				}
				else
				{
					// probably new asset or
					// asset duplicated in another path

					// add new asset
					// looks like Vulkan ...
					Asset asset{
							0, // id = 0 means nullptr, temporary. We set id below vvv
							hash,
							true,
							path
					};

					// Reserve an ID for this asset in the id system
					std::type_index typeIndex = asset.GetTypeIndex();
					auto[success, typeInfo]=TypeDatabase::Instance->GetTypeInfo(typeIndex);
					if (success && typeInfo->allocate)
					{
						std::shared_ptr<void> empty = typeInfo->allocate();
						auto[id, _]= idSystem.AddInternal(empty, typeIndex);
						asset.id = id;

						// we will overwrite this empty data with proper loaded data later in CreateAssets()
						_assets.push_back(asset);
						assert(pathsMap.insert({ path, _assets.size() - 1 }).second);
						hashesMap.insert({ asset.hash, _assets.size() - 1 });
					}
					else
					{
						PLATINUM_ERROR_STREAM << "Can't add to database because no allocator for asset: " << path;
					}
				}
			}
			else
			{
				// found path
				size_t databaseIndex = findPath->second;
				// compare hash of current path with existing Asset
				if (hash == _assets.at(databaseIndex).hash)
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
						_assets.at(databaseIndex).path = path;
						_assets.at(databaseIndex).doesExist = true;
					}
					else
					{
						// probably existing asset edited or
						// asset duplicated in another path

						// update existing entry
						_assets.at(databaseIndex).hash = hash;
						_assets.at(databaseIndex).doesExist = true;
					}
				}
			}
		}
	}
}
