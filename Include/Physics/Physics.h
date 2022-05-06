//
// Created by shawn on 2022/4/22.
//

#pragma once

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include <Maths/Vectors.h>
#include <Maths/Quaternion.h>
#include <IDSystem/IDSystem.h>

#include <vector>

namespace PlatinumEngine
{
	struct PhysicsMaterial
	{
		//between 0 and 1
		float friction;
		float bounciness;
		float damping;
		float angularDamping;

		PhysicsMaterial();
	};

	// Can't include RigidBody because circular dependencies.
	class RigidBody;

	class Physics
	{
	public:

		friend class RigidBody;

		//--------------------------------------------------------------------------------------------------------------
		// helper function helps to convert vectors, quaternions and scalars from bullet to us or us to bullet
		//--------------------------------------------------------------------------------------------------------------
		static btQuaternion ConvertEulerToQuaternion(const btVector3& vector);
		static Maths::Quaternion ConvertQuaternionBack(const btQuaternion& quaternion);
		static btQuaternion ConvertQuaternion(const Maths::Quaternion& quaternion);
		static Maths::Vec3 ConvertVectorBack(const btVector3& vector);
		static btVector3  ConvertVector(Maths::Vec3 vector);

		Physics();
		~Physics();

		//Update the physics world by delta time
		void Update(double time);

		void SetGravity(Maths::Vec3 gravity = {0.f, -9.81f, 0.f});
		Maths::Vec3 GetGravity() const;

		std::vector<SavedReference<PlatinumEngine::RigidBody>>& GetRigidBodies();


	private:

		//parameters and objects
		//bullet init objects
		btDefaultCollisionConfiguration _config;
		btCollisionDispatcher _dispatcher;
		btDbvtBroadphase _broadPhase;
		btSequentialImpulseConstraintSolver _solver;
		btDiscreteDynamicsWorld _bulletWorld;

		// RigidBodies here aren't necessarily in the physics world. They are just the active rigidBodies in the scene.
		std::vector<SavedReference<RigidBody>> _allRigidBodies;
	};
}
