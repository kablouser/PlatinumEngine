//
// Created by shawn on 2022/4/22.
//

#include "ComponentComposition/RigidBody.h"
#include <ComponentComposition/Transform.h>
#include <Logger/Logger.h>
#include <SceneManager/Scene.h>

namespace PlatinumEngine
{
	RigidBody::RigidBody():
						mass(1.f),
						kinematic(false),
						_inertia(Maths::Vec3(0.f, 0.f, 0.f)),
						damping(0.f),
						angularDamping(0.f)
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

	void RigidBody::OnStart(Scene& scene)
	{
		// Set the btmotionState
		{
			auto objectTransform = GetComponent<Transform>();

			_motionState = new btDefaultMotionState(
					btTransform(Physics::convertQuaternion(objectTransform->localRotation),
							Physics::convertVector(objectTransform->localPosition)));
		}
		//Set the btRigidBody based on its collider type
		{
			auto temp = Physics::convertVector(_inertia);

			if (GetComponent<BoxCollider>() != nullptr)
			{
				GetComponent<BoxCollider>()->GetShape()->calculateLocalInertia(mass, temp);
				btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, _motionState,
						GetComponent<BoxCollider>()->GetShape(), temp);
				_rigidBody = new btRigidBody(constructionInfo);
			}
			else if (GetComponent<SphereCollider>() != nullptr)
			{
				GetComponent<SphereCollider>()->GetShape()->calculateLocalInertia(mass, temp);
				btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, _motionState,
						GetComponent<SphereCollider>()->GetShape(), temp);
				_rigidBody = new btRigidBody(constructionInfo);
			}
			else if (GetComponent<CapsuleCollider>() != nullptr)
			{
				GetComponent<CapsuleCollider>()->GetShape()->calculateLocalInertia(mass, temp);
				btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, _motionState,
						GetComponent<CapsuleCollider>()->GetShape(), temp);
				_rigidBody = new btRigidBody(constructionInfo);
			}
		}
		//Add the rigidBody to the physics world
		scene.physics.AddBulletBody(_rigidBody);
		_rigidBody->setDamping(damping, angularDamping);
	}

	void RigidBody::OnEnd(Scene& scene)
	{
		delete _motionState;
		delete _rigidBody;
	}
}

