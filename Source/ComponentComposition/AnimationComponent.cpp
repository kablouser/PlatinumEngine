//
// Created by Shihua on 01/05/2022.
//

#include <ComponentComposition/AnimationComponent.h>

namespace PlatinumEngine
{
	AnimationComponent::AnimationComponent(): isAnimationDisplay(false), selectedAnimationIndex(0)
	{
	}


	void AnimationComponent::AddAnimation(Animation* animation)
	{
		_animations.push_back(animation);
	}


	std::vector<Animation*>& AnimationComponent::GetAnimation()
	{
		return _animations;
	}
}
