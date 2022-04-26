//
// Created by Shihua on 09/03/2022.
//

#pragma once

#include <vector>
#include <string>
#include <IDSystem/IDSystem.h>

#include <ComponentComposition/GameObject.h>

namespace PlatinumEngine
{
	class GameObject;

	class Component;

	class Renderer;

	class Scene
	{
		friend class GameObject;

	public:

		IDSystem& idSystem;

		//--------------------------------------------------------------------------------------------------------------
		// Constructors/destructors
		//--------------------------------------------------------------------------------------------------------------

		explicit Scene(IDSystem& idSystem);

		~Scene();

		// Delete all these operators. Messes up data.

		Scene(const Scene& copy) = delete;

		Scene& operator=(const Scene& copy) = delete;

		Scene(Scene&&) noexcept = delete;

		Scene& operator=(Scene&&) noexcept = delete;

		//--------------------------------------------------------------------------------------------------------------
		// _gameObjects controls
		//--------------------------------------------------------------------------------------------------------------

		SavedReference<GameObject> AddGameObject(
				std::string name = "GameObject",
				SavedReference<GameObject> parent = {},
				bool isEnabled = true);

		void RemoveGameObject(SavedReference<GameObject> gameObject);

		size_t GetGameObjectsCount() const;

		//--------------------------------------------------------------------------------------------------------------
		// _rootGameObjects controls
		//--------------------------------------------------------------------------------------------------------------

		size_t GetRootGameObjectsCount() const;

		SavedReference<GameObject>& GetRootGameObject(size_t index);

		// Returns boolean to check if moving successes or not
		// targetObject : a target object of which the moved object wants to move in front of
		// movedGameObject : an object that want to be moved
		bool MoveRootGameObjectPositionInList(GameObject* targetObject, GameObject* movedGameObject);


		//--------------------------------------------------------------------------------------------------------------
		// _components controls
		//--------------------------------------------------------------------------------------------------------------

		/**
		 * Adds a component to the scene
		 * If a game object is provided, then first checks if component exists.
		 * 	If it does, returns this instead of creating a new one
		 */
		template<typename T>
		T* AddComponent(GameObject* gameObject = nullptr, bool isEnabled = true)
		{
			if (gameObject)
				if (gameObject->GetComponent<T>())
					return gameObject->GetComponent<T>();
			return static_cast<T*>(AddComponentInternal(*static_cast<Component*>(new T()), gameObject, isEnabled));
		}

		void RemoveComponent(Component& component);

		size_t GetComponentsCount() const;

		template<typename T>
		T* FindFirstComponent(bool requireEnabled = true)
		{
			return dynamic_cast<T*>(FindFirstComponentInternal(requireEnabled, typeid(T)));
		}

		//--------------------------------------------------------------------------------------------------------------
		// Event controls
		//--------------------------------------------------------------------------------------------------------------

		/**
		 * true iff Start() has been called and End() has yet to be called.
		 */
		bool IsStarted() const;

		/**
		 * Please call when the game starts. And call only once before the game ends.
		 */
		void Start();

		/**
		 * Please call when the game ends. And call only once before the game starts again.
		 */
		void End();

		/**
		 * Please call when you want to update all components in the scene.
		 * Respects hierarchy order and objects that are "enabled in hierarchy".
		 * @param deltaTime time since last Update call
		 */
		void Update(double deltaTime);

		/**
		 * Please call when you want to render the scene.
		 * Respects hierarchy order and objects that are "enabled in hierarchy".
		 * @param renderer target location for rendering
		 */
		void Render(Renderer& renderer);

	private:

		bool _isStarted;
		std::vector<SavedReference<GameObject>> _gameObjects;
		std::vector<SavedReference<GameObject>> _rootGameObjects;
		std::vector<SavedReference<Component>> _components;

		//--------------------------------------------------------------------------------------------------------------
		// External controls
		//--------------------------------------------------------------------------------------------------------------

		/**
		 * Broadcasts OnStart event to the input gameObject and all of its' children regardless of enabled/disabled.
		 */
		void BroadcastOnStart(GameObject& gameObject);

		/**
		 * Broadcasts OnEnd event to the input gameObject and all of its' children regardless of enabled/disabled.
		 */
		void BroadcastOnEnd(GameObject& gameObject);

		/**
		 * Broadcasts OnEnable event to the input gameObject and all of its' children iff *enabled in hierarchy*
		 *
		 * Used for jump starting the scene on Start().
		 */
		void BroadcastOnEnable(GameObject& gameObject);

		/**
		 * THIS AFFECTS ENABLED OBJECTS ONLY (counter-intuitive)
		 * Broadcasts OnDisable event to the input gameObject and all of its' children iff *enabled in hierarchy*
		 *
		 * Used for cleaning up objects no longer in use on End()/removal.
		 */
		void BroadcastOnDisable(GameObject& gameObject);

		/**
		 * Broadcasts OnUpdate event to the input gameObject and all of its' children iff "enabled in hierarchy".
		 */
		void BroadcastOnUpdate(GameObject& gameObject, double deltaTime);

		/**
		 * Broadcasts OnRender event to the input gameObject and all of its' children iff "enabled in hierarchy".
		 */
		void BroadcastOnRender(GameObject& gameObject, Renderer& renderer);

		/**
		 * Broadcasts UpdateIsEnabledInHierarchy event to the input gameObject and all of its' children
		 * regardless of enabled/disabled.
		 */
		void UpdateIsEnabledInHierarchy(GameObject& gameObject);

		Component* AddComponentInternal(Component& component, GameObject* gameObject, bool isEnabled);

		//--------------------------------------------------------------------------------------------------------------
		// Internal controls
		//--------------------------------------------------------------------------------------------------------------

		void RemoveGameObjectRecurse(GameObject& gameObject);

		void RemoveRootGameObject(GameObject& gameObject);

		Component* FindFirstComponentInternal(
				bool requireEnabled,
				const std::type_info& typeInfo);
		Component* FindFirstComponentRecurse(
				bool requireEnabled,
				const std::type_info& typeInfo,
				GameObject& gameObject);
	};
}
