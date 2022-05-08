#include "ComponentComposition/Transform.h"


namespace PlatinumEngine
{
	void Transform::CreateTypeInfo(TypeDatabase& database)
	{
		/*
		Maths::Vec3 localPosition;
		Maths::Quaternion localRotation;
		Maths::Mat4 transformationMatrix;
		float localScale;
		 */
		database.BeginTypeInfo<Transform>()
		        .WithInherit<Component>()
		        .WithField<Maths::Vec3>("localPosition", PLATINUM_OFFSETOF(Transform, localPosition))
				.WithField<Maths::Quaternion>("localRotation", PLATINUM_OFFSETOF(Transform, localRotation))
				.WithField<Maths::Mat4>("transformationMatrix", PLATINUM_OFFSETOF(Transform, transformationMatrix))
				.WithField<float>("localScale", PLATINUM_OFFSETOF(Transform, localScale));
	}

	//Constructors

	Transform::Transform()
	{
		localPosition = Maths::Vec3();
		localRotation = Maths::Quaternion::identity;
		localScale = 1.f;
	}

	void Transform::Rotate(Maths::Vec3 axis, float angle)
	{
		RotateAround(localPosition, axis, angle);
	}

	void Transform::RotateAround(Maths::Vec3 point, Maths::Vec3 axis, float angle)
	{
		angle *= (Maths::Common::PI / 180.f);
		Maths::Quaternion q = Maths::Quaternion::AngleAxis(axis, angle);
		localPosition = q * (localPosition - point) + point;
		localRotation *= q;
	}

	void Transform::LookAt(Maths::Vec3 target)
	{
		Maths::Vec3 dir = target - localPosition;
		dir = Maths::Normalise(dir);
		glm::quat q = glm::quatLookAt(dir, Maths::Vec3::up);
		localRotation = Maths::Quaternion(q.w, q.x, q.y, q.z);
	}

	Maths::Vec3 Transform::LocalToParentDirection(Maths::Vec3 dir)
	{
		return localRotation * dir;
	}

	Maths::Vec3 Transform::ParentToLocalDirection(Maths::Vec3 dir)
	{
		return Maths::Quaternion::Inverse(localRotation) * dir;
	}

	Maths::Mat4 Transform::GetWorldToLocalMatrix()
	{
		return Maths::Inverse(GetLocalToWorldMatrix());
	}

	Maths::Mat4 Transform::GetLocalToWorldMatrix()
	{
		Maths::Mat4 transformMatrix = GetLocalToParentMatrix();
		SavedReference<Transform> transform = GetParentComponent<Transform>();
		if (transform)
			// recursive, slow but oh well
			return transform.DeRef()->GetLocalToWorldMatrix() * transformMatrix;
		return transformMatrix;
	}

	Maths::Mat4 Transform::GetLocalToParentMatrix()
	{
		Maths::Mat4 result;
		result.SetTRSMatrix(localPosition, localRotation, localScale);
		return result;
	}

	Maths::Vec3 Transform::GetLocalForward()
	{
		return localRotation * Maths::Vec3::forward;
	}

	Maths::Vec3 Transform::GetLocalUp()
	{
		return localRotation * Maths::Vec3::up;
	}

	Maths::Vec3 Transform::GetLocalRight()
	{
		return localRotation * Maths::Vec3::right;
	}

	void Transform::SetLocalToWorldMatrix(Maths::Mat4 LocalToWorld)
	{
		Maths::Mat4 ParentToWorld, LocalToParent;
		SavedReference<Transform> parent = GetParentComponent<Transform>();
		if (parent)
			ParentToWorld = parent.DeRef()->GetLocalToWorldMatrix();
		else
			ParentToWorld.SetIdentityMatrix();

		LocalToParent = Inverse(ParentToWorld) * LocalToWorld;
		LocalToParent.Decompose(&localPosition, &localRotation, &localScale);
	}

}