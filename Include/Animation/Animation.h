//
// Created by Shihua on 24/04/2022.
//

#pragma once

// Unclassified
#include <vector>
#include <assimp/scene.h>
#include <utility>

// PLATINUM Library
#include <OpenGL/Mesh.h>
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
		void SetTranformMatrixByaiMatrix(aiMatrix4x4 aiMatrix)
		{
			offsetMatrix = Maths::Mat4({
					aiMatrix[0][0],aiMatrix[1][0],aiMatrix[2][0],aiMatrix[3][0],
					aiMatrix[0][1],aiMatrix[1][1],aiMatrix[2][1],aiMatrix[3][1],
					aiMatrix[0][2],aiMatrix[1][2],aiMatrix[2][2],aiMatrix[3][2],
					aiMatrix[0][3],aiMatrix[1][3],aiMatrix[2][3],aiMatrix[3][3]});
		}

		// __(( CONSTRUCTOR ))__
		Bone(): trackID(0), offsetMatrix(1.f)
		{}
	};

	class VertexBone{
	public:

		// __(( PARAMETER ))__
		std::vector<unsigned int> trackIDs;
		std::vector<float> weights;

		// __(( FUNCTION ))__
		void AddTrack(unsigned int inTrackID, float inWeight)
		{
			for(unsigned int i =0 ; i< 4; i++)
			{
				if(weights[i] == 0.f)
				{
					trackIDs[i] = inTrackID;
					weights[i] = inWeight;
				}
			}
		}

		void Clean()
		{
			trackIDs.clear();
			weights.clear();
		}

		// __(( CONSTRUCTOR ))__
		VertexBone()
		{
			trackIDs.resize(4, 0);
			weights.resize(4, 0.f);
		}

	};

	class Animation
	{
	public:
		// {  PARAMETER  }
		std::string animationName;

		// Flag
		bool isAnimationExist = false;
		float animationTime = 0.f;

		// OZZ data
		ozz::animation::offline::RawAnimation rawAnimation;
		ozz::unique_ptr<ozz::animation::Animation> animation;
		ozz::animation::SamplingJob::Context context;
		ozz::vector<ozz::math::SoaTransform> localTransformOZZ;
		ozz::vector<ozz::math::Float4x4> worldTransformOZZ;
		std::vector<Maths::Mat4> worldTransform;

		// {  FUNCTION_  }
		void UpdateWorldTransformMatrix(ozz::unique_ptr<ozz::animation::Skeleton>& skeleton, const std::vector<Bone>& bones)
		{
			if(animation == nullptr || skeleton == nullptr)
				return;

			// Samples runtime animation data
			ozz::animation::SamplingJob samplingJob;
			samplingJob.animation = animation.get();
			samplingJob.context = &context;
			samplingJob.ratio = animationTime/animation->duration();
			samplingJob.output = make_span(localTransformOZZ);

			// sample animation
			if (!samplingJob.Run())
			{
				return;
			}

			// get world transformation matrices for all nodes with the order of nodes in the animation track/ channel
			ozz::animation::LocalToModelJob localToModelJob;
			localToModelJob.skeleton = skeleton.get();
			localToModelJob.input = make_span(localTransformOZZ);
			localToModelJob.output = make_span(worldTransformOZZ);

			if (!localToModelJob.Run())
			{
				return;
			}

			// convert the matrix from ozz::math::Float4x4 to Maths::Mat4
			ConvertMatrix();

			// calculate the final transformation matrices for every vertex
			for(auto & bone : bones)
			{
				worldTransform[bone.trackID] = worldTransform[bone.trackID] * bone.offsetMatrix;
			}
		}

		void UpdateAnimationTime()
		{
			animationTime += 0.1f;

			if(animation == nullptr)
				return;

			if(animationTime > animation->duration())
				animationTime = animationTime - std::floor(animationTime/animation->duration())*animation->duration();

		}

		// { CONSTRUCTOR }


		// {  FUNCTION_  }
		void BuildAnimationRuntimeData()
		{
			ozz::animation::offline::AnimationBuilder animationBuilder;
			if(!rawAnimation.Validate())
				std::cout << "raw animation invalid"<<std::endl;

			animation = animationBuilder(rawAnimation);

			// Resize based on the number of joints
			context.Resize(animation->num_tracks());
			localTransformOZZ.resize(animation->num_tracks());
			worldTransformOZZ.resize(animation->num_tracks());
		}


		void ConvertMatrix()
		{

			worldTransform.clear();

			for(ozz::math::Float4x4 m : worldTransformOZZ)
			{
				worldTransform.push_back(Maths::Mat4({
						m.cols[0][0], m.cols[0][1], m.cols[0][2], m.cols[0][3],
						m.cols[1][0], m.cols[1][1], m.cols[1][2], m.cols[1][3],
						m.cols[2][0], m.cols[2][1], m.cols[2][2], m.cols[2][3],
						m.cols[3][0], m.cols[3][1], m.cols[3][2], m.cols[3][3],
				}));
			}
		}

		// { CONSTRUCTOR }


	};
}