//
// Created by Shihua on 04/05/2022.
//

#include <Animation/AnimationLocalTimer.h>
#include <iostream>

namespace PlatinumEngine
{

	void AnimationLocalTimer::CreateTypeInfo(TypeDatabase& typeDatabase)
	{
		typeDatabase.BeginTypeInfo<AnimationLocalTimer>()
				.WithField<float>("_animationTime", PLATINUM_OFFSETOF(AnimationLocalTimer,_animationTime))
				.WithField<float>("_duration", PLATINUM_OFFSETOF(AnimationLocalTimer,_duration))
				.WithField<bool>("_isAnimationLooping", PLATINUM_OFFSETOF(AnimationLocalTimer,_isAnimationLooping))
				.WithField<bool>("_isLastFrame", PLATINUM_OFFSETOF(AnimationLocalTimer,_isLastFrame))
				.WithField<bool>("_isAnimationPlaying", PLATINUM_OFFSETOF(AnimationLocalTimer,_isAnimationPlaying))
				.WithField<bool>("_isAnimationFinish", PLATINUM_OFFSETOF(AnimationLocalTimer,_isAnimationFinish));
	}

	void AnimationLocalTimer::SetAnimationDuration(float inDuration)
	{
		_duration = inDuration >= 0.f ? inDuration:0.f;
	}

	void AnimationLocalTimer::UpdateAnimationTime()
	{
		_animationTime += 0.1f;

		if(_animationTime >= _duration)
		{
			_animationTime = 0.f;

			_isLastFrame = true;
		}
		else
		{
			_isLastFrame = false;
		}
	}

	void AnimationLocalTimer::SetAnimationTime(float newTime)
	{
		if(_duration == 0.f)
			_animationTime = 0.f;
		else
			_animationTime = newTime - float(int(newTime/_duration)) * _duration;
	}

	void AnimationLocalTimer::RestartAnimation()
	{
		_animationTime = 0.f;
		_isLastFrame = false;
		_isAnimationPlaying = true;
	}

	void AnimationLocalTimer::StopAnimation()
	{
		_animationTime = 0.f;
		_isLastFrame = false;
		_isAnimationPlaying = false;
	}

	void AnimationLocalTimer::PauseAnimation()
	{
		_isAnimationPlaying = false;
	}

	void AnimationLocalTimer::ContinueAnimation()
	{
		_isAnimationPlaying = true;
	}

	void AnimationLocalTimer::LoopAnimation()
	{
		_isAnimationLooping = true;
	}

	void AnimationLocalTimer::StopLoopingAnimation()
	{
		_isAnimationLooping = false;
	}

	bool AnimationLocalTimer::CheckIfAnimationFinish() const
	{
		return _isAnimationFinish;
	}

	bool AnimationLocalTimer::CheckIfAnimationPlaying() const
	{
		return _isAnimationPlaying;
	}

	void AnimationLocalTimer::PlayAnimationTimer()
	{
		if(_isAnimationPlaying && !_isAnimationFinish)
			UpdateAnimationTime();

		if(_isLastFrame && !_isAnimationLooping)
			_isAnimationFinish = true;
		else
			_isAnimationFinish = false;
	}

	float AnimationLocalTimer::GetAnimationTime() const
	{
		return _animationTime;
	}

	AnimationLocalTimer::AnimationLocalTimer():_duration(0.f)
	{}



}