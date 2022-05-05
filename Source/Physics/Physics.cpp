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
	}

	void Physics::SetGravity(Maths::Vec3 gravity)
	{
		_bulletWorld.setGravity(ConvertVector(gravity));
	}

	Maths::Vec3 Physics::GetGravity() const
	{
		return ConvertVectorBack(_bulletWorld.getGravity());
	}
}