//
// Created by 肖宜辰 on 2022/4/22.
//

#include "Physics/Physics.h"
#include <Helpers/Time.h>

#include <ComponentComposition/RigidBody.h>
#include <ComponentComposition/BoxCollider.h>
#include <ComponentComposition/SphereCollider.h>
#include <ComponentComposition/CapsuleCollider.h>

#include <Logger/Logger.h>

namespace PlatinumEngine
{
	Physics::Physics() { Initialize(); };

	Physics::~Physics() { CleanUp(); };

	//initialize all bullet class objects and set bullet physics world
	void Physics::Initialize()
	{
		//initialize the bullet physics broadphase which
		//bullet will remove the objects that can not collide able
		_broadPhase = new btDbvtBroadphase();

		//Use defaults
		_config = new btDefaultCollisionConfiguration();
		_dispatcher = new btCollisionDispatcher(_config);

		//the solver allows objects to interact properly
		_solver = new btSequentialImpulseConstraintSolver;

		_bulletWorld = new btDiscreteDynamicsWorld(_dispatcher, _broadPhase, _solver, _config);
		SetGravity(9.81f);
	}

	//clean up all the bullet object pointer
	void Physics::CleanUp()
	{
		delete _bulletWorld;
		delete _solver;
		delete _config;
		delete _dispatcher;
		delete _broadPhase;
	}

	//set the world gravity
	void Physics::SetGravity(float gravity)
	{
		_gravity = gravity;

		_bulletWorld->setGravity(convertVector(Maths::Vec3(0, -gravity, 0)));
	}

	//Get gravity
	float Physics::GetGravity()
	{
		return _gravity;
	}

	//Update the
	void Physics::Update()
	{
		_bulletWorld->stepSimulation(Time.getDelta(), 10);
	}

	void Physics::AddRigidBody(GameObject* gameObject)
	{

		if(gameObject->GetComponent<RigidBody>() != nullptr &&
				(gameObject->GetComponent<BoxCollider>() !=nullptr ||
				gameObject->GetComponent<CapsuleCollider>() !=nullptr ||
				gameObject->GetComponent<SphereCollider>() !=nullptr))
		{
			_physicsObjects.emplace_back(gameObject);
		}
		else
		{
			PLATINUM_WARNING("Object does not have a collider or rigidBody - ignored.");
		}

	}

	void Physics::AddBulletBody(btRigidBody* rigidBody)
	{
		_bulletWorld->addRigidBody(rigidBody);
	}

	void Physics::RemoveBulletBody(btRigidBody* rigidBody)
	{
		_bulletWorld->removeRigidBody(rigidBody);
	}

	//convertor from the bt Math to our Math things
	btVector3 Physics::convertVector(Maths::Vec3 vector)
	{
		return btVector3(vector.x, vector.y, vector.z);
	}
	Maths::Vec3 Physics::convertVectorBack(const btVector3& vector)
	{
		return Maths::Vec3(vector.getX(), vector.getY(), vector.getZ());
	}

	btQuaternion Physics::convertEulerToQuaternion(const btVector3& vector)
	{
		return btQuaternion(vector.getX(), vector.getY(), vector.getZ());
	}
	Maths::Quaternion Physics::convertVQuaternionBack(const btQuaternion& quaternion)
	{
		return Maths::Quaternion(quaternion.getW(), quaternion.getX(), quaternion.getY(), quaternion.getZ());
	}

	btScalar Physics::convertScalar(float scalar)
	{
		return (btScalar)scalar;
	}
	float Physics::convertScalarBack(const btScalar& scalar)
	{
		return (float)scalar;
	}

}