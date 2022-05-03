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

			if (GetComponent<BoxCollider>() != nullptr)
			{
				GetComponent<BoxCollider>()->GetShape()->calculateLocalInertia(mass, temp);
				btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, _motionState,
						GetComponent<BoxCollider>()->GetShape(), temp);
				constructionInfo.m_restitution = material.bounciness;
				constructionInfo.m_friction = material.friction;
				constructionInfo.m_linearDamping = material.damping;
				constructionInfo.m_angularDamping = material.angularDamping;
				_rigidBody = new btRigidBody(constructionInfo);

			}
			else if (GetComponent<SphereCollider>() != nullptr)
			{
				GetComponent<SphereCollider>()->GetShape()->calculateLocalInertia(mass, temp);
				btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, _motionState,
						GetComponent<SphereCollider>()->GetShape(), temp);
				constructionInfo.m_restitution = material.bounciness;
				constructionInfo.m_friction = material.friction;
				constructionInfo.m_linearDamping = material.damping;
				constructionInfo.m_angularDamping = material.angularDamping;
				_rigidBody = new btRigidBody(constructionInfo);
			}
			else if (GetComponent<CapsuleCollider>() != nullptr)
			{
				GetComponent<CapsuleCollider>()->GetShape()->calculateLocalInertia(mass, temp);
				btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, _motionState,
						GetComponent<CapsuleCollider>()->GetShape(), temp);
				constructionInfo.m_restitution = material.bounciness;
				constructionInfo.m_friction = material.friction;
				constructionInfo.m_linearDamping = material.damping;
				constructionInfo.m_angularDamping = material.angularDamping;
				_rigidBody = new btRigidBody(constructionInfo);
			}
		}

		//Add the rigidBody to the physics world
		scene.physics.AddBulletBody(_rigidBody);
	}

	void RigidBody::OnEnd(Scene& scene)
	{
		delete _motionState;
		scene.physics.RemoveBulletBody(_rigidBody);
		delete _rigidBody;
	}

	void RigidBody::OnUpdate(Scene& scene, double deltaTime)
	{
		auto transform = GetComponent<Transform>();
		btTransform worldTransform;
		worldTransform.setOrigin(Physics::ConvertVector(transform->localPosition));
		worldTransform.setRotation(Physics::ConvertQuaternion(transform->localRotation));

		_rigidBody->setWorldTransform(worldTransform);
		_rigidBody->getMotionState()->setWorldTransform(worldTransform);
	}
}

