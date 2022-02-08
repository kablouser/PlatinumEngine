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

		// PARAMETER
		Camera(
				glm::vec3 eyePos = glm::vec3(0.f,0.f,0.f),
				glm::vec3 up     = glm::vec3 (0.f,1.f,0.f),
				glm::vec3 dir    = glm::vec3(0.f,0.f,-1.f)
				);



	private:
		// PARAMETER
		glm::vec3 _eyePos, _up, _dir;
	};

}