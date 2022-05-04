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
		//Constructor
		RigidBody();

		btTransform GetWorldTransform(); // Get the WorldTransform matrix
		Maths::Quaternion GetBulletRotation(); // Get the Bullet RigidBody Rotation Vector
		Maths::Vec3 GetBulletPosition(); // Get the Bullet RigidBody Translation Vector

		static void CreateTypeInfo(TypeDatabase& database);
	public:
		bool  kinematic; //kinematic
		float mass; // mass

		PhysicalMaterial physicalMaterial;
	public:
		// Set the rigidBody when it starts
		void OnStart(Scene& scene) override;

		// Clean up all bullet pointers
		void OnEnd(Scene& scene) override;

		void OnUpdate(Scene& scene, double deltaTime) override;
	private:

		//pointer of bullet objects
		btRigidBody* _rigidBody;
		btDefaultMotionState* _motionState;
	};
}