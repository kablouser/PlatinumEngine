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
	//--------------------------------------------------------------------------------------------------------------
	// Constructors/destructors
	//--------------------------------------------------------------------------------------------------------------

	Scene::Scene() : _isStarted(false)
	{
	}

	Scene::~Scene()
	{
		// we completely manage and own this memory
		// it's safe to delete pointers directly
		for (auto gameObject: _gameObjects)
		{
			delete gameObject;
		}
		for (auto component: _components)
		{
			delete component;
		}
	}

	//--------------------------------------------------------------------------------------------------------------
	// _gameObjects controls
	//--------------------------------------------------------------------------------------------------------------

	GameObject* Scene::AddGameObject(std::string name, GameObject* parent, bool isEnabled)
	{
		GameObject* gameObject = new GameObject(name, parent, isEnabled);

		if (parent == nullptr)
			_rootGameObjects.push_back(gameObject);
		else
			parent->_children.push_back(gameObject);

		_gameObjects.push_back(gameObject);
		return gameObject;
	}

	void Scene::RemoveGameObject(GameObject& gameObject)
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

	GameObject* Scene::GetRootGameObject(size_t index)
	{
		// range checked indexing
		return _rootGameObjects.at(index);
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
			component.OnEnd();
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
			component->OnStart();
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
			component->OnEnd();
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
			component.OnStart();
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
}