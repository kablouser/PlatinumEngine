//
// Created by Shihua on 01/05/2022.
//

#pragma once

#include "Component.h"
#include <TypeDatabase/TypeDatabase.h>


namespace PlatinumEngine
{
	class AnimationComponent: public Component
	{
	public:
		static void CreateTypeInfo(TypeDatabase& typeDatabase);

		AnimationComponent();

		// update Animation from mesh loader
		void AddAnimation(Animation* animation);

		// return mesh
		std::vector<Animation*>& GetAnimation();

		// remove animation
		void RemoveAnimation(unsigned int index);


		// override the OnRender() of the Component
		//void OnRender(Scene& scene, Renderer& renderer) override;

	public:
		bool isAnimationDisplay;
		unsigned int selectedAnimationIndex;
		ozz::animation::SamplingJob::Context context;

	private:

		// TODO you shouldn't use Animation pointers.
		// Because you should restrict the animations to the one attached to the current mesh.
		// You can fetch the list of animations from the mesh on the MeshRender.
		std::vector<Animation*> _animations;
	};
}
