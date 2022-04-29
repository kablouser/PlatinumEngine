#pragma once

#include <map>
#include <vector>
#include <random>
#include <filesystem>

#include <OpenGL/Mesh.h>
#include <OpenGL/Texture.h>
namespace PlatinumEngine
{
	//--------------------------------------------------------------------------------------------------------------
	// Types
	//--------------------------------------------------------------------------------------------------------------

	typedef size_t AssetID;
	typedef uint64_t HashType;

	struct Asset
	{
		// note: id is NOT hash
		AssetID id;
		HashType hash;
		bool doesExist;
		std::filesystem::path path;
	};

	/**
	 * Same as AssetID, but guarantees this is Mesh
	 */
	struct MeshAssetID
	{
		AssetID id;
	};

	struct TextureAssetID
	{
		AssetID id;
	};
	
	struct AudioAssetID
	{
		AssetID id;
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

		~AssetDatabase();

		//--------------------------------------------------------------------------------------------------------------
		// Getters
		//--------------------------------------------------------------------------------------------------------------

		/**
		 * Attempt to retrieve asset using its id from database
		 * @param id key
		 * @param outAsset output asset if found, could be non-existent asset
		 * @return true iff id is in database
		 */
		bool TryGetAsset(AssetID id, Asset& outAsset);

		/**
		 * Attempt to retrieve asset using its filePath from database
		 * @param filePath to match
		 * @param outAsset output asset if found, could be non-existent asset
		 * @return true iff filePath is in database
		 */
		bool TryGetAsset(const std::string& filePath, Asset& outAsset);

		/**
		 * nodiscard means compiler creates warning if you don't use this function's return value
		 * @return readonly database
		 */
		[[nodiscard]] const std::vector<Asset>& GetDatabase() const;

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
		void Update();

		/**
		 * Loaded assets are deleted. Then loads existent assets in database.
		 * TODO only load assets that are in-use, otherwise leave them alone
		 */
		void ReloadAssets();

		//--------------------------------------------------------------------------------------------------------------
		// Get Loaded Asset functions
		//--------------------------------------------------------------------------------------------------------------

		//Mesh
		/**
		 * nodiscard means compiler creates warning if you don't use this function's return value
		 * @param requireExist if true, return only existent assets
		 * @return MeshAssetID that are in the database
		 */
		[[nodiscard]] std::vector<MeshAssetID> GetMeshAssetIDs(bool requireExist = true) const;

		/**
		 * nodiscard means compiler creates warning if you don't use this function's return value
		 * @param requireExist if true, return only existent assets
		 * @return MeshAssetID that are in the database
		 */
		[[nodiscard]] std::vector<AudioAssetID> GetAudioAssetIDs(bool requireExist = true) const;

		/**
		 * Get a mesh object from id. The object (if not null) is guaranteed to be loaded/ready
		 * @param meshAssetID to match
		 * @return nullptr if id doesn't exist or it's not a mesh, otherwise a loaded/ready mesh object
		 */
		Mesh* GetLoadedMeshAsset(MeshAssetID meshAssetID);

		/**
		 * Get a mesh object from id. The object (if not null) is guaranteed to be loaded/ready
		 * @param meshAssetID to match
		 * @return nullptr if id doesn't exist or it's not a mesh, otherwise a loaded/ready mesh object
		 */
		Mesh* operator[](MeshAssetID meshAssetID);

		//Texture
		[[nodiscard]] std::vector<TextureAssetID> GetTextureAssetIDs(bool requireExist = true) const;

		Texture* GetLoadedTextureAsset(TextureAssetID textureAssetID);

		Texture* operator[](TextureAssetID TextureAssetID);

		//Audio
		std::string operator[](AudioAssetID audioAssetID);

	private:

		std::string _assetsFolderPath;
		std::string _assetDatabasePath;

		std::vector<Asset> _database;

		// maps assetID to database index
		std::map<AssetID, size_t> _assetIDsMap;

		// random number generator
		std::mt19937 _generator;
		std::uniform_int_distribution<AssetID> _anyNumber;

		// pointers to loaded in assets
		std::vector<void*> _loadedAssets;
		// Assets loaded in their classes/structures appropriately
		std::vector<Mesh*> _loadedMeshAssets;

		std::vector<Texture*> _loadedTextureAssets;
		
		std::vector<std::string> _loadedAudioAssets;

		//--------------------------------------------------------------------------------------------------------------
		// Internal
		//--------------------------------------------------------------------------------------------------------------

		/**
		 * @return an unique AssetID that's not in database currently
		 */
		AssetID GenerateAssetID();

		/**
		 * Clears _assetIDsMap and rebuilds its structure
		 */
		void RebuildAssetIDsMap();

		/**
		 * Updates the database with list of current existing paths
		 * @param currentExistingPaths all paths to files that existing in the assets folder and its sub-folders
		 * @param debugMessages if true, messages are logged detailing how currentExistingPaths changes database
		 */
		void UpdateWithCurrentPaths(
				const std::vector<std::filesystem::path>& currentExistingPaths,
				bool debugMessages = true);

		void DeleteLoadedAssets();
	};
}