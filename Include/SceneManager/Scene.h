//
// Created by Shihua on 09/03/2022.
//

#pragma once

#include <vector>
#include <queue>
#include <string>
#include <IDSystem/IDSystem.h>
#include <TypeDatabase/TypeDatabase.h>

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

		static void CreateTypeInfo(TypeDatabase& typeDatabase);

		//--------------------------------------------------------------------------------------------------------------
		// Constructors/destructors
		//--------------------------------------------------------------------------------------------------------------

		explicit Scene(IDSystem& idSystem);

		// Delete all these operators. Messes up data.

		Scene(const Scene& copy) = delete;

		Scene& operator=(const Scene& copy) = delete;

		Scene(Scene&&) noexcept = delete;

		Scene& operator=(Scene&&) noexcept = delete;

		//--------------------------------------------------------------------------------------------------------------
		// Load control
		//--------------------------------------------------------------------------------------------------------------

		void LoadFile(std::string filePath);

		void SaveFile(std::string filePath);

		// Clear all data stored on this scene
		void Clear();

		// Please call this after this object has just been deserialized
		void AfterLoad();

		//--------------------------------------------------------------------------------------------------------------
		// _gameObjects controls
		//--------------------------------------------------------------------------------------------------------------

		SavedReference<GameObject> AddGameObject(
				std::string name = "GameObject",
				SavedReference<GameObject> parent = {},
				bool isEnabled = true);

		void RemoveGameObject(SavedReference<GameObject>& gameObject);

		size_t GetGameObjectsCount() const;

		//--------------------------------------------------------------------------------------------------------------
		// _rootGameObjects controls
		//--------------------------------------------------------------------------------------------------------------

		size_t GetRootGameObjectsCount() const;

		SavedReference<GameObject>& GetRootGameObject(size_t index);

		// Returns boolean to check if moving successes or not
		// targetObject : a target object of which the moved object wants to move in front of
		// movedGameObject : an object that want to be moved
		bool MoveRootGameObjectPositionInList(
				SavedReference<GameObject>& targetObject,
				SavedReference<GameObject>& movedGameObject);


		//--------------------------------------------------------------------------------------------------------------
		// _components controls
		//--------------------------------------------------------------------------------------------------------------

		/**
		 * Adds a component to the scene.
		 * @tparam T
		 * @param gameObject
		 * @param isEnabled
		 * @return
		 */
		template<typename T>
		SavedReference<T> AddComponent(SavedReference<GameObject> gameObject = {}, bool isEnabled = true)
		{
			SavedReference<T> componentReference = idSystem.Add<T>();
			AddComponentInternal(
					(SavedReference<Component>)componentReference,
					gameObject,
					isEnabled);
			return componentReference;
		}

		template<typename T>
		void RemoveComponent(SavedReference<T> component)
		{
			static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
			// we can use UnsafeCast because Component is base of T
			RemoveComponentInternal(std::move(component.template UnsafeCast<Component>()));
		}

		void RemoveComponentInternal(SavedReference<Component> component);

		size_t GetComponentsCount() const;

		template<typename T>
		SavedReference<T> FindFirstComponent(bool requireEnabled = true)
		{
			// cannot use recursive functions because template T needed
			// for dynamic casting

			std::queue<SavedReference<GameObject>*> gameObjectsToFind;
			for (auto& root: _rootGameObjects)
				gameObjectsToFind.push(&root);

			while (!gameObjectsToFind.empty())
			{
				SavedReference<GameObject>* nextGameObject = gameObjectsToFind.front();
				gameObjectsToFind.pop();

				if (!nextGameObject->operator bool())
					continue;

				GameObject* rawPointer = nextGameObject->DeRef().get();
				if (requireEnabled && !rawPointer->_isEnabledInHierarchy)
					continue;

				// this is the dynamic cast
				SavedReference<T> targetComponent = rawPointer->GetComponent<T>();
				if (targetComponent)
				{
					if (!requireEnabled || targetComponent.DeRef()->_isEnabledInHierarchy)
						return targetComponent;
				}

				for (SavedReference<GameObject>& child: nextGameObject->DeRef()->_children)
					gameObjectsToFind.push(&child);
			}

			return {};
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

		/**
		 * Call after IDSystem has been deleted from
		 */
		void OnIDSystemUpdate();

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
		void BroadcastOnStart(SavedReference<GameObject>& gameObject);

		/**
		 * Broadcasts OnEnd event to the input gameObject and all of its' children regardless of enabled/disabled.
		 */
		void BroadcastOnEnd(SavedReference<GameObject>& gameObject);

		/**
		 * Broadcasts OnEnable event to the input gameObject and all of its' children iff *enabled in hierarchy*
		 *
		 * Used for jump starting the scene on Start().
		 */
		void BroadcastOnEnable(SavedReference<GameObject>& gameObject);

		/**
		 * THIS AFFECTS ENABLED OBJECTS ONLY (counter-intuitive)
		 * Broadcasts OnDisable event to the input gameObject and all of its' children iff *enabled in hierarchy*
		 *
		 * Used for cleaning up objects no longer in use on End()/removal.
		 */
		void BroadcastOnDisable(SavedReference<GameObject>& gameObject);

		/**
		 * Broadcasts OnUpdate event to the input gameObject and all of its' children iff "enabled in hierarchy".
		 */
		void BroadcastOnUpdate(SavedReference<GameObject>& gameObject, double deltaTime);

		/**
		 * Broadcasts OnRender event to the input gameObject and all of its' children iff "enabled in hierarchy".
		 */
		void BroadcastOnRender(SavedReference<GameObject>& gameObject, Renderer& renderer);

		/**
		 * Broadcasts UpdateIsEnabledInHierarchy event to the input gameObject and all of its' children
		 * regardless of enabled/disabled.
		 */
		void UpdateIsEnabledInHierarchy(SavedReference<GameObject>& gameObject);

		void BroadcastOnIDSystemUpdate(SavedReference<GameObject>& gameObject);

		void AddComponentInternal(
				SavedReference<Component> component,
				SavedReference<GameObject> gameObject,
				bool isEnabled);

		//--------------------------------------------------------------------------------------------------------------
		// Internal controls
		//--------------------------------------------------------------------------------------------------------------

		void RemoveGameObjectRecurse(SavedReference<GameObject>& gameObject);

		void RemoveRootGameObject(SavedReference<GameObject>& gameObject);
	};
}
