//
// Created by Shihua on 01/05/2022.
//

#include <ComponentComposition/AnimationComponent.h>
#include <SceneManager/Scene.h>
#include <Application.h>

namespace PlatinumEngine
{
	void AnimationComponent::CreateTypeInfo(TypeDatabase& typeDatabase)
	{

		typeDatabase.BeginTypeInfo<AnimationComponent>()
					.WithInherit<Component>()
					.WithField<bool>("_isDisplay", PLATINUM_OFFSETOF(AnimationComponent,_isDisplay))
					.WithField<AnimationLocalTimer>("timer", PLATINUM_OFFSETOF(AnimationComponent, timer))
					.WithField<unsigned int>("_selectedAnimationIndex", PLATINUM_OFFSETOF(AnimationComponent, _selectedAnimationIndex))
					.WithField<SavedReference<Mesh>>("_mesh", PLATINUM_OFFSETOF(AnimationComponent, _mesh));
	}

	AnimationComponent::AnimationComponent(): _isDisplay(false), _selectedAnimationIndex(0)
	{}


	void AnimationComponent::UpdateWorldTransformMatrix(
			ozz::unique_ptr<ozz::animation::Skeleton>& skeleton,
			const std::vector<Bone>& bones,
			Time& time
	)
	{
		if(!CheckIfAnimationValid(_selectedAnimationIndex) || skeleton == nullptr)
			return;

		const ozz::animation::Animation* ozzAnimation = _mesh.DeRef()->animations[_selectedAnimationIndex].get();

		timer.Update(time, ozzAnimation->duration());

		ozz::vector<ozz::math::SoaTransform> localTransformOZZ;
		ozz::vector<ozz::math::Float4x4> worldTransformOZZ;
		ozz::animation::SamplingJob::Context context;

		context.Resize(skeleton->num_joints());
		localTransformOZZ.resize(skeleton->num_joints());
		worldTransformOZZ.resize(skeleton->num_joints());

		// Samples runtime animation data
		ozz::animation::SamplingJob samplingJob;

		samplingJob.animation = ozzAnimation;
		samplingJob.context = &context;
		samplingJob.ratio = timer.animationTime/ozzAnimation->duration();
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
		if (CheckIfAnimationValid(inID))
		{
			_selectedAnimationIndex = inID;
		}
		else
		{
			_selectedAnimationIndex = 0;
			timer.animationTime = 0;
		}
	}

	unsigned int AnimationComponent::GetCurrentAnimationID() const
	{
		return _selectedAnimationIndex;
	}

	unsigned int AnimationComponent::GetAmountOfAnimations() const
	{
		if (CheckIfAnimationValid())
		{
			return _mesh.DeRef()->animations.size();
		}
		else
		{
			return 0;
		}
	}

	std::string AnimationComponent::GetAnimationName(unsigned int inID) const
	{
		if(CheckIfAnimationValid(inID))
			return _mesh.DeRef()->animations[inID]->name();
		else
		{
			PLATINUM_WARNING("Animation ID exceed the total amount of animations.");
			return "";
		}
	}

	void AnimationComponent::SetMesh(SavedReference<Mesh> inMesh)
	{
		if(!inMesh)
			return;
		if( inMesh.DeRef()->animations.empty())
		{
			// empty the mesh if the input mesh has no animation
			_mesh = {};
		}
		else
		{
			// Set mesh as the input mesh
			_mesh = std::move(inMesh);
			// Set default selected animation
			this->GetGameObject().DeRef()->GetComponent<AnimationComponent>().DeRef()->SetCurrentAnimationByID(0);
		}
	}

	bool AnimationComponent::CheckIfAnimationValid(unsigned int inID) const
	{
		// return false if the mesh and animation are invalid
		if(!_mesh)
			return false;
		if(_mesh.DeRef()->animations.empty())
			return false;
		if(_mesh.DeRef()->animations.size() <= inID)
			return false;

		// return true if the mesh and animation are valid
		return true;
	}

	void AnimationComponent::SetIsDisplay(bool inIsDisplay)
	{
		if(CheckIfAnimationValid())
			_isDisplay = inIsDisplay;
	}

	bool AnimationComponent::GetIsDisplay() const
	{
		return _isDisplay;
	}

	void AnimationComponent::OnIDSystemUpdate()
	{
		_mesh.OnIDSystemUpdate(Application::Instance->idSystem);
	}

}


