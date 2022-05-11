//
// Created by Shihua on 10/05/2022.
//

#pragma once

#include <iostream>
#include <Maths/Matrices.h>
#include <vector>
#include <ComponentComposition/Component.h>
#include <ComponentComposition/AnimationComponent.h>
#include <ComponentComposition/MeshRender.h>
#include <ComponentComposition/Transform.h>
namespace PlatinumEngine
{
	class AnimationAttachment: public Component
	{
	public:
		// PARAMETERS
		std::vector<std::string> jointsName; //need to update when adding an attachment component to a game object
		Maths::Mat4 offsetMatrix; 			 //need to update when user adjust it in GUI
		Maths::Mat4 transformMatrix; 		 //need to update every frame
											 //need to update when the selected Joint is updated
		unsigned int selectedJoint;			 //need to update when user select a new joint to attach the attachment

		Maths::Vec3 translation, rotation;

		// FUNCTIONS
		void GetSkeletonNameFromParentAnimation();

		void UpdateTransformMatrixBySelectedJoint();

		void SetSelectedJoint(unsigned int jointID);

		void CleanAttachmentSetting();

		void UpdateOffsetMatrix(Maths::Vec3 translation, Maths::Vec3 rotation);

		bool CheckIfParentAnimationDisplay();

		static void CreateTypeInfo(TypeDatabase& typeDatabase);


		// CONSTRUCTOR
		AnimationAttachment();

	private:

	};
}
