//
// Created by Shihua on 10/05/2022.
//

#include <ComponentComposition/AnimationAttachment.h>

namespace PlatinumEngine
{
	void AnimationAttachment::CreateTypeInfo(TypeDatabase& typeDatabase)
	{

		typeDatabase.BeginTypeInfo<AnimationAttachment>()
				.WithInherit<Component>()
				.WithField<std::vector<std::string>>("jointsName", PLATINUM_OFFSETOF(AnimationAttachment,jointsName))
				.WithField<unsigned int>("_selectedJoint", PLATINUM_OFFSETOF(AnimationAttachment,_selectedJoint))
				.WithField<Maths::Vec3>("translation", PLATINUM_OFFSETOF(AnimationAttachment,translation))
				.WithField<Maths::Vec3>("rotation", PLATINUM_OFFSETOF(AnimationAttachment,rotation))
				.WithField<float>("scale", PLATINUM_OFFSETOF(AnimationAttachment,scale))
				.WithField<Maths::Mat4>("offsetMatrix", PLATINUM_OFFSETOF(AnimationAttachment,offsetMatrix))
				.WithField<Maths::Mat4>("transformMatrix", PLATINUM_OFFSETOF(AnimationAttachment,transformMatrix));

	}
	AnimationAttachment::AnimationAttachment():translation(0.f, 0.f, 0.f),rotation(0.f, 0.f, 0.f),_selectedJoint(0), scale(1.0f)
	{
	}

	void AnimationAttachment::GetSkeletonNameFromParentAnimation()
	{
		// Get joint name from parent mesh
		SavedReference<MeshRender> meshRender = GetParentComponent<MeshRender>();

		if(!meshRender)
		{
			CleanAttachmentSetting();
			return;
		}

		SavedReference<Mesh> parentMesh = meshRender.DeRef()->GetMesh();

		if(!parentMesh || parentMesh.DeRef()->skeleton == nullptr)
		{
			CleanAttachmentSetting();
			return;
		}
		jointsName.clear();
		for(unsigned int i =0; i < parentMesh.DeRef()->skeleton->num_joints() ; i++)
			jointsName.emplace_back(parentMesh.DeRef()->skeleton->joint_names()[i]);
	}

	void AnimationAttachment::UpdateTransformMatrixBySelectedJoint()
	{
		SavedReference <AnimationComponent> parentAnimation = GetParentComponent<AnimationComponent>();

		if(!parentAnimation)
		{
			CleanAttachmentSetting();
			return;
		}

		if(parentAnimation.DeRef()->worldTransform.size() <= _selectedJoint)
		{
			CleanAttachmentSetting();
			return;
		}

		transformMatrix = parentAnimation.DeRef()->worldTransform[_selectedJoint];

	}

	void AnimationAttachment::SetSelectedJoint(unsigned int jointID)
	{
		_selectedJoint = jointID;
		UpdateTransformMatrixBySelectedJoint();
	}

	unsigned int AnimationAttachment::GetSelectedJoint()
	{
		return _selectedJoint;
	}

	void AnimationAttachment::CleanAttachmentSetting()
	{
		offsetMatrix.SetIdentityMatrix();
		transformMatrix.SetIdentityMatrix();
		jointsName.clear();
		_selectedJoint = 0;
	}

	void AnimationAttachment::UpdateOffsetMatrix(Maths::Vec3 inTranslation, Maths::Vec3 inRotation, float inScale)
	{

		translation = inTranslation;
		rotation = inRotation;
		scale = inScale;

		Maths::Mat4 translationMat, rotationMat, scaleMat;
		translationMat.SetTranslationMatrix(translation);
		rotationMat.SetRotationMatrix(rotation);
		scaleMat.SetScaleMatrix(Maths::Vec3(scale, scale, scale));
		offsetMatrix = translationMat * rotationMat * scaleMat;
	}

	bool AnimationAttachment::CheckIfParentAnimationDisplay()
	{
		SavedReference <AnimationComponent> parentAnimation = GetParentComponent<AnimationComponent>();

		if(!parentAnimation)
		{
			CleanAttachmentSetting();
			return false;
		}

		return parentAnimation.DeRef()->GetIsDisplay();
	}
}