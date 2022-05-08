//
// Created by Shihua on 04/05/2022.
//

#include <Animation/AnimationLocalTimer.h>

namespace PlatinumEngine
{

	void AnimationLocalTimer::CreateTypeInfo(TypeDatabase& typeDatabase)
	{
		typeDatabase.BeginTypeInfo<AnimationLocalTimer>()
				.WithField<float>("animationTime", PLATINUM_OFFSETOF(AnimationLocalTimer,animationTime))
				.WithField<bool>("isAnimationLooping", PLATINUM_OFFSETOF(AnimationLocalTimer,isAnimationLooping))
				.WithField<bool>("isAnimationPlaying", PLATINUM_OFFSETOF(AnimationLocalTimer,isAnimationPlaying));
		// _lastFrame don't need to be serialized.
	}

	void AnimationLocalTimer::RestartAnimation()
	{
		isAnimationPlaying = true;
		animationTime = 0.f;
	}

	void AnimationLocalTimer::StopAnimation()
	{
		isAnimationPlaying = false;
		animationTime = 0.f;
	}

	[[nodiscard]] bool AnimationLocalTimer::IsAnimationFinish(float animationDuration) const
	{
		return animationDuration <= animationTime;
	}

	void AnimationLocalTimer::Update(Time& time, float animationDuration)
	{
		// sanitise state
		animationTime = std::clamp(animationTime, 0.f, animationDuration);

		if (!isAnimationPlaying)
			return;

		int currentFrame = time.getFramesPassed();
		if (currentFrame != _lastFrame)
		{
			// only add dt when frame number has changed
			animationTime += time.GetDelta();
			_lastFrame = currentFrame;
		}

		if (isAnimationLooping)
		{
			while (IsAnimationFinish(animationDuration))
				animationTime -= animationDuration;
		}
		else
		{
			if (IsAnimationFinish(animationDuration))
			{
				animationTime = animationDuration;
			}
		}
	}

	AnimationLocalTimer::AnimationLocalTimer() = default;
}