#include "ComponentComposition/TransformComponent.h"

namespace PlatinumEngine
{
	//Constructors

	TransformComponent::TransformComponent()
	{
		position = Maths::Vec3();
		rotation = Maths::Quaternion::identity;
		scale = Maths::Vec3(1.f,1.f,1.f);
	}
	TransformComponent::~TransformComponent()
	{
	}

	//Public Methods

	//Transformations are Global unless space is specified

	void TransformComponent::Translate(Maths::Vec3 translation, RelativeTo space)
	{
		if(space==RelativeTo::world)
			position += translation;
		else
			position += TransformDirection(translation);
	}
	void TransformComponent::Translate(float x, float y, float z, RelativeTo space)
	{
		if(space==RelativeTo::world)
			position += Maths::Vec3(x,y,z);
		else
			position += TransformDirection(Maths::Vec3(x,y,z));
	}

	//Transformations are Local unless space is specified

	void TransformComponent::Rotate(Maths::Vec3 euler, RelativeTo space)
	{
		Maths::Quaternion eulerRot = Maths::Quaternion::EulerToQuaternion(euler*(Maths::Common::PI/180.f));
		if(space==RelativeTo::local)
			rotation *= eulerRot;
		else
			rotation *= (Maths::Quaternion::Inverse(rotation) * eulerRot * rotation);
	}
	void TransformComponent::Rotate(Maths::Quaternion q, RelativeTo space)
	{
		if(space==RelativeTo::local)
			rotation *= q;
		else
			rotation *= (Maths::Quaternion::Inverse(rotation) * q * rotation);
	}
	void TransformComponent::Rotate(Maths::Vec3 axis, float angle, RelativeTo space)
	{
		if (space == RelativeTo::local)
			RotateAround(position, TransformDirection(axis), angle);
		else
			RotateAround(position, axis, angle);
	}

	//Rotates by an angle around an axis passing through a point
	void TransformComponent::RotateAround(Maths::Vec3 point, Maths::Vec3 axis, float angle)
	{
		angle *= (Maths::Common::PI / 180.f);
		Maths::Quaternion q = Maths::Quaternion::AngleAxis(axis, angle);
		position = q * (position-point) + point;
		rotation *= q;
	}
	//Calculates a rotation that points in the direction of the target
	void TransformComponent::LookAt(Maths::Vec3 target)
	{
		Maths::Vec3 dir = target-position;
		dir = Maths::Normalise(dir);
		glm::quat q = glm::quatLookAt(dir,Maths::Vec3::up);
		rotation = Maths::Quaternion(q.w,q.x,q.y,q.z);
	}

	//Transforms direction from local space to world space
	Maths::Vec3 TransformComponent::TransformDirection(Maths::Vec3 dir)
	{
		return rotation*dir;
	}
	//Transforms direction from world space to local space
	Maths::Vec3 TransformComponent::InverseTransformDirection(Maths::Vec3 dir)
	{
		return Maths::Quaternion::Inverse(rotation)*dir;
	}

	//Returns matrix with translation and rotation but no scaling which converts from world to local space
	Maths::Mat4 TransformComponent::GetWorldToLocalMatrixNoScale()
	{
		Maths::Mat4 invR, invT;
		invR = Maths::Quaternion::QuaternionToMatrix(Maths::Quaternion::Inverse(rotation));
		invT.SetTranslationMatrix(-position);
		return invR*invT;
	}
	//Returns matrix with translation, rotation, scale which converts from world to local space
	Maths::Mat4 TransformComponent::GetWorldToLocalMatrix()
	{
		return Maths::Inverse(GetLocalToWorldMatrix());
	}
	//Returns matrix which converts from local to world space
	Maths::Mat4 TransformComponent::GetLocalToWorldMatrix()
	{
		Maths::Mat4 t,r,s,trs;
		t.SetTranslationMatrix(GetLocalPosition());
		r.SetRotationMatrix(rotation.EulerAngles());
		s.SetScaleMatrix(scale);
		trs = t*r*s;
		GameObject* parent = GetGameObject()->GetParent();
		if(parent)
			trs = parent->GetComponent<TransformComponent>()->GetLocalToWorldMatrix()*trs;
		return trs;
	}
	//Local position w.r.t parent (if any)
	Maths::Vec3 TransformComponent::GetLocalPosition()
	{
		GameObject* parent = GetGameObject()->GetParent();
		if(parent)
		{
			Maths::Vec3 parentpos = parent->GetComponent<TransformComponent>()->position;
			return (position - parentpos);
		}
		return position;
	}

	//Get/Set vectors w.r.t the object
	Maths::Vec3 TransformComponent::GetForward()
	{
		return rotation * Maths::Vec3::forward;
	}
	Maths::Vec3 TransformComponent::GetUp()
	{
		return rotation * Maths::Vec3::up;
	}
	Maths::Vec3 TransformComponent::GetRight()
	{
		return rotation * Maths::Vec3::right;
	}
}