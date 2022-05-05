//
// Created by shawn on 2022/4/22.
//

#include <ComponentComposition/RigidBody.h>
#include <ComponentComposition/Transform.h>
#include <Logger/Logger.h>
#include <SceneManager/Scene.h>

namespace PlatinumEngine
{
	void RigidBody::CreateTypeInfo(TypeDatabase& database)
	{
		database.BeginTypeInfo<RigidBody>()
				.WithInherit<Component>()
				.WithField<PhysicsMaterial>("physicsMaterial", PLATINUM_OFFSETOF(RigidBody, physicsMaterial))
				.WithField<float>("mass", PLATINUM_OFFSETOF(RigidBody, mass))
				.WithField<bool>("isKinematic", PLATINUM_OFFSETOF(RigidBody, isKinematic));
	}

	RigidBody::RigidBody() :
			mass(1.f),
			isKinematic(false),
			_rigidBody(btRigidBody::btRigidBodyConstructionInfo(mass, nullptr, nullptr, {})),
			_addedToPhysicsWorld(false)
	{
	}

	btTransform RigidBody::GetWorldTransform() const
	{
		return _rigidBody.getWorldTransform();
	}

	std::pair<Maths::Vec3, Maths::Quaternion> RigidBody::GetPositionRotation() const
	{
		btTransform worldTransform = GetWorldTransform();
		return {
				Physics::ConvertVectorBack(worldTransform.getOrigin()),
				Physics::ConvertQuaternionBack(worldTransform.getRotation()) };
	}

	void RigidBody::OnEnable(Scene& scene)
	{
		//Add the rigidBody to the physics manager thing
		scene.physics._allRigidBodies.push_back(scene.idSystem.GetSavedReference<RigidBody>(this));
		UpdatePhysicsProperties(scene.physics);
	}

	void RigidBody::OnDisable(Scene& scene)
	{
		RemoveFromPhysicsWorld(scene.physics);
		if (!VectorHelpers::RemoveFirst(scene.physics._allRigidBodies, scene.idSystem.GetSavedReference<RigidBody>(this)))
			PLATINUM_WARNING("Missing RigidBody from Physics Manager thing");
	}

	void RigidBody::UpdatePhysicsProperties(Physics& physics)
	{
		RemoveFromPhysicsWorld(physics);

		SavedReference<Transform> transform = GetComponent<Transform>();
		if (!transform)
			return;

		// Polymorphic. Can be BoxCollider, CapsuleCollider or SphereCollider.
		SavedReference<Collider> collider = GetComponent<Collider>();
		if (!collider)
			return;

		Transform* transformPointer = transform.DeRef().get();
		btTransform worldTransform;
		worldTransform.setOrigin(Physics::ConvertVector(transformPointer->localPosition));
		worldTransform.setRotation(Physics::ConvertQuaternion(transformPointer->localRotation));

		btCollisionShape* btCollisionShape = collider.DeRef()->GetShape();

		// Important, don't forget to calculate inertia from the shape.
		btVector3 inertia;
		btCollisionShape->calculateLocalInertia(mass, inertia);
		btRigidBody::btRigidBodyConstructionInfo constructInfo(mass, nullptr, btCollisionShape, inertia);
		constructInfo.m_startWorldTransform = worldTransform;
		constructInfo.m_friction = physicsMaterial.friction;
		constructInfo.m_restitution = physicsMaterial.bounciness;
		constructInfo.m_linearDamping = physicsMaterial.damping;
		constructInfo.m_angularDamping = physicsMaterial.angularDamping;
		_rigidBody = btRigidBody(constructInfo);
		if (isKinematic)
			_rigidBody.setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);

		AddToPhysicsWorld(physics);
	}

	void RigidBody::AddToPhysicsWorld(Physics& physics)
	{
		if (_addedToPhysicsWorld)
			return; // avoid repeats
		physics._bulletWorld.addRigidBody(&_rigidBody);
		_addedToPhysicsWorld = true;
	}

	void RigidBody::RemoveFromPhysicsWorld(Physics& physics)
	{
		if (!_addedToPhysicsWorld)
			return; // avoid repeats
		physics._bulletWorld.removeRigidBody(&_rigidBody);
		_addedToPhysicsWorld = false;
	}
}

