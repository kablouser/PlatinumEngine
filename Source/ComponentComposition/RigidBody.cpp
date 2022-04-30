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
						_inertia(Maths::Vec3(0.f, 0.f, 0.f))
	{

	}

	btTransform RigidBody::GetWorldTransform()
	{
		btTransform temp;
		_rigidBody->getMotionState()->getWorldTransform(temp);
		return temp;
	}
	Maths::Quaternion RigidBody::GetBulletRotation()
	{
		Maths::Quaternion temp;
		temp = Physics::ConvertQuaternionBack(GetWorldTransform().getRotation());
		return temp;
	}

	Maths::Vec3 RigidBody::GetBulletPosition()
	{
		return Physics::ConvertVectorBack(GetWorldTransform().getOrigin());
	}

	void RigidBody::OnStart(Scene& scene)
	{
		// Set the btmotionState
		{
			auto objectTransform = GetComponent<Transform>();

			_motionState = new btDefaultMotionState(
					btTransform(Physics::ConvertQuaternion(objectTransform->localRotation),
							Physics::ConvertVector(objectTransform->localPosition)));
		}
		//Set the btRigidBody based on its collider type
		{
			auto temp = Physics::ConvertVector(_inertia);

			btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, _motionState,nullptr, temp);
			constructionInfo.m_linearDamping = material.damping;
			constructionInfo.m_angularDamping = material.angularDamping;
			_rigidBody = new btRigidBody(constructionInfo);
		}
		//Add the rigidBody to the physics world
		scene.physics.AddBulletBody(_rigidBody);
	}

	void RigidBody::OnEnd(Scene& scene)
	{
		delete _motionState;
		delete _rigidBody;
	}

	void RigidBody::SetBulletRotation(Maths::Quaternion rotation)
	{
		btQuaternion quaternion = Physics::ConvertQuaternion(rotation);
		GetWorldTransform().setRotation(quaternion);
	}

	void RigidBody::SetBulletPosition(Maths::Vec3 position)
	{
		btVector3 translation = Physics::ConvertVector(position);
		GetWorldTransform().setOrigin(translation);
	}

	void RigidBody::OnUpdate(Scene& scene, double deltaTime)
	{
		{//set the rigidBody status
			auto temp = Physics::ConvertVector(_inertia);

			if (GetComponent<BoxCollider>() != nullptr)
			{
				_rigidBody->setCollisionShape(GetComponent<BoxCollider>()->GetShape());
				_rigidBody->getCollisionShape()->calculateLocalInertia(mass, temp);
			}
			else if ( GetComponent<CapsuleCollider>() != nullptr)
			{
				_rigidBody->setCollisionShape( GetComponent<CapsuleCollider>()->GetShape());
				_rigidBody->getCollisionShape()->calculateLocalInertia(mass, temp);
			}
			else if (GetComponent<SphereCollider>() != nullptr)
			{
				_rigidBody->setCollisionShape(GetComponent<SphereCollider>()->GetShape());
				_rigidBody->getCollisionShape()->calculateLocalInertia(mass, temp);
			}
			_rigidBody->setRestitution(material.bounciness);
			_rigidBody->setFriction(material.friction);
		}

		{//Set the transform component to the bullet world transform
			Maths::Quaternion rotation = GetComponent<Transform>()->localRotation;
			Maths::Vec3 position = GetComponent<Transform>()->localPosition;

			GetComponent<RigidBody>()->SetBulletPosition(position);
			GetComponent<RigidBody>()->SetBulletRotation(rotation);
		}
	}
}

