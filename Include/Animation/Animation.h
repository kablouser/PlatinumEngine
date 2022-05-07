//
// Created by Shihua on 24/04/2022.
//

#pragma once
#define OZZ_BUILD_SIMD_REF
// Unclassified
#include <vector>
#include <assimp/scene.h>
#include <utility>

// PLATINUM Library
#include <Maths/Matrices.h>
#include <Logger/Logger.h>


//OZZ Library
//DATA STRUCTURE
#include "ozz/animation/runtime/animation.h"
#include "ozz/animation/runtime/skeleton.h"
#include "ozz/animation/offline/raw_animation.h"
#include "ozz/animation/offline/raw_skeleton.h"
//JOB
#include "ozz/animation/runtime/local_to_model_job.h"
#include "ozz/animation/runtime/sampling_job.h"
//BUILDER
#include "ozz/animation/offline/skeleton_builder.h"
#include "ozz/animation/offline/animation_builder.h"

//OZZ Library
//MATH
#include "ozz/base/maths/simd_math.h"
#include "ozz/base/maths/soa_transform.h"
#include "ozz/base/maths/vec_float.h"

namespace PlatinumEngine
{

	class Bone{
	public:

		// __(( PARAMETER ))__
		std::string boneName;
		unsigned int trackID;
		Maths::Mat4 offsetMatrix;

		// __(( FUNCTION ))__
		void SetTranformMatrixByaiMatrix(aiMatrix4x4 aiMatrix);

		// __(( CONSTRUCTOR ))__
		Bone();

	};

	class Animation
	{
	public:
		// {  PARAMETER  }

		// OZZ data
		ozz::unique_ptr<ozz::animation::Animation> animation;

		// TODO transfer to AnimationComponent or somewhere else
		ozz::vector<ozz::math::SoaTransform> localTransformOZZ;
		ozz::vector<ozz::math::Float4x4> worldTransformOZZ;
		std::vector<Maths::Mat4> worldTransform;

		// {  FUNCTION_  }

		/**
		 * Calculate the animation matrices for current frame. the timer is controlled by other functions
		 * @param skeleton
		 * @param bones
		 */
		void UpdateWorldTransformMatrix(
				ozz::unique_ptr<ozz::animation::Skeleton>& skeleton,
				const std::vector<Bone>& bones,
				ozz::animation::SamplingJob::Context& context);

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
		bool CheckIfAnimationFinish();

		/**
		 * Check if the animation is playing
		 * @return :  true -> the animation is playing
		 * 			  false -> the animation is finished, or it is stopped, or it is paused
		 */
		bool CheckIfAnimationPlaying();

		/**
		 * This function runs the animation timers based on the flags.
		 * It will keep looping is _isAnimationLooping == true
		 * It will only displays once if _isAnimationLooping == false
		 */
		void PlayAnimationTimer();



		// { CONSTRUCTOR }

	private:
		// Timer
		float _animationTime = 0.f;
		// Flag
		bool _isAnimationLooping = true;
		bool _isLastFrame = false;
		bool _isAnimationPlaying = true;
		bool _isAnimationFinish = false;

		/**
		 * Keep looping the animation time
		 */
		void UpdateAnimationTime();

		/**
		 * Convert Matrices from ozz::math::Float4x4 to Maths::Mat4
		 */
		void ConvertMatrix();
	};
}