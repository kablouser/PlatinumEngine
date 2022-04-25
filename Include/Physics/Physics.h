//
// Created by shawn on 2022/4/22.
//

#pragma once

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include <ComponentComposition/GameObject.h>

#include <Maths/Vectors.h>
#include <Maths/Quaternion.h>

#include <vector>

namespace PlatinumEngine
{
	class Physics
	{
	public:
		Physics();
		~Physics();

		void Update();

		void SetGravity(float gravity);

		float GetGravity();

		//helper function helps to convert vectors, quaternions and scalars from bullet to us or us to bullet
		static btQuaternion convertEulerToQuaternion(const btVector3& vector);
		static Maths::Quaternion convertVQuaternionBack(const btQuaternion& quaternion);
		static Maths::Vec3 convertVectorBack(const btVector3& vector);
		static btVector3  convertVector(Maths::Vec3 vector);


		//Add rigidbody enabled gameobject to the list of updated objects
		void AddRigidBody(GameObject* gameObject);
		void AddBulletBody(btRigidBody* rigidBody);
		void RemoveBulletBody(btRigidBody* rigidBody);

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

	private://functions

		//initialize and cleanup the bullet physics world
		void Initialize();
		void CleanUp();
	};
}
