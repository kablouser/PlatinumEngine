//
// Created by shawn on 2022/4/22.
//

#include <ComponentComposition/RigidBody.h>
#include <ComponentComposition/Transform.h>
#include <Application.h>

namespace PlatinumEngine
{
	void RigidBody::CreateTypeInfo(TypeDatabase& database)
	{
		database.BeginTypeInfo<RigidBody>()
				.WithInherit<Component>()
				.WithField<PhysicsMaterial>("physicsMaterial", PLATINUM_OFFSETOF(RigidBody, physicsMaterial))
				.WithField<float>("mass", PLATINUM_OFFSETOF(RigidBody, mass))
				.WithField<bool>("isKinematic", PLATINUM_OFFSETOF(RigidBody, isKinematic))
				.WithField<bool>("isCollisionRecorded", PLATINUM_OFFSETOF(RigidBody, isCollisionRecorded));
	}

	RigidBody::RigidBody() :
			mass(1.f),
			isKinematic(false),
			isCollisionRecorded(false),
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

	void RigidBody::OnEnable()
	{
		//Add the rigidBody to the physics manager thing
		Application::Instance->physics._allRigidBodies.push_back(
				Application::Instance->idSystem.GetSavedReference<RigidBody>(this));
		UpdatePhysicsProperties();
	}

	void RigidBody::OnDisable()
	{
		RemoveFromPhysicsWorld();
		if (!VectorHelpers::RemoveFirst(Application::Instance->physics._allRigidBodies,
				Application::Instance->idSystem.GetSavedReference<RigidBody>(this)))
			PLATINUM_WARNING("Missing RigidBody from Physics Manager thing");
	}

	void RigidBody::UpdatePhysicsProperties()
	{
		RemoveFromPhysicsWorld();

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
		btRigidBody::btRigidBodyConstructionInfo constructInfo(mass, &_motionState, btCollisionShape, inertia);
		constructInfo.m_startWorldTransform = worldTransform;
		constructInfo.m_friction = physicsMaterial.friction;
		constructInfo.m_restitution = physicsMaterial.bounciness;
		constructInfo.m_linearDamping = physicsMaterial.damping;
		constructInfo.m_angularDamping = physicsMaterial.angularDamping;
		_rigidBody = btRigidBody(constructInfo);
		if (isKinematic)
			_rigidBody.setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);

		AddToPhysicsWorld();
	}

	void RigidBody::SetVelocity(Maths::Vec3 velocity)
	{
		_rigidBody.setLinearVelocity(Physics::ConvertVector(velocity));
		_rigidBody.activate(); // _rigidBody will not move if sleeping, activate awakens _rigidBody
	}

	Maths::Vec3 RigidBody::GetVelocity() const
	{
		return Physics::ConvertVectorBack(_rigidBody.getLinearVelocity());
	}

	void RigidBody::SetAngularVelocity(Maths::Vec3 angularVelocity)
	{
		_rigidBody.setAngularVelocity(Physics::ConvertVector(angularVelocity));
		_rigidBody.activate(); // _rigidBody will not move if sleeping, activate awakens _rigidBody
	}

	Maths::Vec3 RigidBody::GetAngularVelocity() const
	{
		return Physics::ConvertVectorBack(_rigidBody.getAngularVelocity());
	}

	void RigidBody::AddToPhysicsWorld()
	{
		if (_addedToPhysicsWorld)
			return; // avoid repeats
		Application::Instance->physics._bulletWorld.addRigidBody(&_rigidBody);
		_addedToPhysicsWorld = true;
	}

	void RigidBody::RemoveFromPhysicsWorld()
	{
		if (!_addedToPhysicsWorld)
			return; // avoid repeats
		Application::Instance->physics._bulletWorld.removeRigidBody(&_rigidBody);
		_addedToPhysicsWorld = false;
	}

	void RigidBody::Reposition(const Maths::Vec3& position, const Maths::Quaternion& rotation)
	{
		btTransform initialTransform;

		initialTransform.setOrigin(Physics::ConvertVector(position));
		initialTransform.setRotation(Physics::ConvertQuaternion(rotation));

		_motionState = btDefaultMotionState();
		_rigidBody.setWorldTransform(initialTransform);
		_rigidBody.setMotionState(&_motionState);
		_rigidBody.getMotionState()->setWorldTransform(initialTransform);
	}
}

