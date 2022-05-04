//
// Created by Shihua on 09/03/2022.
//

#include <SceneManager/Scene.h>
#include <ComponentComposition/GameObject.h>
#include <ComponentComposition/Component.h>
#include <Helpers/VectorHelpers.h>
#include <Logger/Logger.h>
#include <fstream>

namespace PlatinumEngine
{
	//------------------------------------------------------------------------------------------------------------------
	// Serialization stuff
	//------------------------------------------------------------------------------------------------------------------

	void Scene::CreateTypeInfo(TypeDatabase& typeDatabase)
	{
		/*
		bool _isStarted;
		std::vector<SavedReference<GameObject>> _gameObjects;
		std::vector<SavedReference<GameObject>> _rootGameObjects;
		std::vector<SavedReference<Component>> _components;
		 */
		typeDatabase.BeginTypeInfoWithoutAllocator<Scene>()
				.WithField<bool>("_isStarted", PLATINUM_OFFSETOF(Scene, _isStarted))
				.WithField<std::vector<SavedReference<GameObject>>>("_gameObjects",
						PLATINUM_OFFSETOF(Scene, _gameObjects))
				.WithField<std::vector<SavedReference<GameObject>>>("_rootGameObjects",
						PLATINUM_OFFSETOF(Scene, _rootGameObjects))
				.WithField<std::vector<SavedReference<Component>>>("_components",
						PLATINUM_OFFSETOF(Scene, _components));
	}

	//--------------------------------------------------------------------------------------------------------------
	// Constructors/destructors
	//--------------------------------------------------------------------------------------------------------------

	Scene::Scene(IDSystem& inIDSystem, Physics& physicsRef) : _isStarted(false), idSystem(inIDSystem), physics(physicsRef)
	{
	}

	void Scene::LoadFile(std::string filePath)
	{
		TypeDatabase* typeDatabase = TypeDatabase::Instance;
		if (!typeDatabase)
		{
			PLATINUM_ERROR("No TypeDatabase");
			return;
		}

		std::ifstream loadFile(filePath, std::ios::binary); // Windows needs binary to use seekg()
		if (loadFile.is_open())
		{
			// first delete existing scene data
			idSystem.Clear();
			Clear();
			// then deserialize
			TypeDatabase::DeserializeReturnCode code = typeDatabase->Deserialize(loadFile, &idSystem);
			if (code != TypeDatabase::DeserializeReturnCode::success)
				PLATINUM_WARNING_STREAM << "Loading ID System has return code " << (int)code;

			code = typeDatabase->Deserialize(loadFile, this);
			if (code != TypeDatabase::DeserializeReturnCode::success)
				PLATINUM_WARNING_STREAM << "Loading ID System has return code " << (int)code;

			AfterLoad();
		}
		else
			PLATINUM_ERROR_STREAM << "Could not open scene file: " << filePath;
	}

	void Scene::SaveFile(std::string filePath)
	{
		TypeDatabase* typeDatabase = TypeDatabase::Instance;
		if (!typeDatabase)
		{
			PLATINUM_ERROR("No TypeDatabase");
			return;
		}

		std::ofstream saveFile(filePath);
		if (saveFile.is_open())
		{
			typeDatabase->Serialize(saveFile, &idSystem);
			typeDatabase->Serialize(saveFile, this);
		}
		else
			PLATINUM_ERROR_STREAM << "Could not open scene file: " << filePath;
	}

	void Scene::Clear()
	{
		// id system is cleared separately
		_gameObjects.clear();
		_rootGameObjects.clear();
		_components.clear();
	}

	void Scene::AfterLoad()
	{
		OnIDSystemUpdate();

		// attempt to fix corrupt data
		// relink gameobject->child pointers and gameobject->component
		for (auto& gameObject: _gameObjects)
		{
			if (!gameObject)
				continue;

			for (auto& child: gameObject.DeRef()->_children)
			{
				if (!child)
					continue;
				child.DeRef()->_parent = gameObject;
			}

			for (auto& component: gameObject.DeRef()->_components)
			{
				if (!component)
					continue;
				component.DeRef()->_gameObject = gameObject;
			}
		}

		for (auto& rootGameObject: _rootGameObjects)
			UpdateIsEnabledInHierarchy(rootGameObject);
	}

	//--------------------------------------------------------------------------------------------------------------
	// _gameObjects controls
	//--------------------------------------------------------------------------------------------------------------

	PlatinumEngine::SavedReference<GameObject> Scene::AddGameObject(
			std::string name,
			SavedReference<GameObject> parent,
			bool isEnabled)
	{
		SavedReference<GameObject> gameObject = idSystem.Add<GameObject>();
		{
			// constructor basically,
			GameObject* gameObjectPointer = gameObject.DeRef().get();
			gameObjectPointer->name = name;
			gameObjectPointer->_parent = parent;
			gameObjectPointer->_isEnabled = isEnabled;
			gameObjectPointer->_isEnabledInHierarchy = gameObjectPointer->CalculateIsEnabledInHierarchy();
		}

		if (parent)
			parent.DeRef()->_children.push_back(gameObject);
		else
			_rootGameObjects.push_back(gameObject);

		_gameObjects.push_back(gameObject);
		return gameObject;
	}

	void Scene::RemoveGameObject(SavedReference<GameObject>& gameObject)
	{
		if (!gameObject)
			return;

		// events are only called when the scene has started
		if (_isStarted)
		{
			// trigger end of life cycle events
			BroadcastOnDisable(gameObject);
			BroadcastOnEnd(gameObject);
		}
		// you only need to clean up the top-most parent. Because everything underneath is deleted anyway.
		SavedReference<GameObject>& parent = gameObject.DeRef()->GetParent();
		if (parent)
			parent.DeRef()->RemoveChild(gameObject);
		else
			RemoveRootGameObject(gameObject);

		RemoveGameObjectRecurse(gameObject);
		// removal changes pointers in the id system
		OnIDSystemUpdate();
	}

	size_t Scene::GetGameObjectsCount() const
	{
		return _gameObjects.size();
	}

	//--------------------------------------------------------------------------------------------------------------
	// _rootGameObjects controls
	//--------------------------------------------------------------------------------------------------------------

	size_t Scene::GetRootGameObjectsCount() const
	{
		return _rootGameObjects.size();
	}

	SavedReference<GameObject>& Scene::GetRootGameObject(size_t index)
	{
		// range checked indexing
		return _rootGameObjects.at(index);
	}

	bool Scene::MoveRootGameObjectPositionInList(
			SavedReference<GameObject>& targetObject,
			SavedReference<GameObject>& movedGameObject)
	{
		// get iterators for the selected target game object
		auto targetGameObjectIterator = std::find(_rootGameObjects.begin(), _rootGameObjects.end(), targetObject);

		// get iterator for the moved game object
		auto movedGameObjectIterator = std::find(_rootGameObjects.begin(), _rootGameObjects.end(), movedGameObject);

		// use rotate to move the item in front of the target object
		if (targetGameObjectIterator != _rootGameObjects.end() && movedGameObjectIterator != _rootGameObjects.end())
		{
			if (movedGameObjectIterator < targetGameObjectIterator)
				std::rotate(movedGameObjectIterator, movedGameObjectIterator + 1, targetGameObjectIterator + 1);

			else if (movedGameObjectIterator > targetGameObjectIterator)
				std::rotate(targetGameObjectIterator, movedGameObjectIterator, movedGameObjectIterator + 1);

			// return true if moving successes
			return true;
		}

		// return false if moving fails
		return false;
	}


	//--------------------------------------------------------------------------------------------------------------
	// _components controls
	//--------------------------------------------------------------------------------------------------------------

	void Scene::RemoveComponentInternal(SavedReference<Component> component)
	{
		if (!component)
			return;

		// trigger end of life cycle events
		if (_isStarted)
		{
			// OnDisable specification
			if (component.DeRef()->_isEnabledInHierarchy)
				component.DeRef()->OnDisable(*this);
			component.DeRef()->OnEnd(*this);
		}

		if (component.DeRef()->_gameObject)
			component.DeRef()->_gameObject.DeRef()->RemoveComponent(component);

		if (!VectorHelpers::RemoveFirst(_components, component))
		{
			PLATINUM_ERROR("Hierarchy is invalid: _components is missing an element");
			return;
		}

		// practically, delete
		if (!idSystem.Remove(component))
			PLATINUM_ERROR("ID System missing component");
		// removal changes pointers in the id system
		OnIDSystemUpdate();
	}

	size_t Scene::GetComponentsCount() const
	{
		return _components.size();
	}

	//--------------------------------------------------------------------------------------------------------------
	// Event controls
	//--------------------------------------------------------------------------------------------------------------

	bool Scene::IsStarted() const
	{
		return _isStarted;
	}

	void Scene::Start()
	{
		if (_isStarted)
		{
			PLATINUM_WARNING("Scene::Start() called when scene has already started. Must call Scene::End() first.");
			return;
		}

		_isStarted = true;

		for (auto& gameObject: _rootGameObjects)
			BroadcastOnStart(gameObject);

		for (auto& gameObject: _rootGameObjects)
			BroadcastOnEnable(gameObject);
	}

	void Scene::End()
	{
		if (!_isStarted)
		{
			PLATINUM_WARNING("Scene::End() called when scene has already ended. Must call Scene::Start() first.");
			return;
		}

		_isStarted = false;

		// OnDisable before OnEnd() according to specification

		for (auto& gameObject: _rootGameObjects)
			BroadcastOnDisable(gameObject);

		for (auto& gameObject: _rootGameObjects)
			BroadcastOnEnd(gameObject);
	}

	void Scene::Update(double deltaTime)
	{
		for (auto& gameObject: _rootGameObjects)
			BroadcastOnUpdate(gameObject, deltaTime);
	}

	void Scene::Render(Renderer& renderer)
	{
		for (auto& gameObject: _rootGameObjects)
			BroadcastOnRender(gameObject, renderer);
	}

	void Scene::OnIDSystemUpdate()
	{
		for (auto& gameObject: _gameObjects)
			gameObject.OnIDSystemUpdate(idSystem);
		for (auto& rootGameObject: _rootGameObjects)
			rootGameObject.OnIDSystemUpdate(idSystem);
		for (auto& component : _components)
			component.OnIDSystemUpdate(idSystem);

		for (auto& gameObject: _rootGameObjects)
			BroadcastOnIDSystemUpdate(gameObject);
	}

	//--------------------------------------------------------------------------------------------------------------
	// External controls
	//--------------------------------------------------------------------------------------------------------------

	void Scene::BroadcastOnStart(SavedReference<GameObject>& gameObject)
	{
		if (!gameObject)
			return;

		for (auto& component: gameObject.DeRef()->_components)
			component.DeRef()->OnStart(*this);

		for (auto& child: gameObject.DeRef()->_children)
			BroadcastOnStart(child);
	}

	void Scene::BroadcastOnEnd(SavedReference<GameObject>& gameObject)
	{
		if (!gameObject)
			return;

		for (auto& component: gameObject.DeRef()->_components)
			component.DeRef()->OnEnd(*this);

		for (auto& child: gameObject.DeRef()->_children)
			BroadcastOnEnd(child);
	}

	void Scene::BroadcastOnEnable(SavedReference<GameObject>& gameObject)
	{
		if (!gameObject || !gameObject.DeRef()->_isEnabledInHierarchy)
			return;

		for (auto& component: gameObject.DeRef()->_components)
			if (component.DeRef()->_isEnabledInHierarchy)
				component.DeRef()->OnEnable(*this);

		for (auto& child: gameObject.DeRef()->_children)
			BroadcastOnEnable(child);
	}

	void Scene::BroadcastOnDisable(SavedReference<GameObject>& gameObject)
	{
		if (!gameObject || !gameObject.DeRef()->_isEnabledInHierarchy)
			return;

		for (auto& component: gameObject.DeRef()->_components)
			if (component.DeRef()->_isEnabledInHierarchy)
				component.DeRef()->OnDisable(*this);

		for (auto& child: gameObject.DeRef()->_children)
			BroadcastOnDisable(child);
	}

	void Scene::BroadcastOnUpdate(SavedReference<GameObject>& gameObject, double deltaTime)
	{
		if (!gameObject || !gameObject.DeRef()->_isEnabledInHierarchy)
			return;

		for (auto& component: gameObject.DeRef()->_components)
			if (component.DeRef()->_isEnabledInHierarchy)
				component.DeRef()->OnUpdate(*this, deltaTime);

		for (auto& child: gameObject.DeRef()->_children)
			BroadcastOnUpdate(child, deltaTime);
	}

	void Scene::BroadcastOnRender(SavedReference<GameObject>& gameObject, Renderer& renderer)
	{
		if (!gameObject || !gameObject.DeRef()->_isEnabledInHierarchy)
			return;

		for (auto& component: gameObject.DeRef()->_components)
			if (component.DeRef()->_isEnabledInHierarchy)
				component.DeRef()->OnRender(*this, renderer);

		for (auto& child: gameObject.DeRef()->_children)
			BroadcastOnRender(child, renderer);
	}

	void Scene::UpdateIsEnabledInHierarchy(SavedReference<GameObject>& gameObject)
	{
		if (!gameObject)
			return;

		// recursively, broadcast the function UpdateIsEnabledInHierarchy to all it's components and children
		for (auto& component: gameObject.DeRef()->_components)
			component.DeRef()->UpdateIsEnabledInHierarchy(*this);

		for (auto& child: gameObject.DeRef()->_children)
			// recurse
			// UpdateIsEnabledInHierarchy could further call UpdateIsEnabledInHierarchy
			child.DeRef()->UpdateIsEnabledInHierarchy(*this, child);
	}

	void Scene::BroadcastOnIDSystemUpdate(SavedReference<GameObject>& gameObject)
	{
		if (!gameObject)
			return;

		GameObject* gameObjectPointer = gameObject.DeRef().get();

		for (auto& component: gameObjectPointer->_components)
			component.OnIDSystemUpdate(idSystem);

		for (auto& component: gameObjectPointer->_components)
		{
			if (!component)
				continue;
			Component* componentPointer = component.DeRef().get();
			componentPointer->_gameObject.OnIDSystemUpdate(idSystem);
			componentPointer->OnIDSystemUpdate(*this);
		}

		// recurse
		for (auto& child: gameObjectPointer->_children)
			BroadcastOnIDSystemUpdate(child);
	}

	void Scene::AddComponentInternal(
			SavedReference<Component> component,
			SavedReference<GameObject> gameObject,
			bool isEnabled)
	{
		_components.push_back(component);
		// basically forcing a constructor on all components, regardless of base calling
		component.DeRef()->_gameObject = gameObject;
		component.DeRef()->_isEnabled = isEnabled;
		component.DeRef()->_isEnabledInHierarchy = component.DeRef()->CalculateIsEnabledInHierarchy();

		if (gameObject)
			gameObject.DeRef()->_components.push_back(component);

		if (_isStarted)
		{
			component.DeRef()->OnStart(*this);
			if (component.DeRef()->_isEnabledInHierarchy)
			{
				component.DeRef()->OnEnable(*this);
			}
		}
	}

	//--------------------------------------------------------------------------------------------------------------
	// Internal controls
	//--------------------------------------------------------------------------------------------------------------

	void Scene::RemoveGameObjectRecurse(SavedReference<GameObject>& gameObject)
	{
		if (!gameObject)
			return;

		// recurse
		for (auto& child: gameObject.DeRef()->_children)
			RemoveGameObjectRecurse(child);

		for (auto& component: gameObject.DeRef()->_components)
		{
			if (!VectorHelpers::RemoveFirst(_components, component))
				PLATINUM_ERROR("Hierarchy is invalid: _components is missing an element");

			if (!idSystem.Remove(component))
				// Component should be in the id system, if not then component wasn't being tracked and this is very bad
				PLATINUM_ERROR("ID System missing component");
		}

		if (!VectorHelpers::RemoveFirst(_gameObjects, gameObject))
			PLATINUM_ERROR("Hierarchy is invalid: _gameObjects is missing an element");

		if (!idSystem.Remove(gameObject))
			// GameObject should be in the id system, if not then GameObject wasn't being tracked and this is very bad
			PLATINUM_ERROR("ID System missing GameObject");
	}

	void Scene::RemoveRootGameObject(SavedReference<GameObject>& rootGameObject)
	{
		if (!rootGameObject)
			return;

		if (!VectorHelpers::RemoveFirst(_rootGameObjects, rootGameObject))
			PLATINUM_ERROR("Hierarchy is invalid: _rootGameObjects is missing an element");
	}

}