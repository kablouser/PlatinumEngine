#pragma once

namespace PlatinumEngine
{

	/**
	 * Compositional thing that can be attached to GameObjects and updated during the game loop.
	 */
	class Component
	{
	public:
		/**
		 * Default constructor. Sets component to enabled.
		 */
		Component();

		explicit Component(bool isEnabled);


		bool IsEnabled() const;

		/**
		 * Sets _isEnabled state and triggers events (OnEnable, OnDisable)
		 * @param isEnabled is component receiving updates
		 * @param areEventsTriggered are relevant triggers upon state changes
		 */
		void SetEnabled(bool isEnabled, bool areEventsTriggered = true);


		/**
		 * Gets the GameObject this component is attached to. Could be null.
		 */
		// GameObject* GetGameObject() const;

		/**
		 * Event triggered when this component is added to the scene while the game is running.
		 * If this component is in the scene before the game starts, it's triggered before the 1st frame of the game.
		 * If this component is added in the middle of the game, it's triggered as soon as it's added.
		 */
		virtual void OnStart();

		/**
		 * Event triggered when this component is *recycled* while the game is running.
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
		 * If this component is already disabled before the start of the game, it's triggered before the 1st frame of the game.
		 */
		virtual void OnDisable();

		/**
		 * Event triggered every frame while this component is enabled and the game is running.
		 */
		virtual void OnUpdate(float deltaTime);


		/**
		 * Event triggered every render frame. Could render when the game is not running (i.e. in the scene editor).
		 */
		// virtual void OnRender(Renderer& renderer)


	private:
		bool _isEnabled;
		// GameObject* gameObject;

	};
}