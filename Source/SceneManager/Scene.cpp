//
// Created by Shihua on 09/03/2022.
//

#include <SceneManager/Scene.h>
#include <ComponentComposition/GameObject.h>
#include <ComponentComposition/Component.h>
#include <Helpers/VectorHelpers.h>
#include <Logger/Logger.h>

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
				.WithField<std::vector<SavedReference<GameObject>>>("_gameObjects", PLATINUM_OFFSETOF(Scene, _gameObjects))
				.WithField<std::vector<SavedReference<GameObject>>>("_rootGameObjects", PLATINUM_OFFSETOF(Scene, _rootGameObjects))
				.WithField<std::vector<SavedReference<GameObject>>>("_components", PLATINUM_OFFSETOF(Scene, _components));
	}

	//--------------------------------------------------------------------------------------------------------------
	// Constructors/destructors
	//--------------------------------------------------------------------------------------------------------------

	Scene::Scene(IDSystem& inIDSystem) : _isStarted(false), idSystem(inIDSystem)
	{
	}

	Scene::~Scene()
	{
		for (auto& gameObject: _gameObjects)
		{
			if (!idSystem.RemoveInternal(
					gameObject.id,
					std::type_index(
							typeid(gameObject.pointer.get())) // this only works because gameObject is polymorphic
			))
				PLATINUM_WARNING("ID System missing gameobject");
		}
		for (auto& component: _components)
		{
			if (!idSystem.RemoveInternal(
					component.id,
					std::type_index(typeid(component.pointer.get())) // this only works because component is polymorphic
			))
				PLATINUM_WARNING("ID System missing component");
		}

		// removal changes pointers in the id system
		idSystem.UpdateSavedReferences();
	}

	//--------------------------------------------------------------------------------------------------------------
	// _gameObjects controls
	//--------------------------------------------------------------------------------------------------------------

	PlatinumEngine::SavedReference<GameObject> Scene::AddGameObject(
			std::string name,
			SavedReference<GameObject> parent,
			bool isEnabled)
	{
		SavedReference<GameObject> gameObject = idSystem.Add<GameObject>(
				std::make_shared<GameObject>(name, parent, isEnabled));

		if (parent)
			parent.pointer->_children.push_back(gameObject);
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

		RemoveGameObjectRecurse(gameObject);

		// you only need to clean up the top-most parent. Because everything underneath is deleted anyway.
		SavedReference<GameObject>& parent = gameObject.pointer->GetParent();
		if (parent)
			parent.pointer->RemoveChild(gameObject);
		else
			RemoveRootGameObject(gameObject);

		// removal changes pointers in the id system
		idSystem.UpdateSavedReferences();
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
			if (component.pointer->_isEnabledInHierarchy)
				component.pointer->OnDisable(*this);
			component.pointer->OnEnd(*this);
		}

		if (component.pointer->_gameObject)
			component.pointer->_gameObject.pointer->RemoveComponent(component);

		if (!VectorHelpers::RemoveFirst(_components, component))
		{
			PLATINUM_ERROR("Hierarchy is invalid: _components is missing an element");
			return;
		}

		// practically, delete
		if (!idSystem.Remove(component))
			PLATINUM_ERROR("ID System missing component");
		// removal changes pointers in the id system
		idSystem.UpdateSavedReferences();
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

	//--------------------------------------------------------------------------------------------------------------
	// External controls
	//--------------------------------------------------------------------------------------------------------------

	void Scene::BroadcastOnStart(SavedReference<GameObject>& gameObject)
	{
		if (!gameObject)
			return;

		for (auto& component: gameObject.pointer->_components)
			component.pointer->OnStart(*this);

		for (auto& child: gameObject.pointer->_children)
			BroadcastOnStart(child);
	}

	void Scene::BroadcastOnEnd(SavedReference<GameObject>& gameObject)
	{
		if (!gameObject)
			return;

		for (auto& component: gameObject.pointer->_components)
			component.pointer->OnEnd(*this);

		for (auto& child: gameObject.pointer->_children)
			BroadcastOnEnd(child);
	}

	void Scene::BroadcastOnEnable(SavedReference<GameObject>& gameObject)
	{
		if (!gameObject || !gameObject.pointer->_isEnabledInHierarchy)
			return;

		for (auto& component: gameObject.pointer->_components)
			if (component.pointer->_isEnabledInHierarchy)
				component.pointer->OnEnable(*this);

		for (auto& child: gameObject.pointer->_children)
			BroadcastOnEnable(child);
	}

	void Scene::BroadcastOnDisable(SavedReference<GameObject>& gameObject)
	{
		if (!gameObject || !gameObject.pointer->_isEnabledInHierarchy)
			return;

		for (auto& component: gameObject.pointer->_components)
			if (component.pointer->_isEnabledInHierarchy)
				component.pointer->OnDisable(*this);

		for (auto& child: gameObject.pointer->_children)
			BroadcastOnDisable(child);
	}

	void Scene::BroadcastOnUpdate(SavedReference<GameObject>& gameObject, double deltaTime)
	{
		if (!gameObject || !gameObject.pointer->_isEnabledInHierarchy)
			return;

		for (auto& component: gameObject.pointer->_components)
			if (component.pointer->_isEnabledInHierarchy)
				component.pointer->OnUpdate(*this, deltaTime);

		for (auto& child: gameObject.pointer->_children)
			BroadcastOnUpdate(child, deltaTime);
	}

	void Scene::BroadcastOnRender(SavedReference<GameObject>& gameObject, Renderer& renderer)
	{
		if (!gameObject || !gameObject.pointer->_isEnabledInHierarchy)
			return;

		for (auto& component: gameObject.pointer->_components)
			if (component.pointer->_isEnabledInHierarchy)
				component.pointer->OnRender(*this, renderer);

		for (auto& child: gameObject.pointer->_children)
			BroadcastOnRender(child, renderer);
	}

	void Scene::UpdateIsEnabledInHierarchy(SavedReference<GameObject>& gameObject)
	{
		if (!gameObject)
			return;

		// recursively, broadcast the function UpdateIsEnabledInHierarchy to all it's components and children
		for (auto& component: gameObject.pointer->_components)
			component.pointer->UpdateIsEnabledInHierarchy(*this);

		for (auto& child: gameObject.pointer->_children)
			// recurse
			// UpdateIsEnabledInHierarchy could further call UpdateIsEnabledInHierarchy
			child.pointer->UpdateIsEnabledInHierarchy(*this, child);
	}

	void Scene::AddComponentInternal(
			SavedReference<Component> component,
			SavedReference<GameObject> gameObject,
			bool isEnabled)
	{
		_components.push_back(component);
		// basically forcing a constructor on all components, regardless of base calling
		component.pointer->_gameObject = gameObject;
		component.pointer->_isEnabled = isEnabled;
		component.pointer->_isEnabledInHierarchy = component.pointer->CalculateIsEnabledInHierarchy();

		if (gameObject)
			gameObject.pointer->_components.push_back(component);

		if (_isStarted)
		{
			component.pointer->OnStart(*this);
			if (component.pointer->_isEnabledInHierarchy)
			{
				component.pointer->OnEnable(*this);
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
		for (auto& child: gameObject.pointer->_children)
			RemoveGameObjectRecurse(child);

		for (auto& component: gameObject.pointer->_components)
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