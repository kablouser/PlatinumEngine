//
// Created by Shihua on 04/05/2022.
//

#pragma once

namespace PlatinumEngine
{
	class AnimationLocalTimer
	{
	public:

		// {{{ Function }}}
		void SetAnimationDuration(float inDuration);


		/**
		 * Set animation time to be a new time. (the input time will be clamped into the duration)
		 * @param newTime : new time
		 */
		void SetAnimationTime(float newTime);

		/**
		 * restart the animation. set animation time to be 0.f
		 */
		void RestartAnimation();

		/**
		 * stop the animation and set animation time back to be 0.f
		 */
		void StopAnimation();

		/**
		 * pause the animation. the animation time remain unchanged
		 */
		void PauseAnimation();

		/**
		 * Make the frozen animation move again.
		 * The timer will not be changed.
		 */
		void ContinueAnimation();

		/**
		 * Set the display mode to be looping
		 */
		void LoopAnimation();

		/**
		 * Set the display mode to not looping
		 */
		void StopLoopingAnimation();

		/**
		 * Check if the animation reach the end
		 * @return : true  -> the animation finishes playing;
		 * 			 false -> the animation is still playing.
		 */
		[[nodiscard]] bool CheckIfAnimationFinish() const;

		/**
		 * Check if the animation is playing
		 * @return :  true -> the animation is playing
		 * 			  false -> the animation is finished, or it is stopped, or it is paused
		 */
		[[nodiscard]] bool CheckIfAnimationPlaying() const;

		/**
		 * This function runs the animation timers based on the flags.
		 * It will keep looping is _isAnimationLooping == true
		 * It will only displays once if _isAnimationLooping == false
		 */
		void PlayAnimationTimer();

		[[nodiscard]] float GetAnimationTime() const;


		// {{{ CONSTRUCTOR　}}}
		AnimationLocalTimer();

	private:
		// {{{ Parameter }}}
		// Timer
		float _animationTime = 0.f;
		float _duration = 0.f;
		// Flag
		bool _isAnimationLooping = true;
		bool _isLastFrame = false;
		bool _isAnimationPlaying = true;
		bool _isAnimationFinish = false;


		// {{{ Function }}}
		/**
		 * Keep looping the animation time
		 */
		void UpdateAnimationTime();

	};

}