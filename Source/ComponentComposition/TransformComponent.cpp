#include "ComponentComposition/TransformComponent.h"

namespace PlatinumEngine
{
	//Constructors

	TransformComponent::TransformComponent()
	{
		position = Maths::Vec3();
		rotation = Maths::Quaternion();
		scale = Maths::Vec3();
		_localposition = Maths::Vec3();
	}
	TransformComponent::~TransformComponent()
	{
	}

	//Public Methods

	//Transformations are Global unless space is specified
	void TransformComponent::Translate(Maths::Vec3 translation)
	{
		position += translation;
	}
	void TransformComponent::Translate(Maths::Vec3 translation, relativeTo space)
	{
		if(space==relativeTo::WORLD)
			position += translation;
		else
			position += TransformDirection(translation);
	}
	void TransformComponent::Translate(float x, float y, float z)
	{
		position += Maths::Vec3(x,y,z);
	}
	void TransformComponent::Translate(float x, float y, float z, relativeTo space)
	{
		if(space==relativeTo::WORLD)
			position += Maths::Vec3(x,y,z);
		else
			position += TransformDirection(Maths::Vec3(x,y,z));
	}

	//Transformations are Local unless space is specified
	void TransformComponent::Rotate(Maths::Vec3 euler)
	{
		Maths::Quaternion eulerRot = Maths::Quaternion::EulerToQuat(euler);
		rotation *= eulerRot;
	}
	void TransformComponent::Rotate(Maths::Quaternion q)
	{
		rotation *= q;
	}
	void TransformComponent::Rotate(Maths::Vec3 axis, float angle)
	{
		RotateAround(position, axis, angle);
	}
	void TransformComponent::Rotate(Maths::Vec3 euler, relativeTo space)
	{
		Maths::Quaternion eulerRot = Maths::Quaternion::EulerToQuat(euler);
		if(space==relativeTo::LOCAL)
			rotation *= eulerRot;
		else
			rotation *= (Maths::Quaternion::Inverse(rotation) * eulerRot * rotation);
	}
	void TransformComponent::Rotate(Maths::Quaternion q, relativeTo space)
	{
		if(space==relativeTo::LOCAL)
			rotation *= q;
		else
			rotation *= (Maths::Quaternion::Inverse(rotation) * q * rotation);
	}
	void TransformComponent::Rotate(Maths::Vec3 axis, float angle, relativeTo space)
	{
		if (space == relativeTo::LOCAL)
			RotateAround(position, TransformDirection(axis), angle);
		else
			RotateAround(position, axis, angle);
	}

	//Rotates by an angle around an axis passing through a point
	void TransformComponent::RotateAround(Maths::Vec3 point, Maths::Vec3 axis, float angle)
	{
		Maths::Quaternion q = Maths::Quaternion::AngleAxis(axis, angle);
		position = q * (position-point) + point;
		rotation *= q;
	}

	void TransformComponent::LookAt(Maths::Vec3 target)
	{
		Maths::Vec3 forwardVector = target - position;
		forwardVector = Maths::Normalise(forwardVector);
		Maths::Vec3 f = forward(), u = up();
		float dot = Maths::Dot(f, forwardVector);
		if (abs(dot - (-1.0f)) < _eps)
		{
			Rotate(Maths::Quaternion(u.x, u.y, u.z, _PI));
			return;
		}
		if (abs(dot - (1.0f)) < _eps)
		{
			Rotate(Maths::Quaternion::identity);
			return;
		}
		float rotAngle = (float)acos(dot);
		Maths::Vec3 rotAxis = Maths::Cross(forward(), forwardVector);
		rotAxis = Maths::Normalise(rotAxis);
		Rotate(Maths::Quaternion::AngleAxis(rotAxis, rotAngle));
	}

	//Transforms direction from local space to world space
	Maths::Vec3 TransformComponent::TransformDirection(Maths::Vec3 dir)
	{
		return GetWorldRotation()*dir;
	}
	//Transforms direction from world space to local space
	Maths::Vec3 TransformComponent::InverseTransformDirection(Maths::Vec3 dir)
	{
		return Maths::Quaternion::Inverse(rotation)*dir;
	}
	//Transforms point from local space to world space
	Maths::Vec3 TransformComponent::TransformPoint(Maths::Vec3 point)
	{
		Maths::Vec4 worldPos = GetLocalToWorldMatrix()*Maths::Vec4(point.x,point.y,point.z,1.f);
		return Maths::Vec3(worldPos.x, worldPos.y, worldPos.z);
	}
	//Transforms point from world space to local space
	Maths::Vec3 TransformComponent::InverseTransformPoint(Maths::Vec3 point)
	{
		Maths::Vec3 newPosition, localPosition;
		if (parent)
			localPosition = parent.InverseTransformPoint(point);
		else
			localPosition = point;
		localPosition  -= _localposition;
		newPosition = Maths::Quaternion::Inverse(rotation)*localPosition;
		return newPosition*Maths::Vec3(1.f/scale.x,1.f/scale.y,1.f/scale.z);
	}

	//Returns matrix with translation and rotation but no scaling
	Maths::Mat4 TransformComponent::GetWorldToLocalMatrixNoScale()
	{	return SetTRInverse(position, rotation);	}
	//Returns matrix with translation, rotation, scale
	Maths::Mat4 TransformComponent::GetWorldToLocalMatrix()
	{
		Maths::Mat4 m = GetLocalToWorldMatrix();
		m.Invert_Full();
		return m;
	}

	Maths::Mat4 TransformComponent::GetLocalToWorldMatrix()
	{
		Maths::Mat4 t;
		t.SetTranslationMatrix(_localposition);
		t.SetRotationMatrix(rotation.EulerAngles());
		t.SetScaleMatrix(scale);
		if(parent)
			t = parent.GetLocalToWorldMatrix()*t;
		return t;
	}

	Maths::Vec3 TransformComponent::GetLocalPosition()
	{
		Maths::Vec4 v = GetWorldToLocalMatrix()*Maths::Vec4(position.x,position.y,position.z,1.f);
		_localposition = Maths::Vec3(v.x,v.y,v.z);
		return _localposition;
	}

	Maths::Quaternion TransformComponent::GetWorldRotation()
	{
		Maths::Quaternion worldRot = rotation;
		while(parent)
		{
			worldRot *= rotation;
			parent = parent.parent;
		}
		return worldRot;
	}

	//Get/Set vectors w.r.t the object
	Maths::Vec3 TransformComponent::forward()
	{	return rotation * _forward;	}
	void TransformComponent::forward(Maths::Vec3 v)
	{	rotation = Maths::Quaternion::LookRotation(v);	}
	Maths::Vec3 TransformComponent::up()
	{	return rotation * _up;	}
	void TransformComponent::up(Maths::Vec3 v)
	{	rotation = Maths::Quaternion::FromToRotation(_up, v);	}
	Maths::Vec3 TransformComponent::right()
	{	return rotation * _right;	}
	void TransformComponent::right(Maths::Vec3 v)
	{	rotation = Maths::Quaternion::FromToRotation(_right, v);	}

	//Private Methods

	//Just sets the matrix using translation and the inverse rotation
	Maths::Mat4 TransformComponent::SetTRInverse(Maths::Vec3 pos, Maths::Quaternion rot)
	{
		Maths::Mat4 n = Maths::Quaternion::QuaternionToMatrix (Maths::Quaternion::Inverse(rot));
		n.SetTranslationMatrix(Maths::Vec3(-pos[0], -pos[1], -pos[2]));
		return n;
	}

	//Private Static Variables

	Maths::Vec3 TransformComponent::_forward = Maths::Vec3(0.f,0.f,1.f);
	Maths::Vec3 TransformComponent::_up = Maths::Vec3(0.f,1.f,0.f);
	Maths::Vec3 TransformComponent::_right = Maths::Vec3(1.f,0.f,0.f);
	float TransformComponent::_eps = 0.0000001f;
	float TransformComponent::_PI = 3.1415926535897932f;

}