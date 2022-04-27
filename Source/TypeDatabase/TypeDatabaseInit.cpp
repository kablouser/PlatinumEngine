#include <TypeDatabase/TypeDatabase.h>
#include <IDSystem/IDSystem.h>
#include <SceneManager/Scene.h>
#include <ComponentComposition/GameObject.h>

void PlatinumEngine::TypeDatabase::Init()
{
	{
		auto idSystem = BeginTypeInfo<IDSystem>();
		auto& idSystemTypeInfo = _typeInfos.at(idSystem.typeInfoIndex);
		idSystemTypeInfo.streamIn = IDSystem::StreamIn;
		idSystemTypeInfo.streamOut = IDSystem::StreamOut;
	}

	Scene::CreateTypeInfo(*this);
	GameObject::CreateTypeInfo(*this);

	// this checks all fields/things are defined properly
	FinalCheck();
}