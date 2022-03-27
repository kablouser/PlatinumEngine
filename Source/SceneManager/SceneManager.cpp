#include <SceneManager/SceneManager.h>
// for move semantics
#include <utility>

namespace PlatinumEngine
{
	Scene& SceneManager::GetScene()
	{
		return _scene;
	}

	void SceneManager::ReloadCacheScene()
	{
		// TODO requires serialization
	}

	void SceneManager::CacheScene()
	{
		// TODO requires serialization
	}

	bool SceneManager::SaveFile(const std::string& file)
	{
		// TODO requires serialization
		return false;
	}

	bool SceneManager::LoadFile(const std::string& file)
	{
		// TODO requires serialization
		return false;
	}
}