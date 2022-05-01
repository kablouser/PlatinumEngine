#include "ComponentComposition/TransformComponent.h"

namespace PlatinumEngine
{
	void TransformComponent::CreateTypeInfo(TypeDatabase& database)
	{
		/*
		Maths::Vec3 localPosition;
		Maths::Quaternion localRotation;
		Maths::Mat4 transformationMatrix;
		float localScale;
		 */
		database.BeginTypeInfo<TransformComponent>()
		        .WithInherit<Component>()
		        .WithField<Maths::Vec3>("localPosition", PLATINUM_OFFSETOF(TransformComponent, localPosition))
				.WithField<Maths::Quaternion>("localRotation", PLATINUM_OFFSETOF(TransformComponent, localRotation))
				.WithField<Maths::Mat4>("transformationMatrix", PLATINUM_OFFSETOF(TransformComponent, transformationMatrix))
				.WithField<float>("localScale", PLATINUM_OFFSETOF(TransformComponent, localScale));
	}

	//Constructors

	TransformComponent::TransformComponent()
	{
		localPosition = Maths::Vec3();
		localRotation = Maths::Quaternion::identity;
		localScale = 1.f;
	}

	void TransformComponent::Rotate(Maths::Vec3 axis, float angle)
	{
		RotateAround(localPosition, axis, angle);
	}

	void TransformComponent::RotateAround(Maths::Vec3 point, Maths::Vec3 axis, float angle)
	{
		angle *= (Maths::Common::PI / 180.f);
		Maths::Quaternion q = Maths::Quaternion::AngleAxis(axis, angle);
		localPosition = q * (localPosition - point) + point;
		localRotation *= q;
	}

	void TransformComponent::LookAt(Maths::Vec3 target)
	{
		Maths::Vec3 dir = target - localPosition;
		dir = Maths::Normalise(dir);
		glm::quat q = glm::quatLookAt(dir, Maths::Vec3::up);
		localRotation = Maths::Quaternion(q.w, q.x, q.y, q.z);
	}

	Maths::Vec3 TransformComponent::LocalToParentDirection(Maths::Vec3 dir)
	{
		return localRotation * dir;
	}

	Maths::Vec3 TransformComponent::ParentToLocalDirection(Maths::Vec3 dir)
	{
		return Maths::Quaternion::Inverse(localRotation) * dir;
	}

	Maths::Mat4 TransformComponent::GetWorldToLocalMatrix()
	{
		return Maths::Inverse(GetLocalToWorldMatrix());
	}

	Maths::Mat4 TransformComponent::GetLocalToWorldMatrix()
	{
		Maths::Mat4 transformMatrix = GetLocalToParentMatrix();
		SavedReference<TransformComponent> transform = GetParentComponent<TransformComponent>();
		if (transform)
			// recursive, slow but oh well
			return transform.pointer->GetLocalToWorldMatrix() * transformMatrix;
		return transformMatrix;
	}

	Maths::Mat4 TransformComponent::GetLocalToParentMatrix()
	{
		Maths::Mat4 result;
		result.SetTRSMatrix(localPosition, localRotation, localScale);
		return result;
	}

	Maths::Vec3 TransformComponent::GetLocalForward()
	{
		return localRotation * Maths::Vec3::forward;
	}

	Maths::Vec3 TransformComponent::GetLocalUp()
	{
		return localRotation * Maths::Vec3::up;
	}

	Maths::Vec3 TransformComponent::GetLocalRight()
	{
		return localRotation * Maths::Vec3::right;
	}

	void TransformComponent::SetLocalToWorldMatrix(Maths::Mat4 LocalToWorld)
	{
		Maths::Mat4 ParentToWorld, LocalToParent;
		SavedReference<TransformComponent> parent = GetParentComponent<TransformComponent>();
		if (parent)
			ParentToWorld = parent.pointer->GetLocalToWorldMatrix();
		else
			ParentToWorld.SetIdentityMatrix();

		LocalToParent = Inverse(ParentToWorld) * LocalToWorld;
		LocalToParent.Decompose(&localPosition, &localRotation, &localScale);
	}

}