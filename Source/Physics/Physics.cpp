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
		int rigidBodyIndex = 0;
		for (SavedReference<PlatinumEngine::RigidBody>& rigidBody: _allRigidBodies)
		{
			if (!rigidBody)
				continue;
			RigidBody* rigidBodyPointer = rigidBody.DeRef().get();

			if (!rigidBodyPointer->_addedToPhysicsWorld)
				continue; // if not added, it's not simulated

			SavedReference<Transform> transform = rigidBodyPointer->GetComponent<Transform>();
			if (!transform)
				continue;
			Transform* transformPointer = transform.DeRef().get();

			btTransform worldTransform;
			worldTransform.setOrigin(Physics::ConvertVector(transformPointer->localPosition));
			worldTransform.setRotation(Physics::ConvertQuaternion(transformPointer->localRotation));
			rigidBodyPointer->_rigidBody.setWorldTransform(worldTransform);
			// this is used below for the collision recording
			rigidBodyPointer->_rigidBody.setUserIndex(1 + rigidBodyIndex); // 1 based-index, so nullable
			++rigidBodyIndex;

			// clean state for new collision recordings
			rigidBodyPointer->_collisionRecords.clear();
		}

		_bulletWorld.stepSimulation((btScalar)time, 1);

		// All physics rigidBody need there updated position to reflect what bullet decides
		// Write data from bullet back into our data
		for (SavedReference<PlatinumEngine::RigidBody>& rigidBody: _allRigidBodies)
		{
			if (!rigidBody)
				continue;
			RigidBody* rigidBodyPointer = rigidBody.DeRef().get();

			if (!rigidBodyPointer->_addedToPhysicsWorld)
				continue; // if not added, it's not simulated

			SavedReference<Transform> transform = rigidBodyPointer->GetComponent<Transform>();
			if (!transform)
				continue;
			Transform* transformPointer = transform.DeRef().get();

			auto[position, rotation] = rigidBodyPointer->GetPositionRotation();
			transformPointer->localPosition = position;
			transformPointer->localRotation = rotation;
		}

		// Collision recording
		int numManifolds = _dispatcher.getNumManifolds();
		for (int manifoldIndex = 0; manifoldIndex < numManifolds; ++manifoldIndex)
		{
			btPersistentManifold *contactManifold = _dispatcher.getManifoldByIndexInternal(manifoldIndex);
			const btCollisionObject *btRigidBody0 = contactManifold->getBody0();
			const btCollisionObject *btRigidBody1 = contactManifold->getBody1();

			int rigidBodyIndex0 = btRigidBody0->getUserIndex() - 1; // userIndex is 1 based-index, convert back
			int rigidBodyIndex1 = btRigidBody1->getUserIndex() - 1;

			if (rigidBodyIndex0 == -1 || rigidBodyIndex1 == -1)
			{
				PLATINUM_WARNING("Untracked RigidBody in simulation");
				continue;
			}

			SavedReference<RigidBody> rigidBody0 = _allRigidBodies[rigidBodyIndex0];
			SavedReference<RigidBody> rigidBody1 = _allRigidBodies[rigidBodyIndex1];

			RigidBody* rigidBodyPointer0 = rigidBody0.DeRef().get();
			RigidBody* rigidBodyPointer1 = rigidBody1.DeRef().get();

			if (!rigidBodyPointer0->isCollisionRecorded && !rigidBodyPointer1->isCollisionRecorded)
				continue;

			int numContacts = contactManifold->getNumContacts();
			for (int contactIndex = 0; contactIndex < numContacts; ++contactIndex)
			{
				btManifoldPoint& manifoldPoint = contactManifold->getContactPoint(contactIndex);

				Maths::Vec3 contactPoint0 = ConvertVectorBack(manifoldPoint.m_positionWorldOnA);
				Maths::Vec3 contactPoint1 = ConvertVectorBack(manifoldPoint.m_positionWorldOnB);

				if (rigidBodyPointer0->isCollisionRecorded)
					rigidBodyPointer0->_collisionRecords.push_back({ rigidBody1, contactPoint0, contactPoint1 });
				if (rigidBodyPointer1->isCollisionRecorded)
					rigidBodyPointer1->_collisionRecords.push_back({ rigidBody0, contactPoint1, contactPoint0 });
			}
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

	std::vector<SavedReference<RigidBody>>& Physics::GetRigidBodies()
	{
		return _allRigidBodies;
	}
}