//
// Created by Shihua on 01/05/2022.
//

#pragma once

#include "Component.h"


namespace PlatinumEngine
{
	class AnimationComponent: public Component
	{
	public:
		AnimationComponent();

		// update Animation from mesh loader
		void AddAnimation(Animation* animation);

		// return mesh
		std::vector<Animation*>& GetAnimation();

		// override the OnRender() of the Component
		//void OnRender(Scene& scene, Renderer& renderer) override;

	public:
		bool isAnimationDisplay;
		unsigned int selectedAnimationIndex;

	private:
		std::vector<Animation*> _animations;
	};
}
