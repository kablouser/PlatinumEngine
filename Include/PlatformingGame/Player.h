#pragma once

#include <ComponentComposition/Component.h>
#include <ComponentComposition/RigidBody.h>
#include <ComponentComposition/AudioComponent.h>
#include <ComponentComposition/Transform.h>
#include <ComponentComposition/AnimationComponent.h>
#include <ComponentComposition/Camera.h>

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
		SavedReference<AudioComponent> _jumpAudio;
		SavedReference<AudioComponent> _footStepAudio;
		SavedReference<Transform> _transform;
		SavedReference<AnimationComponent> _animationComponent;
		SavedReference<Transform> _animationTransform;
		SavedReference<Transform> _cameraTransform;
		float _currentVelocityX = 0.f;
		float _nextJumpAvailable = 0.f;
		float _nextFootStep = 0.f;

		bool IsGrounded(RigidBody* rigidBodyPointer) const;
	};
}