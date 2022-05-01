#include <TypeDatabase/TypeDatabase.h>
#include <IDSystem/IDSystem.h>
#include <SceneManager/Scene.h>
#include <ComponentComposition/GameObject.h>
#include <ComponentComposition/RenderComponent.h>

void PlatinumEngine::TypeDatabase::Init()
{
	{
		auto idSystem = BeginTypeInfo<IDSystem>();
		auto& idSystemTypeInfo = idSystem.GetTypeInfo();
		idSystemTypeInfo.streamIn = IDSystem::StreamIn;
		idSystemTypeInfo.streamOut = IDSystem::StreamOut;
	}

	Scene::CreateTypeInfo(*this);
	GameObject::CreateTypeInfo(*this);
	RenderComponent::CreateTypeInfo(*this);

	BeginTypeInfo<Mesh>();
	BeginTypeInfo<Texture>();

	// this checks all fields/things are defined properly
	FinalCheck();
}