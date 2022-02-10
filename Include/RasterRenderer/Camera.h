//
// Created by Shihua on 2022/2/8.
//

#pragma once

// math library

#include "glm/glm.hpp"

namespace PlatinumEngine
{
	class Camera
	{
	public:
		// VARIABLE
		glm::mat4 lookAtMatrix4;
		bool isFirstMousePress = false;

		// FUNCTION
		/**
		 * This function calculate the newest lookAt matrix. *ONLY RIGHT_HAND FOR NOW.
		 * @param eulerAngle
		 * @param translationValue
		 */
		void MoveCamera(glm::vec3 eulerAngle, glm::vec3 translationValue);

		void RotationByMouse(glm::vec2 delta);

		void UpdateInitialMousePosition(glm::vec2 delta);

		// CONSTRUCTOR
		Camera();




	private:
		// PARAMETER
		glm::vec3 _eulerAngle;
		glm::vec3 _translationValue;
		glm::vec2 _initialMousePosition;


	};

}