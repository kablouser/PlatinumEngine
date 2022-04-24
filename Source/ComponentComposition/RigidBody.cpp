//
// Created by 肖宜辰 on 2022/4/22.
//

#include "ComponentComposition/RigidBody.h"
#include <ComponentComposition/Transform.h>
namespace PlatinumEngine
{
	RigidBody::RigidBody(GameObject* gameObject, Physics* physics):
						_mass(1.f),
						_kinematic(false),
						_initialForce(Maths::Vec3(0.f, 0.f, 0.f)),

						_attachedObject(gameObject),
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
		temp = _physics->convertVQuaternionBack(GetWorldTransform().getRotation());
		return(temp.EulerAngles());
	}
	Maths::Vec3 RigidBody::GetBulletPosition()
	{
		return _physics->convertVectorBack(GetWorldTransform().getOrigin());
	}

	Maths::Vec3 RigidBody::GetForce()
	{
		return _initialForce;
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
		_initialForce = force;
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
		if(_attachedObject == nullptr || (_attachedObject->GetComponent<BoxCollider>() == nullptr ||
		       							  _attachedObject->GetComponent<SphereCollider>() == nullptr ||
										  _attachedObject->GetComponent<CapsuleCollider>() == nullptr))
		{
			return;
		}
		//if the object is attached with some rigidBody, delete it and replace
		delete _rigidBody;
		_rigidBody = nullptr;

		auto objectTransform = _attachedObject->GetComponent<Transform>();

		btDefaultMotionState* motionState = new btDefaultMotionState(
				btTransform(btQuaternion(objectTransform->localRotation.x,objectTransform->localRotation.y,objectTransform->localRotation.z),
					   _physics->convertVector(objectTransform->localPosition)));

		auto temp = _physics->convertVector(_initialForce);
		shape->calculateLocalInertia(_physics->convertScalar(_mass), temp);

		btRigidBody::btRigidBodyConstructionInfo constructionInfo(_mass, motionState, shape, _physics->convertVector(_initialForce));
		_rigidBody = new btRigidBody(constructionInfo);

		_physics->AddBulletBody(_rigidBody);

	}
}

