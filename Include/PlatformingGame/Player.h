#pragma once

#include <ComponentComposition/Component.h>
#include <ComponentComposition/RigidBody.h>
#include <ComponentComposition/AudioComponent.h>
#include <ComponentComposition/Transform.h>

namespace PlatinumEngine
{
	class Player : public Component
	{
	public:
		static void CreateTypeInfo();

		void OnStart() override;

		void OnUpdate() override;

		void OnIDSystemUpdate() override;

		float moveSpeed = 5.0f;
		float moveAcceleration = 1.0f;
		float moveDeceleration = 1.0f;
		float jumpSpeed = 5.0f;

	private:
		SavedReference<RigidBody> _rigidBody;
		SavedReference<AudioComponent> _audioComponent;
		SavedReference<Transform> _transform;
		float _currentVelocityX = 0.f;
		float _nextJumpAvailable = 0.f;

		bool IsGrounded(RigidBody* rigidBodyPointer) const;
	};
}