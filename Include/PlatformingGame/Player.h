#pragma once

#include <ComponentComposition/Component.h>
#include <ComponentComposition/RigidBody.h>

namespace PlatinumEngine
{
	class Player : public Component
	{
	public:
		static void CreateTypeInfo();

		void OnStart() override;

		void OnEnd() override;

		void OnUpdate() override;

		void OnIDSystemUpdate() override;

		float moveSpeed = 5.0f;
		float moveAcceleration = 1.0f;
		float moveDeceleration = 1.0f;
		float jumpSpeed = 5.0f;

	private:
		SavedReference<RigidBody> _rigidBody;
		float _currentVelocityX;

		bool IsGrounded(RigidBody* rigidBodyPointer) const;
	};
}