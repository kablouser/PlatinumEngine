//
// Created by Shihua on 01/05/2022.
//

#pragma once

#include "Component.h"
#include <TypeDatabase/TypeDatabase.h>
#include <Animation/AnimationLocalTimer.h>


namespace PlatinumEngine
{
	class AnimationComponent: public Component
	{
	public:
		// {  PARAMETER  }
		std::vector<Maths::Mat4> worldTransform;
		bool isDisplay;
		ozz::vector<ozz::unique_ptr<ozz::animation::Animation>> animations;


	public:
		// {  CONSTRUCTOR  }
		AnimationComponent();


		// {  FUNCTION  }
		static void CreateTypeInfo(TypeDatabase& typeDatabase);

		// update Animation from mesh loader
		void AddAnimation(ozz::unique_ptr<ozz::animation::Animation>& animation);

		// remove animation
		void RemoveAnimation(unsigned int index);

		void UpdateWorldTransformMatrix(ozz::unique_ptr<ozz::animation::Skeleton>& skeleton,const std::vector<Bone>& bones);

		void SetCurrentAnimationByID(unsigned int inID);

		unsigned int GetCurrentAnimationID() const;

		unsigned  int GetAmountOfAnimations() const;

		std::string GetAnimationName(unsigned int inID) const;

	private:
		// {  PARAMETER  }
		AnimationLocalTimer _timer;
		unsigned int _selectedAnimationIndex;
	};
}
