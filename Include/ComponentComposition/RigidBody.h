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
		friend class Physics;

		static void CreateTypeInfo(TypeDatabase& database);

		//Constructor
		RigidBody();

		// Get the WorldTransform matrix
		btTransform GetWorldTransform() const;

		// Get the Bullet RigidBody Translation and Rotation Vector
		std::pair<Maths::Vec3, Maths::Quaternion> GetPositionRotation() const;

		// Set the rigidBody when it starts
		void OnEnable(Scene& scene) override;
		// Clean up all bullet pointers
		void OnDisable(Scene& scene) override;

		bool isKinematic; //kinematic
		float mass; // mass
		PlatinumEngine::PhysicsMaterial physicalMaterial;

	private:
		// bullet objects
		btDefaultMotionState _motionState;
		btRigidBody _rigidBody;
		// dummy shape to keep Bullet happy until a real shape is detected
		btEmptyShape _emptyShape;
	};
}