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

		//Reposition function
		void Reposition(Maths::Vec3 position, Maths::Quaternion rotation);

		// Get the WorldTransform matrix
		btTransform GetWorldTransform() const;

		// Get the Bullet RigidBody Translation and Rotation Vector
		std::pair<Maths::Vec3, Maths::Quaternion> GetPositionRotation() const;

		// Set the rigidBody when it starts
		void OnEnable(Scene& scene) override;
		// Clean up all bullet pointers
		void OnDisable(Scene& scene) override;
		// Removes btRigidBody from the physics world. Reconstructs it. Adds it back if there's a transform and collider.
		// btRigidBody loses all velocities and some other stuff.
		void UpdatePhysicsProperties(Physics& physics);

		// These fields are only used to construct btRigidBody.
		// They don't change the btRigidBody while it's simulating.
		bool isKinematic;
		float mass; // min=00001f, because static is not allowed
		PlatinumEngine::PhysicsMaterial physicsMaterial;

		Maths::Vec3 initialPosition;
		Maths::Quaternion initialRotation;



	private:
		// bullet objects
		btRigidBody _rigidBody;
		// has btRigidBody been added to the bullet physics world?
		bool _addedToPhysicsWorld;

		void AddToPhysicsWorld(Physics& physics);
		void RemoveFromPhysicsWorld(Physics& physics);
	};
}