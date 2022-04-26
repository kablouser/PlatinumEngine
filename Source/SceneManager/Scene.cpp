//
// Created by Shihua on 09/03/2022.
//

#include <SceneManager/Scene.h>
#include <ComponentComposition/GameObject.h>
#include <ComponentComposition/Component.h>
#include <Helpers/VectorHelpers.h>
#include <Logger/Logger.h>

#include <utility>

namespace PlatinumEngine
{
	//--------------------------------------------------------------------------------------------------------------
	// Constructors/destructors
	//--------------------------------------------------------------------------------------------------------------

	Scene::Scene(IDSystem& inIDSystem) : _isStarted(false), idSystem(inIDSystem)
	{
	}

	Scene::~Scene()
	{
		for (auto gameObject: _gameObjects)
		{
			idSystem.RemoveInternal(
					gameObject.id,
					std::type_index(typeid(gameObject.pointer.get())) // this only works because gameObject is polymorphic
					);
		}
		for (auto component: _components)
		{
			idSystem.RemoveInternal(
					component.id,
					std::type_index(typeid(component.pointer.get())) // this only works because component is polymorphic
			);
		}
	}

	//--------------------------------------------------------------------------------------------------------------
	// _gameObjects controls
	//--------------------------------------------------------------------------------------------------------------

	PlatinumEngine::SavedReference<GameObject> Scene::AddGameObject(
			std::string name,
			SavedReference<GameObject> parent,
			bool isEnabled)
	{
		SavedReference<GameObject> newGameObject(
				new GameObject(std::move(name), std::move(parent), isEnabled),
				idSystem);

		if (parent)
			parent.pointer->_children.push_back(newGameObject);
		else
			_rootGameObjects.push_back(newGameObject);

		_gameObjects.push_back(newGameObject);
		return newGameObject;
	}

	void Scene::RemoveGameObject(SavedReference<GameObject> gameObject)
	{
		// events are only called when the scene has started
		if (_isStarted)
		{
			// trigger end of life cycle events
			BroadcastOnDisable(gameObject);
			BroadcastOnEnd(gameObject);
		}
		RemoveGameObjectRecurse(gameObject);

		// you only need to clean up the top-most parent. Because everything underneath is deleted anyway.
		GameObject* parent = gameObject.GetParent();
		if(parent == nullptr)
		{
			RemoveRootGameObject(gameObject);
		}
		else
		{
			parent->RemoveChild(&gameObject);
		}
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

	bool Scene::MoveRootGameObjectPositionInList(GameObject* targetObject, GameObject* movedGameObject)
	{
		// get iterators for the selected target game object
		auto targetGameObjectIterator = std::find(_rootGameObjects.begin(), _rootGameObjects.end(), targetObject);

		// get iterator for the moved game object
		auto movedGameObjectIterator = std::find(_rootGameObjects.begin(), _rootGameObjects.end(), movedGameObject);

		// use rotate to move the item in front of the target object
		if(targetGameObjectIterator!= _rootGameObjects.end() && movedGameObjectIterator != _rootGameObjects.end())
		{
			if(movedGameObjectIterator < targetGameObjectIterator)
			std::rotate(movedGameObjectIterator, movedGameObjectIterator+1, targetGameObjectIterator+1);

			else if (movedGameObjectIterator > targetGameObjectIterator)
			std::rotate(targetGameObjectIterator, movedGameObjectIterator, movedGameObjectIterator+1);

			// return true if moving successes
			return true;
		}

		// return false if moving fails
		return false;
	}


//--------------------------------------------------------------------------------------------------------------
	// _components controls
	//--------------------------------------------------------------------------------------------------------------

	void Scene::RemoveComponent(Component& component)
	{
		// trigger end of life cycle events
		if (_isStarted)
		{
			// OnDisable specification
			if (component._isEnabledInHierarchy)
			{
				component.OnDisable(*this);
			}
			component.OnEnd(*this);
		}

		if (component._gameObject)
		{
			component._gameObject->RemoveComponent(&component);
		}

		if (!VectorHelpers::RemoveFirst(_components, &component))
		{
			PLATINUM_ERROR("Hierarchy is invalid: _components is missing an element");
			return;
		}

		delete &component;
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

		for (auto gameObject: _rootGameObjects)
		{
			BroadcastOnStart(*gameObject);
		}

		for (auto gameObject: _rootGameObjects)
		{
			BroadcastOnEnable(*gameObject);
		}
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

		for (auto gameObject: _rootGameObjects)
		{
			BroadcastOnDisable(*gameObject);
		}

		for (auto gameObject: _rootGameObjects)
		{
			BroadcastOnEnd(*gameObject);
		}
	}

	void Scene::Update(double deltaTime)
	{
		for (auto gameObject: _rootGameObjects)
		{
			BroadcastOnUpdate(*gameObject, deltaTime);
		}
	}

	void Scene::Render(Renderer& renderer)
	{
		for (auto gameObject: _rootGameObjects)
		{
			BroadcastOnRender(*gameObject, renderer);
		}
	}

	//--------------------------------------------------------------------------------------------------------------
	// External controls
	//--------------------------------------------------------------------------------------------------------------

	void Scene::BroadcastOnStart(GameObject& gameObject)
	{
		for (auto component: gameObject._components)
		{
			component->OnStart(*this);
		}

		for (auto child: gameObject._children)
		{
			BroadcastOnStart(*child);
		}
	}

	void Scene::BroadcastOnEnd(GameObject& gameObject)
	{
		for (auto component: gameObject._components)
		{
			component->OnEnd(*this);
		}

		for (auto child: gameObject._children)
		{
			BroadcastOnEnd(*child);
		}
	}

	void Scene::BroadcastOnEnable(GameObject& gameObject)
	{
		if (!gameObject._isEnabledInHierarchy)
			return;

		for (auto component: gameObject._components)
		{
			if (component->_isEnabledInHierarchy)
				component->OnEnable(*this);
		}

		for (auto child: gameObject._children)
		{
			BroadcastOnEnable(*child);
		}
	}

	void Scene::BroadcastOnDisable(GameObject& gameObject)
	{
		if (!gameObject._isEnabledInHierarchy)
			return;

		for (auto component: gameObject._components)
		{
			if (component->_isEnabledInHierarchy)
				component->OnDisable(*this);
		}

		for (auto child: gameObject._children)
		{
			BroadcastOnDisable(*child);
		}
	}

	void Scene::BroadcastOnUpdate(GameObject& gameObject, double deltaTime)
	{
		if (!gameObject._isEnabledInHierarchy)
			return;

		for (auto component: gameObject._components)
		{
			if (component->_isEnabledInHierarchy)
				component->OnUpdate(*this, deltaTime);
		}

		for (auto child: gameObject._children)
		{
			BroadcastOnUpdate(*child, deltaTime);
		}
	}

	void Scene::BroadcastOnRender(GameObject& gameObject, Renderer& renderer)
	{
		if (!gameObject._isEnabledInHierarchy)
			return;

		for (auto component: gameObject._components)
		{
			if (component->_isEnabledInHierarchy)
				component->OnRender(*this, renderer);
		}

		for (auto child: gameObject._children)
		{
			BroadcastOnRender(*child, renderer);
		}
	}

	void Scene::UpdateIsEnabledInHierarchy(GameObject& gameObject)
	{
		// recursively, broadcast the function UpdateIsEnabledInHierarchy to all it's components and children

		for (auto component: gameObject._components)
		{
			component->UpdateIsEnabledInHierarchy(*this);
		}

		for (auto child: gameObject._children)
		{
			// recurse
			// UpdateIsEnabledInHierarchy could further call UpdateIsEnabledInHierarchy
			child->UpdateIsEnabledInHierarchy(*this);
		}
	}

	Component* Scene::AddComponentInternal(Component& component, GameObject* gameObject, bool isEnabled)
	{
		_components.push_back(&component);
		// basically forcing a constructor on all components, regardless of base calling
		component._gameObject = gameObject;
		component._isEnabled = isEnabled;
		component._isEnabledInHierarchy = component.CalculateIsEnabledInHierarchy();

		if (gameObject)
		{
			gameObject->_components.push_back(&component);
		}

		if (_isStarted)
		{
			component.OnStart(*this);
			if (component._isEnabledInHierarchy)
			{
				component.OnEnable(*this);
			}
		}

		return &component;
	}

	//--------------------------------------------------------------------------------------------------------------
	// Internal controls
	//--------------------------------------------------------------------------------------------------------------

	void Scene::RemoveGameObjectRecurse(GameObject& gameObject)
	{
		// recurse
		for (auto child: gameObject._children)
		{
			RemoveGameObjectRecurse(*child);
		}

		for (auto component: gameObject._components)
		{
			if (!VectorHelpers::RemoveFirst(_components, component))
			{
				PLATINUM_ERROR("Hierarchy is invalid: _components is missing an element");
			}
			delete component;
		}

		if (!VectorHelpers::RemoveFirst(_gameObjects, &gameObject))
		{
			PLATINUM_ERROR("Hierarchy is invalid: _gameObjects is missing an element");
    }

		delete &gameObject;
	}

	void Scene::RemoveRootGameObject(GameObject& rootGameObject)
	{
		if (!VectorHelpers::RemoveFirst(_rootGameObjects, &rootGameObject))
		{
			PLATINUM_ERROR("Hierarchy is invalid: _rootGameObjects is missing an element");
		}
	}

	Component* Scene::FindFirstComponentInternal(
			bool requireEnabled,
			const std::type_info& typeInfo)
	{
		for (auto rootGameObject : _rootGameObjects)
		{
			auto first = FindFirstComponentRecurse(requireEnabled, typeInfo, *rootGameObject);
			if (first)
				return first;
		}
		return nullptr;
	}

	Component* Scene::FindFirstComponentRecurse(
			bool requireEnabled,
			const std::type_info& typeInfo,
			GameObject& gameObject)
	{
		if (requireEnabled && !gameObject._isEnabledInHierarchy)
			return nullptr;

		auto component = gameObject.GetComponentInternal(typeInfo);
		if (component)
			return component;

		for (auto child : gameObject._children)
		{
			auto first = FindFirstComponentRecurse(requireEnabled, typeInfo, *child);
			if (first)
				return first;
		}
		return nullptr;
	}
}