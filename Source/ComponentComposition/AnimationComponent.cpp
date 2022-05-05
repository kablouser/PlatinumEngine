//
// Created by Shihua on 01/05/2022.
//

#include <ComponentComposition/AnimationComponent.h>
#include <ComponentComposition/MeshRender.h>

namespace PlatinumEngine
{
	void AnimationComponent::CreateTypeInfo(TypeDatabase& typeDatabase)
	{

		typeDatabase.BeginTypeInfo<AnimationComponent>()
					.WithInherit<Component>()
					.WithField<std::vector<Maths::Mat4>>("worldTransform", PLATINUM_OFFSETOF(AnimationComponent,worldTransform))
					.WithField<bool>("isDisplay", PLATINUM_OFFSETOF(AnimationComponent,isDisplay))
					.WithField<ozz::unique_ptr<ozz::animation::Animation>>("animations", PLATINUM_OFFSETOF(AnimationComponent, animations))
					.WithField<AnimationLocalTimer>("_timer", PLATINUM_OFFSETOF(AnimationComponent, _timer))
					.WithField<unsigned int>("_selectedAnimationIndex", PLATINUM_OFFSETOF(AnimationComponent, _selectedAnimationIndex));
	}

	AnimationComponent::AnimationComponent(): isDisplay(false), _selectedAnimationIndex(0)
	{
	}

	void AnimationComponent::AddAnimation(ozz::unique_ptr<ozz::animation::Animation>& animation)
	{
		SavedReference<Mesh> currentMesh = GetGameObject().DeRef()->GetComponent<MeshRender>().DeRef()->GetMesh();

		if(currentMesh.DeRef() == nullptr)
			return;

		if(animation->num_tracks() != currentMesh.DeRef()->skeleton->num_joints())
			return;

		for(ozz::unique_ptr<ozz::animation::Animation>& a : animations)
		{
			if(a == animation)
				return;
		}

		animations.push_back(std::move(animation));
	}



	void AnimationComponent::RemoveAnimation(unsigned int index)
	{
		if(index < animations.size())
		{
			animations.erase(animations.begin() + index);
			if (_selectedAnimationIndex == index)
				_selectedAnimationIndex = 0;
			else if (_selectedAnimationIndex > index)
				--_selectedAnimationIndex;
		}
	}



	void AnimationComponent::UpdateWorldTransformMatrix(
			ozz::unique_ptr<ozz::animation::Skeleton>& skeleton,
			const std::vector<Bone>& bones
	)
	{
		if(animations.size() <= _selectedAnimationIndex || skeleton == nullptr)
			return;

		_timer.PlayAnimationTimer();

		ozz::vector<ozz::math::SoaTransform> localTransformOZZ;
		ozz::vector<ozz::math::Float4x4> worldTransformOZZ;
		ozz::animation::SamplingJob::Context context;

		context.Resize(skeleton->num_joints());
		localTransformOZZ.resize(skeleton->num_joints());
		worldTransformOZZ.resize(skeleton->num_joints());

		// Samples runtime animation data
		ozz::animation::SamplingJob samplingJob;

		samplingJob.animation = animations[_selectedAnimationIndex].get();
		samplingJob.context = &context;
		samplingJob.ratio = _timer.GetAnimationTime()/animations[_selectedAnimationIndex]->duration();
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
		worldTransform.clear();

		for(ozz::math::Float4x4 m : worldTransformOZZ)
		{
			auto matrix = Maths::Mat4({
					m.cols[0].x, m.cols[0].y, m.cols[0].z, m.cols[0].w,
					m.cols[1].x, m.cols[1].y, m.cols[1].z, m.cols[1].w,
					m.cols[2].x, m.cols[2].y, m.cols[2].z, m.cols[2].w,
					m.cols[3].x, m.cols[3].y, m.cols[3].z, m.cols[3].w
			});
			worldTransform.push_back(matrix);
		}

		// calculate the final transformation matrices for every vertex
		for(auto & bone : bones)
		{
			worldTransform[bone.trackID] = worldTransform[bone.trackID] * bone.offsetMatrix;
		}
	}

	void AnimationComponent::SetCurrentAnimationByID(unsigned int inID)
	{
		if (inID < animations.size())
		{
			_selectedAnimationIndex = inID;
			_timer.SetAnimationDuration(animations[_selectedAnimationIndex]->duration());
		}
	}

	unsigned int AnimationComponent::GetCurrentAnimationID() const
	{
		return _selectedAnimationIndex;
	}

	unsigned int AnimationComponent::GetAmountOfAnimations() const
	{
		return animations.size();
	}

	std::string AnimationComponent::GetAnimationName(unsigned int inID) const
	{
		if(animations.size() > inID)
			return animations[_selectedAnimationIndex]->name();
		else
		{
			PLATINUM_WARNING("Animation ID exceed the total amount of animations.");
			return "";
		}
	}

}


