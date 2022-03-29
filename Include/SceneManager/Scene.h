//
// Created by Shihua on 09/03/2022.
//

#pragma once

#include <vector>
#include <string>

namespace PlatinumEngine
{
	class GameObject;

	class Component;

	class Renderer;

	class Scene
	{
		friend class GameObject;

	public:
		//--------------------------------------------------------------------------------------------------------------
		// Constructors/destructors
		//--------------------------------------------------------------------------------------------------------------

		Scene();

		~Scene();

		// Delete all these operators. Messes up data.

		Scene(const Scene& copy) = delete;

		Scene& operator=(const Scene& copy) = delete;

		Scene(Scene&&) noexcept = delete;

		Scene& operator=(Scene&&) noexcept = delete;

		//--------------------------------------------------------------------------------------------------------------
		// _gameObjects controls
		//--------------------------------------------------------------------------------------------------------------

		GameObject* AddGameObject(std::string name = "GameObject", GameObject* parent = nullptr, bool isEnabled = true);

		void RemoveGameObject(GameObject& gameObject);

		size_t GetGameObjectsCount() const;

		//--------------------------------------------------------------------------------------------------------------
		// _rootGameObjects controls
		//--------------------------------------------------------------------------------------------------------------

		size_t GetRootGameObjectsCount() const;

		GameObject* GetRootGameObject(size_t index);

		//--------------------------------------------------------------------------------------------------------------
		// _components controls
		//--------------------------------------------------------------------------------------------------------------

		template<typename T>
		T* AddComponent(GameObject* gameObject = nullptr, bool isEnabled = true)
		{
			return static_cast<T*>(AddComponentInternal(*new T(), gameObject, isEnabled));
		}

		void RemoveComponent(Component& component);

		size_t GetComponentsCount() const;

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
		std::vector<GameObject*> _gameObjects;
		std::vector<GameObject*> _rootGameObjects;
		std::vector<Component*> _components;

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
	};
}
