//
// Created by Shihua on 04/05/2022.
//

#pragma once
#include <TypeDatabase/TypeDatabase.h>
#include <Helpers/Time.h>

namespace PlatinumEngine
{
	class AnimationLocalTimer
	{
	public:

		// {{{ Function }}}
		static void CreateTypeInfo(TypeDatabase& typeDatabase);

		/**
		 * restart the animation. set isAnimationPlaying=true and set animationTime to be 0.f
		 */
		void RestartAnimation();

		/**
		 * pause the animation and set animation time back to be 0.f
		 */
		void StopAnimation();

		/**
		 * Check if the animation has finished playing while isAnimationPlaying == true.
		 * @return : true  -> the animation is non-looping and animationTime is past the duration
		 * 			 false -> otherwise
		 */
		[[nodiscard]] bool IsAnimationFinish(float animationDuration) const;

		/**
		 * This function runs the animation timers based on the flags.
		 * It will keep looping if isAnimationLooping
		 * It will only displays once if isAnimationLooping == false
		 */
		void Update(Time& time, float animationDuration);

		// {{{ CONSTRUCTOR　}}}
		AnimationLocalTimer();

		// {{{ Parameter }}}
		// Timer
		float animationTime = 0.f;
		// Flag
		bool isAnimationLooping = true;
		bool isAnimationPlaying = true;

	private:
		int _lastFrame = -1;
	};

}