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

		AnimationLocalTimer timer;

	public:
		// {  CONSTRUCTOR  }
		AnimationComponent();


		// {  FUNCTION  }
		static void CreateTypeInfo(TypeDatabase& typeDatabase);

		void UpdateWorldTransformMatrix(
				ozz::unique_ptr<ozz::animation::Skeleton>& skeleton,
				const std::vector<Bone>& bones,
				Time& time);

		void SetCurrentAnimationByID(unsigned int inID);

		unsigned int GetCurrentAnimationID() const;

		unsigned int GetAmountOfAnimations() const;

		std::string GetAnimationName(unsigned int inID) const;

		void SetMesh(SavedReference<Mesh> mesh);

		bool CheckIfAnimationValid(unsigned int inID = 0) const;

		void SetIsDisplay(bool inIsDisplay);

		bool GetIsDisplay() const;

		void OnIDSystemUpdate() override;

	private:
		// {  PARAMETER  }
		SavedReference<Mesh> _mesh;
		unsigned int _selectedAnimationIndex;
		bool _isDisplay;
	};
}
