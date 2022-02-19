//
// Created by Shihua on 2022/2/8.
//

#pragma once

// math library
#include "glm/gtc/quaternion.hpp"

// sfml
#include <SFML/System/Clock.hpp>


namespace PlatinumEngine
{
	class EditorCamera
	{
	public:
		// VARIABLE
		glm::mat4 lookAtMatrix4;
		bool isFirstMousePress = false;
		bool isFirstMouseWheelPress = false;
		enum KeyType
		{
			up, down, left, right
		};


		// FUNCTION
		/**
		 * The function calculate the newest view matrix.
		 * @param eulerAngle
		 * @param translationValue
		 */
		void MoveCamera(glm::vec3 eulerAngle, glm::vec3 translationValue);


		/**
		 * Accept mouse input and rotate camera based on the mouse position.
		 * @param delta
		 */
		void RotationByMouse(glm::vec2 delta, const sf::Clock deltaClock);


		/**
		 * Accept mouse input and translate camera (left, right, up or down)
		 * based on the mouse position.
		 * @param newMousePosition
		 */
		void TranslationByMouse(glm::vec2 newMousePosition, const sf::Clock deltaClock);

		/**
		 * Accept mouse input and translate camera (forward or backward)
		 * based on the mouse wheel delta value.
		 * @param wheelDelta
		 */
		void TranslationByMouse(float wheelDelta, const sf::Clock deltaClock);

		/**
		 * Accept keyboard input and translate camera
		 * based on the mouse wheel delta value.
		 * @param type
		 */
		void TranslationByKeyBoard(KeyType type, const sf::Clock deltaClock);

		/**
		 * Update the initial mouse position.
		 * @param delta
		 */
		void UpdateInitialMousePosition(glm::vec2 delta);

		/**
		 * The three functions are to calculate new up/forward/right
		 * direction by the newest view matrix
		 * @return glm::vec3 is the new up/forward/right direction
		 */
		glm::vec3 GetUpDirection();
		glm::vec3 GetForwardDirection();
		glm::vec3 GetRightDirection();



		// CONSTRUCTOR
		EditorCamera();


	private:
		// PARAMETER

		// transformation
		glm::vec3 _eulerAngle;
		glm::vec3 _translationValue;

		// device input data
		glm::vec2 _initialMousePosition;
		float _translationSpeed = 0.005;
		float _rotationSpeed = 0.005;

	};

}