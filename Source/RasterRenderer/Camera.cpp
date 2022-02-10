//
// Created by Shihua on 2022/2/8.
//


#include "RasterRenderer/Camera.h"

#include <iostream>

#include "glm/gtc/quaternion.hpp"



using namespace PlatinumEngine;

Camera::Camera()
	:_eulerAngle(0.f, 0.f, 0.f),
	 _translationValue(0.f, 0.f, 1.f)
{

}

void Camera::RotationByMouse(glm::vec2 newMousePosition)
{

	glm::vec2 delta = newMousePosition - _initialMousePosition;
	_eulerAngle.x += delta.y *0.1;
	_eulerAngle.y += delta.x *0.1;

	_initialMousePosition = newMousePosition;

}

void Camera::UpdateInitialMousePosition(glm::vec2 initialMousePosition)
{
	_initialMousePosition = initialMousePosition;

}

void Camera::MoveCamera(glm::vec3 eulerAngle, glm::vec3 translationValue)
{
	_eulerAngle +=eulerAngle;

	// calculate rotation matrix (quaternion)
	glm::quat quaternionRoll(glm::vec3(-_eulerAngle.x, 0, 0));

	glm::quat quaternionYaw(glm::vec3( 0, -_eulerAngle.y, 0));

	glm::quat quaternionPitch(glm::vec3( 0, 0 , _eulerAngle.z));

	glm::mat4 rotationMat4 = glm::mat4_cast(quaternionYaw) * glm::mat4_cast(quaternionPitch) * glm::mat4_cast(quaternionRoll) ;

	// update translation value
	_translationValue.x += translationValue.x;
	_translationValue.y += translationValue.y;
	_translationValue.z += translationValue.z;

	glm::mat4 translationMat4(1.f);

	translationMat4[3][0] = -_translationValue.x;
	translationMat4[3][1] = -_translationValue.y;
	translationMat4[3][2] = -_translationValue.z;


	// calculate the new look at matrix by applying the transformation matrix
	lookAtMatrix4 = rotationMat4 * translationMat4;


}



