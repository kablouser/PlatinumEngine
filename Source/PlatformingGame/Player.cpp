#include <PlatformingGame/Player.h>
#include <Application.h>

namespace PlatinumEngine
{
	void Player::CreateTypeInfo()
	{
		Application::Instance->typeDatabase.BeginTypeInfo<Player>()
				.WithField<float>("moveSpeed", PLATINUM_OFFSETOF(Player, moveSpeed))
				.WithField<float>("moveAcceleration", PLATINUM_OFFSETOF(Player, moveAcceleration))
				.WithField<float>("moveDeceleration", PLATINUM_OFFSETOF(Player, moveDeceleration))
				.WithField<float>("jumpSpeed", PLATINUM_OFFSETOF(Player, jumpSpeed));
	}

	void Player::OnStart()
	{
		_rigidBody = GetComponent<RigidBody>();
		if (_rigidBody)
			// movement requires collision recorded
			_rigidBody.DeRef()->isCollisionRecorded = true;
		_audioComponent = GetComponent<AudioComponent>();
		_transform = GetComponent<Transform>();
	}

	void Player::OnUpdate()
	{
		if (!_rigidBody)
		{
			PLATINUM_WARNING("Player component requires RigidBody!");
			return;
		}
		RigidBody* rigidBodyPointer = _rigidBody.DeRef().get();

		float x = Application::Instance->inputManager.GetAxis("Horizontal");
		float y = Application::Instance->inputManager.GetAxis("Vertical");

		auto velocity = rigidBodyPointer->GetVelocity();

		float desiredVelocityX = x * moveSpeed;
		float accelerationFactor;
		if (desiredVelocityX * velocity.x < -0.01f)
			// trying to move in opposite directions, decelerate
			accelerationFactor = moveDeceleration;
		else
			accelerationFactor = moveAcceleration;
		// unit length
		float interpolateDirection = (desiredVelocityX - _currentVelocityX) < 0.0f ? -1.0f : 1.0f;

		// get range before interpolation
		float velocityXMin = std::min(_currentVelocityX, desiredVelocityX);
		float velocityXMax = std::max(_currentVelocityX, desiredVelocityX);
		// interpolate
		_currentVelocityX += interpolateDirection * accelerationFactor * Application::Instance->time.GetDelta();
		// clamp ranges
		_currentVelocityX = std::clamp(_currentVelocityX, velocityXMin, velocityXMax);

		float time = Application::Instance->time.getTime();
		if (0.1f < y && _nextJumpAvailable < time && IsGrounded(rigidBodyPointer))
		{
			// jump up
			velocity.y = jumpSpeed;
			// disallow jumps for a short period
			_nextJumpAvailable = time + 0.2f;

			if (_audioComponent)
				_audioComponent.DeRef()->Play();
		}

		rigidBodyPointer->SetVelocity({ _currentVelocityX, velocity.y, 0 });
		rigidBodyPointer->SetAngularVelocity({});
		if (_transform)
			_transform.DeRef()->localRotation = Maths::Quaternion();
	}

	void Player::OnIDSystemUpdate()
	{
		_rigidBody.OnIDSystemUpdate(Application::Instance->idSystem);
	}

	bool Player::IsGrounded(RigidBody* rigidBodyPointer) const
	{
		for (const Collision& collision: rigidBodyPointer->GetCollisionRecords())
		{
			if (0.2f < collision.normal.y)
				return true;
		}
		return false;
	}
}