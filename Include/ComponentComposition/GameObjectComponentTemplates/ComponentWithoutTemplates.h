#pragma once

/*
 * This files contains the forward declarations for Component class
 * But no implementations for the templated functions.
 */

#include <IDSystem/IDSystem.h>

namespace PlatinumEngine
{
	class GameObject;

	/**
	 * Compositional thing that can be attached to GameObjects and updated during the game loop.
	 */
	class Component
	{
		friend class Scene;

	public:

		static void CreateTypeInfo(TypeDatabase& typeDatabase);

		//--------------------------------------------------------------------------------------------------------------
		// Constructors/destructors
		//--------------------------------------------------------------------------------------------------------------

		/**
		 * Default constructor. Sets component to enabled.
		 */
		Component();

		/**
		 * When this is called, the hierarchy is not guaranteed to be valid.
		 * Only deal with this component object in isolation.
		 */
		virtual ~Component();

		//--------------------------------------------------------------------------------------------------------------
		// _isEnabled controls
		//--------------------------------------------------------------------------------------------------------------

		/**
		 * @return true or false, regardless of hierarchy
		 */
		bool IsEnabled() const;

		/**
		 * Sets _isEnabled state and triggers events (OnEnable, OnDisable)
		 * @param isEnabled is component receiving updates
		 * @param scene scene object that this component is running inside
		 */
		void SetEnabled(bool isEnabled);

		/**
		 * @return true iff this and the gameObject it's attached and all of the gameObject's parents are enabled.
		 * false when this is not attached to a gameObject.
		 */
		bool IsEnabledInHierarchy() const;

		//--------------------------------------------------------------------------------------------------------------
		// _gameObject controls
		//--------------------------------------------------------------------------------------------------------------

		/**
		 * Gets the GameObject this component is attached to. Could be null.
		 */
		SavedReference<GameObject>& GetGameObject();

		void SetGameObject(SavedReference<GameObject> gameObject, Scene& scene);

		//--------------------------------------------------------------------------------------------------------------
		// Events
		//--------------------------------------------------------------------------------------------------------------

		/**
		 * Event triggered when this component is added to the scene while the game is running.
		 * If this component is in the scene before the game starts, it's triggered before the 1st frame of the game.
		 * If this component is added in the middle of the game, it's triggered as soon as it's added.
		 */
		virtual void OnStart();

		/**
		 * Event triggered when this component is removed while the game is running.
		 * If this component is removed from the scene in the middle of the game, it's triggered as soon as it's removed.
		 * If this component is in the scene when the game ends, it's triggered after the final frame of the game.
		 */
		virtual void OnEnd();

		/**
		 * Event triggered when _isEnabled state has changed to true while the game is running.
		 * If this component is already enabled before the start of the game, it's triggered before the 1st frame of the game.
		 */
		virtual void OnEnable();

		/**
		 * Event triggered when _isEnabled state has changed to false while the game is running.
		 * If this component is enabled when the game ends, it's triggered after the final frame of the game.
		 */
		virtual void OnDisable();

		/**
		 * Event triggered every frame while this component is enabled and the game is running.
		 */
		virtual void OnUpdate();

		/**
		 * Event triggered every render frame. Could render when the game is not running (i.e. in the scene editor).
		 */
		virtual void OnRender();

		/**
		 * Event triggered when SavedReference's pointers are updated.
		 */
		virtual void OnIDSystemUpdate();

		//--------------------------------------------------------------------------------------------------------------
		// Convenience functions - forward declare only
		//--------------------------------------------------------------------------------------------------------------

		template<class T>
		SavedReference<T> GetComponent();

		template<class T>
		SavedReference<T> GetParentComponent();

	private:

		SavedReference<GameObject> _gameObject;
		bool _isEnabled;
		bool _isEnabledInHierarchy;

		//--------------------------------------------------------------------------------------------------------------
		// Internal controls
		//--------------------------------------------------------------------------------------------------------------

		/* Calculates whether the current value of IsEnabledInHierarchy using this parent
		 * relies on parent being in a valid state
		 */
		bool CalculateIsEnabledInHierarchy() const;

		/* Must be called everytime IsEnabledInHierarchy might be changed
		 * IsEnabledInHierarchy might change if _gameObject or _isEnabled is changed
		 * Uses scene to broadcast events when there's a change
		 */
		void UpdateIsEnabledInHierarchy();
	};
}