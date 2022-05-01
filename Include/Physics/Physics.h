//
// Created by shawn on 2022/4/22.
//

#pragma once

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include <ComponentComposition/GameObject.h>
#include <ComponentComposition/Collider.h>
#include <Maths/Vectors.h>
#include <Maths/Quaternion.h>
#include <Helpers/Time.h>

#include <vector>

namespace PlatinumEngine
{
	struct PhysicalMaterial
	{
	public:
		//between 0 and 1
		float friction;
		float bounciness;
		float rollingFriction;
		float spinningFriction;
		float damping;
		float angularDamping;

		PhysicalMaterial(): friction(0.f),
							bounciness(1.f),
							rollingFriction(0.f),
							spinningFriction(0.f),
							damping(0.f),
							angularDamping(0.f)
		{

		};
		~PhysicalMaterial() {};
	};

	class Physics
	{
	public:
		Physics();
		~Physics();

		//Update the physics world by delta time
		void Update(double time);

		void SetGravity(float gravity);

		float GetGravity();

		//helper function helps to convert vectors, quaternions and scalars from bullet to us or us to bullet
		static btQuaternion ConvertEulerToQuaternion(const btVector3& vector);
		static Maths::Quaternion ConvertQuaternionBack(const btQuaternion& quaternion);
		static btQuaternion ConvertQuaternion(const Maths::Quaternion& quaternion);
		static Maths::Vec3 ConvertVectorBack(const btVector3& vector);
		static btVector3  ConvertVector(Maths::Vec3 vector);


		//Add rigidbody enabled gameobject to the list of updated objects
		void AddRigidBody(GameObject* gameObject);
		void AddBulletBody(btRigidBody* rigidBody);
		void RemoveBulletBody(btRigidBody* rigidBody);

		//initialize and cleanup the bullet physics world
		void Initialize();
		void CleanUp();

	private://paramters and objects
		//bullet init objects
		btBroadphaseInterface* _broadPhase;
		btDefaultCollisionConfiguration* _config;
		btCollisionDispatcher* _dispatcher;
		btSequentialImpulseConstraintSolver* _solver;
		btDiscreteDynamicsWorld* _bulletWorld;

		//Holds a list of gameobject pointer that need to be updated
		std::vector<GameObject*> _physicsObjects;

		//Holds the current gravity value
		float _gravity;
	};
}
