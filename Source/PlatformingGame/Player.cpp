#include <Application.h>
#include <PlatformingGame/Player.h>

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
		_jumpAudio = GetComponent<AudioComponent>();
		_transform = GetComponent<Transform>();

		if (0 < GetGameObject().DeRef()->GetChildrenCount())
		{
			_animationComponent =
					GetGameObject().DeRef()->GetChild(0).DeRef()->GetComponent<AnimationComponent>();
			_animationTransform = _animationComponent.DeRef()->GetComponent<Transform>();
			if (_animationTransform)
				_animationTransform.DeRef()->localRotation = Maths::Quaternion(Maths::Vec3(0, 110, 0));

			_footStepAudio =
					GetGameObject().DeRef()->GetChild(0).DeRef()->GetComponent<AudioComponent>();
		}

		SavedReference<Camera> camera = Application::Instance->scene.FindFirstComponent<Camera>();
		if (camera)
			_cameraTransform = camera.DeRef()->GetComponent<Transform>();
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

			if (_jumpAudio)
				_jumpAudio.DeRef()->Play();
		}

		rigidBodyPointer->SetVelocity({ _currentVelocityX, velocity.y, 0 });
		rigidBodyPointer->SetAngularVelocity({});
		if (_transform)
			_transform.DeRef()->localRotation = Maths::Quaternion();

		if (_animationComponent && _animationTransform)
		{
			AnimationComponent* animationPointer = _animationComponent.DeRef().get();
			Transform* animationTransformPointer = _animationTransform.DeRef().get();

			bool isWalking = false;
			if (std::abs(_currentVelocityX) < 0.01f)
			{
				if (y < 0.f)
					animationPointer->SetCurrentAnimationByID(1);
				else
					animationPointer->SetCurrentAnimationByID(3);
			}
			else if (0 < _currentVelocityX)
			{
				isWalking = true;
				animationTransformPointer->localRotation = Maths::Quaternion(Maths::Vec3(0, 110, 0));
			}
			else
			{
				isWalking = true;
				animationTransformPointer->localRotation = Maths::Quaternion(Maths::Vec3(0, -110, 0));
			}

			if (isWalking)
			{
				animationPointer->SetCurrentAnimationByID(4);
				if (_footStepAudio && _nextFootStep < Application::Instance->time.getSeconds() && isGrounded)
				{
					_footStepAudio.DeRef()->Play();
					_nextFootStep = Application::Instance->time.getSeconds() + 0.2f;
				}
			}

			if (isGrounded == false)
				// fly animation
				animationPointer->SetCurrentAnimationByID(2);
		}

		if (_cameraTransform)
		{
			Maths::Vec3 targetCameraPosition = _transform.DeRef()->localPosition;
			targetCameraPosition += Maths::Vec3(0.f, 2.f, -15.f);

			float lerpParameter = Application::Instance->time.GetDelta() * 2.0f;
			_cameraTransform.DeRef()->localPosition =
					_cameraTransform.DeRef()->localPosition * (1.f - lerpParameter) +
					targetCameraPosition * lerpParameter;
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