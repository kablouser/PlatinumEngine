//
// Created by shawn on 2022/4/22.
//

#include <Physics/Physics.h>

#include <ComponentComposition/Transform.h>
#include <ComponentComposition/RigidBody.h>

#include <Helpers/VectorHelpers.h>

#include <Logger/Logger.h>

namespace PlatinumEngine
{
	PhysicsMaterial::PhysicsMaterial() :
			friction(0.f),
			bounciness(1.f),
			damping(0.f),
			angularDamping(0.f)
	{
	};

	//--------------------------------------------------------------------------------------------------------------
	// helper function helps to convert vectors, quaternions and scalars from bullet to us or us to bullet
	//--------------------------------------------------------------------------------------------------------------

	btVector3 Physics::ConvertVector(Maths::Vec3 vector)
	{
		return btVector3(vector.x, vector.y, vector.z);
	}

	Maths::Vec3 Physics::ConvertVectorBack(const btVector3& vector)
	{
		return { vector.getX(), vector.getY(), vector.getZ() };
	}

	btQuaternion Physics::ConvertEulerToQuaternion(const btVector3& vector)
	{
		return btQuaternion(vector.getX(), vector.getY(), vector.getZ());
	}

	Maths::Quaternion Physics::ConvertQuaternionBack(const btQuaternion& quaternion)
	{
		return { quaternion.getW(), quaternion.getX(), quaternion.getY(), quaternion.getZ() };
	}

	btQuaternion Physics::ConvertQuaternion(const Maths::Quaternion& quaternion)
	{
		return btQuaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	}

	//--------------------------------------------------------------------------------------------------------------
	// Physics
	//--------------------------------------------------------------------------------------------------------------

	Physics::Physics() :
			_dispatcher(&_config),
			_bulletWorld(&_dispatcher, &_broadPhase, &_solver, &_config)
	{
		SetGravity();
	};

	Physics::~Physics() = default;

	// Update the bulletWorld
	void Physics::Update(double time)
	{
		// Write our data into bullet
		for (SavedReference<PlatinumEngine::RigidBody>& rigidBody: _allRigidBodies)
		{
			if (!rigidBody)
				continue;
			RigidBody* rigidBodyPointer = rigidBody.DeRef().get();

			SavedReference<Transform> transform = rigidBodyPointer->GetComponent<Transform>();
			if (!transform)
				continue;
			Transform* transformPointer = transform.DeRef().get();

			// Polymorphic. Can be BoxCollider, CapsuleCollider or SphereCollider.
			SavedReference<Collider> collider = rigidBodyPointer->GetComponent<Collider>();
			btCollisionShape* btCollisionShape = nullptr;
			if (collider)
				btCollisionShape = collider.DeRef()->GetShape();
			else
				btCollisionShape = &rigidBodyPointer->_emptyShape;

			// Position and rotation
			btTransform worldTransform;
			worldTransform.setOrigin(Physics::ConvertVector(transformPointer->localPosition));
			worldTransform.setRotation(Physics::ConvertQuaternion(transformPointer->localRotation));
//			rigidBodyPointer->_motionState.setWorldTransform(worldTransform);
//			rigidBodyPointer->_rigidBody.setWorldTransform(worldTransform);
//			rigidBodyPointer->_rigidBody.setMotionState(&rigidBodyPointer->_motionState);

			// setCollisionShape uses m_updateRevision, keep calls to a minimum
			if (btCollisionShape != rigidBodyPointer->_rigidBody.getCollisionShape())
				rigidBodyPointer->_rigidBody.setCollisionShape(btCollisionShape);

//			if (rigidBodyPointer->isKinematic)
//				// Add flag
//				rigidBodyPointer->_rigidBody.setCollisionFlags(
//						rigidBodyPointer->_rigidBody.getCollisionFlags() |
//						btCollisionObject::CF_KINEMATIC_OBJECT);
//			else
//				// Remove flag
//				rigidBodyPointer->_rigidBody.setCollisionFlags(
//						rigidBodyPointer->_rigidBody.getCollisionFlags() &
//						~btCollisionObject::CF_KINEMATIC_OBJECT);

			// mass, friction, bounciness uses m_updateRevision,
			// which means we need to call the set functions as little as possible
			if (rigidBodyPointer->mass != rigidBodyPointer->_rigidBody.getMass())
				rigidBodyPointer->_rigidBody.setMassProps(
						rigidBodyPointer->mass,
						rigidBodyPointer->_rigidBody.getLocalInertia());
			if (rigidBodyPointer->physicalMaterial.friction != rigidBodyPointer->_rigidBody.getFriction())
				rigidBodyPointer->_rigidBody.setFriction(rigidBodyPointer->physicalMaterial.friction);
			if (rigidBodyPointer->physicalMaterial.bounciness != rigidBodyPointer->_rigidBody.getRestitution())
				rigidBodyPointer->_rigidBody.setRestitution(rigidBodyPointer->physicalMaterial.bounciness);

			// setDamping has no m_updateRevision
			rigidBodyPointer->_rigidBody.setDamping(
					rigidBodyPointer->physicalMaterial.damping,
					rigidBodyPointer->physicalMaterial.angularDamping);
		}

		_bulletWorld.stepSimulation((btScalar)time, 1);

		// All physics rigidBody need there updated position to reflect what bullet decides
		// Write data from bullet back into our data
		for (SavedReference<PlatinumEngine::RigidBody>& rigidBody: _allRigidBodies)
		{
			if (!rigidBody)
				continue;
			RigidBody* rigidBodyPointer = rigidBody.DeRef().get();

			SavedReference<Transform> transform = rigidBodyPointer->GetComponent<Transform>();
			if (!transform)
				continue;
			Transform* transformPointer = transform.DeRef().get();

			auto[position, rotation] = rigidBodyPointer->GetPositionRotation();
			transformPointer->localPosition = position;
			transformPointer->localRotation = rotation;
		}
	}

	void Physics::SetGravity(Maths::Vec3 gravity)
	{
		_bulletWorld.setGravity(ConvertVector(gravity));
	}

	Maths::Vec3 Physics::GetGravity() const
	{
		return ConvertVectorBack(_bulletWorld.getGravity());
	}

	void Physics::RigidBodyOnBegin(SavedReference<PlatinumEngine::RigidBody> rigidBody)
	{
		_allRigidBodies.push_back(rigidBody);
		_bulletWorld.addRigidBody(&rigidBody.DeRef()->_rigidBody);
	}

	void Physics::RigidBodyOnEnd(SavedReference<PlatinumEngine::RigidBody> rigidBody)
	{
		if (!VectorHelpers::RemoveFirst(_allRigidBodies, rigidBody))
			PLATINUM_WARNING("Missing RigidBody");

		_bulletWorld.removeRigidBody(&rigidBody.DeRef()->_rigidBody);
	}
}