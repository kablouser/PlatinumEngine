//
// Created by Shihua on 2022/2/8.
//

#pragma once

#include "glm/glm.hpp"

namespace PlatinumEngine
{
	class Camera
	{
	public:
		// FUNCTION
		void RotateAroundPitch(float angle);
		void RotateAroundYaw(float angle);
		void RotateAroundRoll(float angle);

		// CONSTRUCTOR
		Camera();
		Camera(
				glm::vec3 eyePos,
				glm::vec3 up,
				glm::vec3 dir
				);



	private:
		// PARAMETER
		glm::vec3 _eyePosition, _up, _viewDirection;
	};

}