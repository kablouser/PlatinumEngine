//
// Created by Shihua on 01/05/2022.
//

#pragma once

#include "Component.h"
#include <TypeDatabase/TypeDatabase.h>
#include <Animation/AnimationLocalTimer.h>
#include <SceneManager/Scene.h>

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

		/**
		 *
		 * @param skeleton : skeleton for the animation
		 * @param bones : bones that store the offset matrices (matrix that transform local coordinate to be "bone" coordinate) for calculation
		 */
		void UpdateWorldTransformMatrix(ozz::unique_ptr<ozz::animation::Skeleton>& skeleton,const std::vector<Bone>& bones);

		void SetCurrentAnimationByID(unsigned int inID);

		unsigned int GetCurrentAnimationID() const;

		unsigned int GetAmountOfAnimations() const;

		std::string GetAnimationName(unsigned int inID) const;

		void SetMesh(SavedReference<Mesh> mesh);

		bool CheckIfAnimationValid(unsigned int inID = 0) const;

		void SetIsDisplay(bool inIsDisplay);

		bool GetIsDisplay() const;

		void OnIDSystemUpdate(Scene& scene);

	private:
		// {  PARAMETER  }
		SavedReference<Mesh> _mesh;
		unsigned int _selectedAnimationIndex;
		bool _isDisplay;
	};
}
