//
// Created by Shihua on 24/04/2022.
//

// PLATINUM Library
#include <Animation/Animation.h>

namespace PlatinumEngine
{

	void Bone::SetTranformMatrixByaiMatrix(aiMatrix4x4 aiMatrix)
	{
		offsetMatrix = Maths::Mat4({
				aiMatrix[0][0], aiMatrix[1][0], aiMatrix[2][0], aiMatrix[3][0],
				aiMatrix[0][1], aiMatrix[1][1], aiMatrix[2][1], aiMatrix[3][1],
				aiMatrix[0][2], aiMatrix[1][2], aiMatrix[2][2], aiMatrix[3][2],
				aiMatrix[0][3], aiMatrix[1][3], aiMatrix[2][3], aiMatrix[3][3] });
	}

	Bone::Bone() : trackID(0), offsetMatrix(1.f){}
}


namespace PlatinumEngine
{

	void VertexBone::AddTrack(unsigned int inTrackID, float inWeight)
	{
		for (unsigned int i = 0; i < 4; i++)
		{
			if (weights[i] == 0.f)
			{
				trackIDs[i] = inTrackID;
				weights[i] = inWeight;
			}
		}
	}

	void VertexBone::Clean()
	{
		trackIDs.clear();
		weights.clear();
	}

	VertexBone::VertexBone()
	{
		trackIDs.resize(4, 0);
		weights.resize(4, 0.f);
	}
}


namespace PlatinumEngine
{
	void Animation::UpdateWorldTransformMatrix(ozz::unique_ptr<ozz::animation::Skeleton>& skeleton, const std::vector<Bone>& bones)
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

	void Animation::UpdateAnimationTime()
	{

		if(animation == nullptr)
			return;

		animationTime += 0.1f;

		if(animationTime >= animation->duration())
		{
			animationTime = 0.f;

			_isLastFrame = true;
		}
		else
		{
			_isLastFrame = false;
		}
	}

	void Animation::SetAnimationTime(float newTime)
	{
		animationTime = newTime;
	}

	void Animation::RestartAnimation()
	{
		animationTime = 0.f;
		_isLastFrame = false;
		_isAnimationPlaying = true;
	}

	void Animation::StopAnimation()
	{
		animationTime = 0.f;
		_isLastFrame = false;
		_isAnimationPlaying = false;
	}

	void Animation::PauseAnimation()
	{
		_isAnimationPlaying = false;
	}

	void Animation::ContinueAnimation()
	{
		_isAnimationPlaying = true;
	}

	void Animation::LoopAnimation()
	{
		_isAnimationLooping = true;
	}

	void Animation::StopLoopingAnimation()
	{
		_isAnimationLooping = false;
	}

	bool Animation::CheckIfAnimationFinish()
	{
		return _isAnimationFinish;
	}

	bool Animation::CheckIfAnimationPlaying()
	{
		return _isAnimationPlaying;
	}

	void Animation::PlayAnimationTimer()
	{
		if(_isAnimationPlaying && !_isAnimationFinish)
			UpdateAnimationTime();

		if(_isLastFrame && !_isAnimationLooping)
			_isAnimationFinish = true;
		else
			_isAnimationFinish = false;
	}

	void Animation::BuildAnimationRuntimeData()
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


	void Animation::ConvertMatrix()
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

}