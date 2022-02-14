//
// Created by Shihua on 2022/2/8.
//-----------------------------------------------------
// 0. Camera Control
//------------------------------------------------------
// Mouse:
// 		  left button                     -> do rotation
// 	      middle wheel (press and scroll) -> translation
//
// Keyboard:
// 		  up, down. left. right           -> translation
//------------------------------------------------------
// 1. Other notes
// -----------------------------------------------------
// Want to apply delta time but fails
//------------------------------------------------------

#include "RasterRenderer/EditorCamera.h"

#include <iostream> // TODO remove later

#include "glm/gtc/quaternion.hpp"



using namespace PlatinumEngine;

EditorCamera::EditorCamera()
	:_eulerAngle(0.f, 0.f, 0.f),
	 _translationValue(0.f, 0.f, 1.f)
{

}


void EditorCamera::RotationByMouse(glm::vec2 newMousePosition, const sf::Clock deltaClock)
{

	// calculate the movement of mouse
	glm::vec2 delta = newMousePosition - _initialMousePosition;

	_eulerAngle.x += delta.y * _rotationSpeed ;//* deltaClock.getElapsedTime().asSeconds();
	_eulerAngle.y += delta.x * _rotationSpeed ;//* deltaClock.getElapsedTime().asSeconds();

	// update mouse initial position
	_initialMousePosition = newMousePosition;

	// update view matrix
	MoveCamera(glm::vec3(0.0,0.0,0),glm::vec3(0.0,0.0,0.0));

}

void EditorCamera::TranslationByMouse(glm::vec2 newMousePosition, const sf::Clock deltaClock)
{

	// calculate the movement of mouse
	glm::vec2 delta = newMousePosition - _initialMousePosition;

	_translationValue += GetUpDirection() * delta.y * _translationSpeed ;//* deltaClock.getElapsedTime().asSeconds();
	_translationValue += GetRightDirection() * delta.x * (-_translationSpeed) ;//* deltaClock.getElapsedTime().asSeconds();



	// update mouse initial position
	_initialMousePosition = newMousePosition;



	// update view matrix
	MoveCamera(glm::vec3(0.0,0.0,0),glm::vec3(0.0,0.0,0.0));

}

void EditorCamera::TranslationByMouse( float wheelDelta, const sf::Clock deltaClock)
{

	if(wheelDelta > 0)
		_translationValue += GetForwardDirection() * (-_translationSpeed) * 5.f ;//* deltaClock.getElapsedTime().asSeconds();
	else
		_translationValue += GetForwardDirection() * (_translationSpeed)  * 5.f ;//* deltaClock.getElapsedTime().asSeconds();
	// update view matrix
	MoveCamera(glm::vec3(0.0,0.0,0),glm::vec3(0.0,0.0,0.0));

}

void EditorCamera::TranslationByKeyBoard(keyType type, const sf::Clock deltaClock)
{

	// move along forward direction

	// get forward direction
	if(type == down)
	{
		_translationValue += GetForwardDirection() * _translationSpeed * 5.f ;//* deltaClock.getElapsedTime().asSeconds();
	}
	else if(type == up)
	{
		_translationValue += GetForwardDirection() * (-_translationSpeed) * 5.f ;//* deltaClock.getElapsedTime().asSeconds();
	}

	// move along right direction
	else if(type == left)
	{
		_translationValue += GetRightDirection() * _translationSpeed * 5.f;// * deltaClock.getElapsedTime().asSeconds();
	}
	else if(type == right)
	{

		_translationValue += GetRightDirection() * (-_translationSpeed) * 5.f;// * deltaClock.getElapsedTime().asSeconds();

	}

	// update view matrix
	MoveCamera(glm::vec3(0.0,0.0,0),glm::vec3(0.0,0.0,0.0));

}

void EditorCamera::UpdateInitialMousePosition(glm::vec2 initialMousePosition)
{
	_initialMousePosition = initialMousePosition;

}


glm::vec3 EditorCamera::GetUpDirection()
{

	return glm::quat (_eulerAngle) * glm::vec3 (0.f, 1.f, 0.f);

}

glm::vec3 EditorCamera::GetForwardDirection()
{

	return glm::quat (_eulerAngle) * glm::vec3 (0.f, 0.f, 1.f);

}

glm::vec3 EditorCamera::GetRightDirection()
{

	return glm::quat (_eulerAngle) * glm::vec3 (1.f, 0.f, 0.f);

}

void EditorCamera::MoveCamera(glm::vec3 eulerAngle, glm::vec3 translationValue)
{
	// update euler angle
	_eulerAngle += eulerAngle;

	// get rotation matrix (quaternion)
	glm::quat quaternionRoll(glm::vec3(-_eulerAngle.x, 0, 0));

	glm::quat quaternionYaw(glm::vec3( 0, -_eulerAngle.y, 0));

	glm::quat quaternionPitch(glm::vec3( 0, 0 , _eulerAngle.z));

	glm::mat4 rotationMat4 = glm::mat4_cast(quaternionPitch) * glm::mat4_cast(quaternionRoll) * glm::mat4_cast(quaternionYaw) ;


	// update translation value
	_translationValue.x += translationValue.x;
	_translationValue.y += translationValue.y;
	_translationValue.z += translationValue.z;

	// get translation matrix
	glm::mat4 translationMat4(1.f);

	translationMat4[3][0] = -_translationValue.x;
	translationMat4[3][1] = -_translationValue.y;
	translationMat4[3][2] = -_translationValue.z;


	// calculate the new look at matrix by applying the transformation matrix
	lookAtMatrix4 = rotationMat4 * translationMat4;


}



