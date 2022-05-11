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
				.WithField<unsigned int>("unsigned int", PLATINUM_OFFSETOF(AnimationAttachment,selectedJoint))
				.WithField<Maths::Vec3>("Maths::Vec3", PLATINUM_OFFSETOF(AnimationAttachment,translation))
				.WithField<Maths::Vec3>("Maths::Vec3", PLATINUM_OFFSETOF(AnimationAttachment,rotation));

	}
	AnimationAttachment::AnimationAttachment():translation(0.f, 0.f, 0.f),rotation(0.f, 0.f, 0.f),selectedJoint(0)
	{}

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

		if(parentAnimation.DeRef()->worldTransform.size() <= selectedJoint)
		{
			CleanAttachmentSetting();
			return;
		}

		transformMatrix = parentAnimation.DeRef()->worldTransform[selectedJoint];

	}

	void AnimationAttachment::SetSelectedJoint(unsigned int jointID)
	{
		selectedJoint = jointID;
		UpdateTransformMatrixBySelectedJoint();
	}

	void AnimationAttachment::CleanAttachmentSetting()
	{
		offsetMatrix.SetIdentityMatrix();
		transformMatrix.SetIdentityMatrix();
		jointsName.clear();
		selectedJoint = 0;
	}

	void AnimationAttachment::UpdateOffsetMatrix(Maths::Vec3 inTranslation, Maths::Vec3 inRotation)
	{

		translation = inTranslation;
		rotation = inRotation;

		Maths::Mat4 translationMat, rotationMat;
		translationMat.SetTranslationMatrix(translation);
		rotationMat.SetRotationMatrix(rotation);
		offsetMatrix = translationMat * rotationMat;
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