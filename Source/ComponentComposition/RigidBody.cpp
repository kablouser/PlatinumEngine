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
				.WithField<PhysicsMaterial>("physicalMaterial", PLATINUM_OFFSETOF(RigidBody, physicalMaterial))
				.WithField<float>("mass", PLATINUM_OFFSETOF(RigidBody, mass))
				.WithField<bool>("kinematic", PLATINUM_OFFSETOF(RigidBody, isKinematic));
	}

	RigidBody::RigidBody() :
			mass(1.f),
			isKinematic(false),
			_rigidBody(btRigidBody::btRigidBodyConstructionInfo(mass, &_motionState, &_emptyShape, {}))
	{
	}

	btTransform RigidBody::GetWorldTransform() const
	{
		btTransform matrix = _rigidBody.getWorldTransform();
		//_motionState.getWorldTransform(matrix);
		return matrix;
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
		//Add the rigidBody to the physics world
		scene.physics.RigidBodyOnBegin(scene.idSystem.GetSavedReference<RigidBody>(this));
	}

	void RigidBody::OnDisable(Scene& scene)
	{
		scene.physics.RigidBodyOnEnd(scene.idSystem.GetSavedReference<RigidBody>(this));
	}
}

