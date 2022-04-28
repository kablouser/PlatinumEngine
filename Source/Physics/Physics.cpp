//
// Created by shawn on 2022/4/22.
//

#include "Physics/Physics.h"


#include <ComponentComposition/RigidBody.h>
#include <ComponentComposition/BoxCollider.h>
#include <ComponentComposition/SphereCollider.h>
#include <ComponentComposition/CapsuleCollider.h>
#include <ComponentComposition/Transform.h>

#include <Logger/Logger.h>

namespace PlatinumEngine
{
	Physics::Physics():_gravity(9.81f) {};

	Physics::~Physics() {};

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
		SetGravity(_gravity);
	}

	//clean up all the bullet object pointer
	void Physics::CleanUp()
	{
		delete _bulletWorld;
		delete _solver;
		delete _dispatcher;
		delete _config;
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

	//Render the bulletWorld
	void Physics::Update(double time)
	{
		PLATINUM_INFO("Render");
		_bulletWorld->stepSimulation((btScalar)time, 10);

		//All physics rigidbodys need there updated position to refelect what bullet decides
		for (auto & _physicsObject : _physicsObjects)
		{
			//The actual position is a combination of the rigidbodys position and the transform position
			_physicsObject->GetComponent<Transform>()->localPosition = _physicsObject->GetComponent<RigidBody>()->GetBulletPosition();
			_physicsObject->GetComponent<Transform>()->localRotation = Maths::Quaternion(_physicsObject->GetComponent<RigidBody>()->GetBulletRotation().x,
																							  _physicsObject->GetComponent<RigidBody>()->GetBulletRotation().y,
																							  _physicsObject->GetComponent<RigidBody>()->GetBulletRotation().z);
		}
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
		return {vector.getX(), vector.getY(), vector.getZ()};
	}

	btQuaternion Physics::convertEulerToQuaternion(const btVector3& vector)
	{
		return btQuaternion(vector.getX(), vector.getY(), vector.getZ());
	}
	Maths::Quaternion Physics::convertVQuaternionBack(const btQuaternion& quaternion)
	{
		return {quaternion.getW(), quaternion.getX(), quaternion.getY(), quaternion.getZ()};
	}

	btQuaternion Physics::convertQuaternion(const Maths::Quaternion& quaternion)
	{
		return btQuaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	}

}