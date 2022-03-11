//
// Created by Shihua on 2022/2/8.
//

#pragma once

#include "Maths/Matrices.h"

namespace PlatinumEngine
{
	class EditorCamera
	{
	public:
		// VARIABLE

		// Camera relative Matrix
		Maths::Mat4 viewMatrix4;
		Maths::Mat4 projectionMatrix4;

		// Flags
		bool isOrthogonal;

		// Input type
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
		void MoveCamera(Maths::Vec3 eulerAngle,
				Maths::Vec3 translationValue);


		/**
		 * Accept mouse input and rotate camera based on the mouse position.
		 * @param delta
		 */
		void RotationByMouse(glm::vec2 delta);


		/**
		 * Accept mouse input and translate camera (left, right, up or down)
		 * based on the mouse position.
		 * @param newMousePosition
		 */
		void TranslationByMouse(glm::vec2 newMousePosition);

		/**
		 * Accept mouse input and translate camera (forward or backward)
		 * based on the mouse wheel delta value.
		 * @param wheelDelta
		 */
		void TranslationByMouse(float wheelDelta);

		/**
		 * Accept keyboard input and translate camera
		 * based on the mouse wheel delta value.
		 * @param type
		 */
		void TranslationByKeyBoard(KeyType type);

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
		Maths::Vec3 GetUpDirection();
		Maths::Vec3 GetForwardDirection();
		Maths::Vec3 GetRightDirection();



		/// Functions for projection matrix

		void SetFrustumMatrix(float left, float right, float bottom, float top, float near, float far);
		void SetOrthogonalMatrix(float left, float right, float bottom, float top, float zNear, float zFar);
		void SetPerspectiveMatrix(float fovy, float aspect, float near, float far);






		// CONSTRUCTOR
		EditorCamera();


	private:
		// PARAMETER

		// transformation
		Maths::Vec3 _eulerAngle;
		Maths::Vec3 _translationValue;

		// device input data
		glm::vec2 _initialMousePosition;
		float _translationSpeed = 0.005;
		float _rotationSpeed = 0.005;

	};

}