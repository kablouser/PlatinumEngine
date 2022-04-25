//
// Created by shawn on 2022/4/22.
//

#pragma once

#include <ComponentComposition/Component.h>
#include <ComponentComposition/BoxCollider.h>
#include <ComponentComposition/SphereCollider.h>
#include <ComponentComposition/CapsuleCollider.h>

#include <Physics/Physics.h>

namespace PlatinumEngine
{
	class RigidBody: public Component
	{
	public:
		RigidBody(Physics* physics);

		//set up the rigidBody, called after physics world set up
		void Initialize(btCollisionShape* shape);

		//Setters
		void SetMass(float mass);
		void SetForce(Maths::Vec3 force);
		void SetKinematic(bool value);

		//Getters
		float GetMass();
		Maths::Vec3 GetForce();
		bool GetKinematic();

		btTransform GetWorldTransform();
		Maths::Vec3 GetBulletRotation();
		Maths::Vec3 GetBulletPosition();


	private:
		Maths::Vec3 _inertia;
		Maths::Vec3 _linearVelocity;
		Maths::Vec3 _angularVelocity;

		bool _kinematic;
		float _mass;

		btRigidBody* _rigidBody;
		Collider* collider;
		Physics* _physics;
	};
}