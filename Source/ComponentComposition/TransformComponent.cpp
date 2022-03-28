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

	TransformComponent::~TransformComponent()
	{
	}

	void TransformComponent::Translate(Maths::Vec3 translation)
	{
		position += translation;
	}

	void TransformComponent::Rotate(Maths::Vec3 euler)
	{
		Maths::Quaternion eulerRot = Maths::Quaternion::EulerToQuat(euler);
		rotation = rotation * eulerRot;
	}

	void TransformComponent::Rotate(Maths::Quaternion q)
	{
		rotation = rotation * q;
	}

	void TransformComponent::RotateAround(Maths::Vec3 point, Maths::Vec3 axis, float angle)
	{
		Maths::Quaternion q = Maths::Quaternion::AngleAxis(up, angle);
		position = q.EulerAngles() * (position-point) + point;
		rotation *= q;
	}

	void TransformComponent::Scale(Maths::Vec3 newScale)
	{
		scale.x *= newScale.x;
		scale.y *= newScale.y;
		scale.z *= newScale.z;
	}

	void TransformComponent::LookAt(Maths::Vec3 target)
	{
		Maths::Vec3 forwardVector = target - position;
		forwardVector = Maths::Normalise(forwardVector);
		float dot = Maths::Dot(forward, forwardVector);

		if (abs(dot - (-1.0f)) < 0.000001f)
		{
			Rotate(Maths::Quaternion(up.x, up.y, up.z, 3.1415926535897932f));
			return;
		}
		if (abs(dot - (1.0f)) < 0.000001f)
		{
			Rotate(Maths::Quaternion::identity);
			return;
		}

		float rotAngle = (float)acos(dot);
		Maths::Vec3 rotAxis = Maths::Cross(forward, forwardVector);
		rotAxis = Maths::Normalise(rotAxis);
		Rotate(Maths::Quaternion::AngleAxis(rotAxis, rotAngle));
	}
}