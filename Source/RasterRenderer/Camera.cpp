//
// Created by Shihua on 2022/2/8.
//


#include "RasterRenderer/Camera.h"

using namespace PlatinumEngine;

Camera::Camera()
	:_eyePosition(0.f,0.f,0.f),
	_up(0.f,1.f,0.f),
	_viewDirection(0.f,0.f,-1.f)
{

}

Camera::Camera(glm::vec3 eyePosition, glm::vec3 up, glm::vec3 viewDirection)
		: _eyePosition(eyePosition.x, eyePosition.y, eyePosition.z),
		  _up(up.x, up.y, up.z),
		  _viewDirection(viewDirection.x, viewDirection.y, viewDirection.z)
{

}


void Camera::RotateAroundYaw(float angle)
{



}



