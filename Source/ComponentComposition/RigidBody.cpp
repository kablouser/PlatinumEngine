//
// Created by 肖宜辰 on 2022/4/22.
//

#include "ComponentComposition/RigidBody.h"
#include <ComponentComposition/Transform.h>
namespace PlatinumEngine
{
	RigidBody::RigidBody(Physics* physics):
						_mass(1.f),
						_kinematic(false),
						_inertia(Maths::Vec3(0.f, 0.f, 0.f)),

						_physics(physics)
	{

	}

	btTransform RigidBody::GetWorldTransform()
	{
		btTransform temp;
		_rigidBody->getMotionState()->getWorldTransform(temp);
		return temp;
	}
	Maths::Vec3 RigidBody::GetBulletRotation()
	{
		Maths::Quaternion temp;
		temp = Physics::convertVQuaternionBack(GetWorldTransform().getRotation());
		return(temp.EulerAngles());
	}
	Maths::Vec3 RigidBody::GetBulletPosition()
	{
		return Physics::convertVectorBack(GetWorldTransform().getOrigin());
	}

	Maths::Vec3 RigidBody::GetForce()
	{
		return _inertia;
	}
	float RigidBody::GetMass()
	{
		return _mass;
	}
	bool RigidBody::GetKinematic()
	{
		return _kinematic;
	}

	void RigidBody::SetForce(Maths::Vec3 force)
	{
		_inertia = force;
	}
	void RigidBody::SetMass(float mass)
	{
		_mass = mass;
	}
	void RigidBody::SetKinematic(bool value)
	{
		_kinematic = value;
	}

	void RigidBody::Initialize(btCollisionShape* shape)
	{
		if(GetGameObject() == nullptr || (GetComponent<Collider>() == nullptr))
		{
			return;
		}
		//if the object is attached with some rigidBody, delete it and replace
		delete _rigidBody;
		_rigidBody = nullptr;

		auto objectTransform = GetComponent<Transform>();

		btDefaultMotionState* motionState = new btDefaultMotionState(
				btTransform(btQuaternion(objectTransform->localRotation.x,objectTransform->localRotation.y,objectTransform->localRotation.z),
					   Physics::convertVector(objectTransform->localPosition)));

		auto temp = Physics::convertVector(_inertia);
		shape->calculateLocalInertia(_mass, temp);

		btRigidBody::btRigidBodyConstructionInfo constructionInfo(_mass, motionState, shape, Physics::convertVector(_inertia));
		_rigidBody = new btRigidBody(constructionInfo);

		_physics->AddBulletBody(_rigidBody);
	}
}

