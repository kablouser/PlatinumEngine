#include "ComponentComposition/TransformComponent.h"

namespace PlatinumEngine
{
	TransformComponent::TransformComponent()
	{
		position = Maths::Vec3();
		rotation = Maths::Quaternion();
		scale = Maths::Vec3();
		forward = Maths::Vec3(0.f,0.f,1.f);
		up = Maths::Vec3(0.f,1.f,0.f);
		right = Maths::Vec3(1.f,0.f,0.f);
	}

	void TransformComponent::Translate(Maths::Vec3 translation)
	{
		position += translation;
	}

	void TransformComponent::Rotate(Maths::Vec3 euler)
	{
		Maths::Quaternion eulerRot = Maths::Quaternion.EulerToQuaternion(eulers.x, eulers.y, eulers.z);
		rotation = rotation * eulerRot;
	}

	void TransformComponent::RotateAround(Maths::Vec3 point, Maths::Vec3 axis, float angle)
	{

	}

	void TransformComponent::Scale(Maths::Vec3 newScale)
	{
		scale.x *= newScale.x;
		scale.y *= newScale.y;
		scale.z *= newScale.z;
	}

	void TransformComponent::LookAt(Maths::Vec3 target)
	{

	}
}