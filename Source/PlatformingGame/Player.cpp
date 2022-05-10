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

		if (0 < GetGameObject().DeRef()->GetChildrenCount())
		{
			_animationComponent =
					GetGameObject().DeRef()->GetChild(0).DeRef()->GetComponent<AnimationComponent>();
			_animationTransform = _animationComponent.DeRef()->GetComponent<Transform>();
			if (_animationTransform)
				_animationTransform.DeRef()->localRotation = Maths::Quaternion(Maths::Vec3(0, 110, 0));
		}
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
		bool isGrounded = IsGrounded(rigidBodyPointer);
		if (0.1f < y && _nextJumpAvailable < time && isGrounded)
		{
			// jump up
			velocity.y = jumpSpeed;
			// disallow jumps for a short period
			_nextJumpAvailable = time + 0.2f;
			isGrounded = false;

			if (_audioComponent)
				_audioComponent.DeRef()->Play();
		}

		rigidBodyPointer->SetVelocity({ _currentVelocityX, velocity.y, 0 });
		rigidBodyPointer->SetAngularVelocity({});
		if (_transform)
			_transform.DeRef()->localRotation = Maths::Quaternion();

		if (_animationComponent && _animationTransform)
		{
			AnimationComponent* animationPointer = _animationComponent.DeRef().get();
			Transform* animationTransformPointer = _animationTransform.DeRef().get();
			if (std::abs(_currentVelocityX) < 0.01f)
			{
				animationPointer->SetCurrentAnimationByID(3);
				if (y < 0.f)
					animationPointer->SetCurrentAnimationByID(1);
			}
			else if (0 < _currentVelocityX)
			{
				animationPointer->SetCurrentAnimationByID(4);
				animationTransformPointer->localRotation = Maths::Quaternion(Maths::Vec3(0, 110, 0));
			}
			else
			{
				animationPointer->SetCurrentAnimationByID(4);
				animationTransformPointer->localRotation = Maths::Quaternion(Maths::Vec3(0, -110, 0));
			}

			if (isGrounded == false)
				// fly animation
				animationPointer->SetCurrentAnimationByID(2);
		}
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