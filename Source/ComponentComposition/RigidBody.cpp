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
					btTransform(Physics::ConvertQuaternion(objectTransform.DeRef()->localRotation),
							Physics::ConvertVector(objectTransform.DeRef()->localPosition)));
		}
		//Set the btRigidBody based on its collider type
		{
			auto temp = Physics::ConvertVector(_inertia);

			if (GetComponent<BoxCollider>().DeRef() != nullptr)
			{
				GetComponent<BoxCollider>().DeRef()->GetShape()->calculateLocalInertia(mass, temp);
				btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, _motionState,
						GetComponent<BoxCollider>().DeRef()->GetShape(), temp);
				constructionInfo.m_restitution = physicalMaterial.bounciness;
				constructionInfo.m_friction = physicalMaterial.friction;
				constructionInfo.m_linearDamping = physicalMaterial.damping;
				constructionInfo.m_angularDamping = physicalMaterial.angularDamping;
				_rigidBody = new btRigidBody(constructionInfo);

			}
			else if (GetComponent<SphereCollider>().DeRef() != nullptr)
			{
				GetComponent<SphereCollider>().DeRef()->GetShape()->calculateLocalInertia(mass, temp);
				btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, _motionState,
						GetComponent<SphereCollider>().DeRef()->GetShape(), temp);
				constructionInfo.m_restitution = physicalMaterial.bounciness;
				constructionInfo.m_friction = physicalMaterial.friction;
				constructionInfo.m_linearDamping = physicalMaterial.damping;
				constructionInfo.m_angularDamping = physicalMaterial.angularDamping;
				_rigidBody = new btRigidBody(constructionInfo);
			}
			else if (GetComponent<CapsuleCollider>().DeRef() != nullptr)
			{
				GetComponent<CapsuleCollider>().DeRef()->GetShape()->calculateLocalInertia(mass, temp);
				btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, _motionState,
						GetComponent<CapsuleCollider>().DeRef()->GetShape(), temp);
				constructionInfo.m_restitution = physicalMaterial.bounciness;
				constructionInfo.m_friction = physicalMaterial.friction;
				constructionInfo.m_linearDamping = physicalMaterial.damping;
				constructionInfo.m_angularDamping = physicalMaterial.angularDamping;
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
		worldTransform.setOrigin(Physics::ConvertVector(transform.DeRef()->localPosition));
		worldTransform.setRotation(Physics::ConvertQuaternion(transform.DeRef()->localRotation));

		_rigidBody->setWorldTransform(worldTransform);
		_rigidBody->getMotionState()->setWorldTransform(worldTransform);
	}

	void RigidBody::CreateTypeInfo(TypeDatabase& database)
	{
			database.BeginTypeInfo<RigidBody>()
			        .WithInherit<Component>()
					.WithField<PhysicalMaterial>("physicalMaterial", PLATINUM_OFFSETOF(RigidBody, physicalMaterial))
					.WithField<float>("mass", PLATINUM_OFFSETOF(RigidBody, mass))
					.WithField<bool>("kinematic", PLATINUM_OFFSETOF(RigidBody, kinematic))
					.WithField<Maths::Vec3>("inertia", PLATINUM_OFFSETOF(RigidBody, _inertia))
					.WithField<Maths::Vec3>("linearVelocity", PLATINUM_OFFSETOF(RigidBody, _linearVelocity))
					.WithField<Maths::Vec3>("_angularVelocity", PLATINUM_OFFSETOF(RigidBody, _angularVelocity));
	}
}

