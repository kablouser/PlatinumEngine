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
		void Reposition(const Maths::Vec3& position, const Maths::Quaternion& rotation);

		// Get the WorldTransform matrix
		btTransform GetWorldTransform() const;

		// Get the Bullet RigidBody Translation and Rotation Vector
		std::pair<Maths::Vec3, Maths::Quaternion> GetPositionRotation() const;

		// Set the rigidBody when it starts
		void OnEnable() override;
		// Clean up all bullet pointers
		void OnDisable() override;
		// Removes btRigidBody from the physics world. Reconstructs it. Adds it back if there's a transform and collider.
		// btRigidBody loses all velocities and some other stuff.
		void UpdatePhysicsProperties();

		void SetVelocity(Maths::Vec3 velocity);
		Maths::Vec3 GetVelocity() const;

		void SetAngularVelocity(Maths::Vec3 angularVelocity);
		Maths::Vec3 GetAngularVelocity() const;

		const std::vector<Collision>& GetCollisionRecords() const;

		// These fields are only used to construct btRigidBody.
		// They don't change the btRigidBody while it's simulating.
		bool isKinematic;
		float mass; // min=00001f, because static is not allowed
		PlatinumEngine::PhysicsMaterial physicsMaterial;

		bool isCollisionRecorded;

	private:
		// bullet objects
		btRigidBody _rigidBody;
		btDefaultMotionState _motionState;
		// has btRigidBody been added to the bullet physics world?
		bool _addedToPhysicsWorld;
		// collisions from last frame
		std::vector<Collision> _collisionRecords;

		void AddToPhysicsWorld();
		void RemoveFromPhysicsWorld();
	};
}