//
// Created by Shihua on 24/04/2022.
//

#pragma once

#include <OpenGL/Mesh.h>
#include <utility>
#include <Logger/Logger.h>
#include <Animation/Animation.h>

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
	class Animation
	{
	public:
		// {  PARAMETER  }
		// Flag
		bool isAnimationOn = false;

		float animationTime = 0.f;
		// Storing connection between vertices and bones/nodes
		std::vector<AnimationVertex> animationVertex;
		//std::vector<VertexBone> verticesBones;
		std::vector<Bone> bones;
		// Mapping bones' name and id
		std::map<std::string, unsigned int> boneMapping;
		// OZZ data
		ozz::animation::offline::RawSkeleton rawSkeleton;
		ozz::animation::offline::RawAnimation rawAnimation;
		ozz::unique_ptr<ozz::animation::Skeleton> skeleton;
		ozz::unique_ptr<ozz::animation::Animation> animation;
		ozz::animation::SamplingJob::Context context;
		ozz::vector<ozz::math::SoaTransform> localTransformOZZ;
		ozz::vector<ozz::math::Float4x4> worldTransformOZZ;
		std::vector<Maths::Mat4> worldTransform;



		// {  FUNCTION_  }
		void UpdateWorldTransformMatrix()
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
		}

		void BuildSkeletonRuntimeData()
		{

			ozz::animation::offline::SkeletonBuilder skeletonBuilder;

			skeleton = skeletonBuilder(rawSkeleton);

			// Resize based on the number of joints
			context.Resize(skeleton->num_joints());
			localTransformOZZ.resize(skeleton->num_joints());
			worldTransformOZZ.resize(skeleton->num_joints());

			for(auto n: skeleton->joint_names())
				std::cout<<n<<std::endl;

			std::cout<<"--------------------"<<std::endl;
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