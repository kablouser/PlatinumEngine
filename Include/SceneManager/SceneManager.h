#pragma once
// for interface for SaveFile
// TODO replace with file manager's File/Resource type
#include <string>
#include <SceneManager/Scene.h>

namespace PlatinumEngine
{
	class SceneManager
	{
	public:

		// We can use default implementation because we only have class fields, no primitive types

		SceneManager() = default;
		~SceneManager() = default;

		// Delete all these operators. Messes up data.

		SceneManager(SceneManager const&) = delete;
		SceneManager& operator=(SceneManager const&) = delete;
		SceneManager(SceneManager&&) noexcept = delete;
		SceneManager& operator=(SceneManager&&) noexcept = delete;

		// returns _scene
		Scene& GetScene();

		// Copies _cacheScene into _scene, called after game has stopped
		void ReloadCacheScene();
		// Copies _scene into _cacheScene, called before game has started
		void CacheScene();

		// Copies _scene into file
		bool SaveFile(const std::string& file);
		// Copies data from file into _scene
		bool LoadFile(const std::string& file);

	private:
		// current scene that you're working on
		Scene _scene;
	};
}