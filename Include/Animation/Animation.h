//
// Created by Shihua on 24/04/2022.
//

#pragma once

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

	class VertexBone{
	public:

		// __(( PARAMETER ))__
		std::vector<unsigned int> trackIDs;
		std::vector<float> weights;

		// __(( FUNCTION ))__
		void AddTrack(unsigned int inTrackID, float inWeight);

		void Clean();

		// __(( CONSTRUCTOR ))__
		VertexBone();

	};

	class Animation
	{
	public:
		// {  PARAMETER  }

		// Timer
		float animationTime = 0.f;

		// OZZ data
		ozz::animation::offline::RawAnimation rawAnimation;
		ozz::unique_ptr<ozz::animation::Animation> animation;
		ozz::animation::SamplingJob::Context context;
		ozz::vector<ozz::math::SoaTransform> localTransformOZZ;
		ozz::vector<ozz::math::Float4x4> worldTransformOZZ;
		std::vector<Maths::Mat4> worldTransform;

		// {  FUNCTION_  }
		void UpdateWorldTransformMatrix(ozz::unique_ptr<ozz::animation::Skeleton>& skeleton, const std::vector<Bone>& bones);

		void UpdateAnimationTime();

		void SetAnimationTime(float newTime);

		void RestartAnimation();

		void StopAnimation();

		void PauseAnimation();

		void ContinueAnimation();

		void LoopAnimation();

		void StopLoopingAnimation();

		bool CheckIfAnimationFinish();

		bool CheckIfAnimationPlaying();

		void PlayAnimationTimer();

		void BuildAnimationRuntimeData();

		void ConvertMatrix();

		// { CONSTRUCTOR }

	private:
		// Flag
		bool _isAnimationLooping = true;
		bool _isLastFrame = false;
		bool _isAnimationPlaying = true;
		bool _isAnimationFinish = false;
	};
}