#pragma once

#include <map>
#include <vector>
#include <random>
#include <filesystem>

#include <TypeDatabase/TypeDatabase.h>
#include <SceneManager/Scene.h>

namespace PlatinumEngine
{
	//--------------------------------------------------------------------------------------------------------------
	// Types
	//--------------------------------------------------------------------------------------------------------------

	typedef uint64_t HashType;

	struct Asset
	{
		IDSystem::ID id;
		HashType hash; // not related to id
		bool doesExist;
		std::filesystem::path path;

		std::type_index GetTypeIndex() const; // implied from path
	};

	// Helper functions when you know the exact type of the asset
	template<typename T>
	struct AssetWithType
	{
		Asset asset;

		/**
		 * @param fromIDSystem using ids inside this system
		 * @return if loading was bad null, otherwise something non-null
		 */
		SavedReference<T> GetSavedReference(IDSystem& fromIDSystem) const
		{
			return fromIDSystem.template GetSavedReference<T>(asset.id);
		}
	};

	/**
	 * Keeps track of assets in assets folder, loads them, and manages a database file.
	 */
	class AssetDatabase
	{
	public:

		//--------------------------------------------------------------------------------------------------------------
		// Static
		//--------------------------------------------------------------------------------------------------------------

		// static constexpr aka constant expression means "copy and paste this like a macro"

		// 1MB
		static constexpr std::uintmax_t BIG_FILE_THRESHOLD_BYTES = 1000000;
		static constexpr const char* FIRST_LINE = "id, hash, doesExist, path";

		/**
		 * Write database to file
		 */
		friend std::ostream& operator<<(std::ostream& output, AssetDatabase& assetDatabase);

		/**
		 * Read database from file
		 */
		friend std::istream& operator>>(std::istream& input, AssetDatabase& assetDatabase);

		/**
		 * Calculates the hash of a file.
		 * Usually hashes the contents of the file.
		 * But if the file is "BIG", then the filename is hashed (See BIG_FILE_THRESHOLD_BYTES)
		 * @param path path to target file
		 * @return hash value
		 */
		static HashType HashFile(const std::filesystem::path& path);

		/**
		 * Converts different system file path formats into a standard
		 * Converts all \ to / and removes consecutive \ or /
		 * @param path to convert
		 * @return standardised path string
		 */
		static std::string StandardisePath(const std::filesystem::path& path);

		//--------------------------------------------------------------------------------------------------------------
		// Constructors/destructors
		//--------------------------------------------------------------------------------------------------------------

		AssetDatabase();

		AssetDatabase(std::string assetsFolderPath, std::string assetDatabasePath);

		//--------------------------------------------------------------------------------------------------------------
		// Getters
		//--------------------------------------------------------------------------------------------------------------

		/**
		 * Attempt to retrieve asset using its path
		 * @param withPath match with equivalent path
		 * @return {true, something} if path is in database, otherwise {false, nullptr}
		 */
		std::pair<bool, const Asset*> GetAsset(const std::filesystem::path &withPath);

		/**
		 * nodiscard means compiler creates warning if you don't use this function's return value
		 * @return readonly database
		 */
		[[nodiscard]] const std::vector<Asset>& GetAssets() const;

		//--------------------------------------------------------------------------------------------------------------
		// Database controls
		//--------------------------------------------------------------------------------------------------------------

		/**
		 * Count the number of times a hash appears in the map
		 * stops as soon as 0 < outExistCount || 1 < outNonExistCount
		 * @param hashesMap target map
		 * @param hash to match in map
		 * @param outExistCount output number of assets that exist
		 * @param outNonExistCount output number of assets that are non-existent
		 * @param outLastDatabaseIndex output last database index before loop terminated, -1 if loop never started
		 */
		void CountHashesInMap(
				std::multimap<HashType, size_t>& hashesMap,
				HashType hash,
				size_t& outExistCount,
				size_t& outNonExistCount,
				size_t& outLastDatabaseIndex);

		/**
		 * Database saves non-existent assets in-case they return in the future.
		 * Purge will delete these non-existent assets in the database.
		 * Only use when the database file is too large.
		 */
		void PurgeNonExistAssets();

		/**
		 * 1. read from database file
		 * 2. scan assets in assets folder
		 * 3. write to database file
		 * 4. ReloadAssets()
		 */
		void Update(IDSystem& idSystem, Scene& scene);

		//--------------------------------------------------------------------------------------------------------------
		// Get Loaded Asset functions
		//--------------------------------------------------------------------------------------------------------------

		template<typename T>
		std::vector<AssetWithType<T>> GetAssets()
		{
			auto findAssetIndices = _assetIndicesByType.find(std::type_index(typeid(T)));
			if (findAssetIndices == _assetIndicesByType.end())
				return {};
			else
			{
				std::vector<AssetWithType<T>> results(findAssetIndices->second.size());
				for (size_t i = 0; i < findAssetIndices->second.size(); ++i)
					results[i] = { _assets.at(findAssetIndices->second[i]) };
				return results;
			}
		}


	private:

		std::string _assetsFolderPath;
		std::string _assetDatabasePath;

		std::vector<Asset> _assets;
		// only loaded assets are inside this data structure
		std::map<std::type_index, std::vector<std::size_t>> _assetIndicesByType;

		//--------------------------------------------------------------------------------------------------------------
		// Internal
		//--------------------------------------------------------------------------------------------------------------

		// Clears _assetIndicesByType and rebuilds its structure
		void RebuildStoredMap();

		// Creates all current assets into the idSystem. Existing IDs' are overwritten.
		void CreateAssets(IDSystem& idSystem);

		// Removes all current assets from the fromIDSystem
		void RemoveAssets(IDSystem& fromIDSystem);

		/**
		 * Updates the database with list of current existing paths
		 * @param currentExistingPaths all paths to files that existing in the assets folder and its sub-folders
		 * @param debugMessages if true, messages are logged detailing how currentExistingPaths changes database
		 */
		void UpdateWithCurrentPaths(
				const std::vector<std::filesystem::path>& currentExistingPaths,
				IDSystem& idSystem,
				bool debugMessages = true);
	};
}