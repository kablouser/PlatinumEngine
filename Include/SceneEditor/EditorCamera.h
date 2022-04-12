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

		enum AxisType{horizontalAxis, verticalAxis};


		// FUNCTION
		/**
		 * The function calculate the newest view matrix.
		 */
		void MoveCamera();


		/**
		 * Accept mouse input and rotate camera based on the mouse position.
		 * @param delta
		 */
		void RotationByMouse(Maths::Vec2 delta);

		/**
		 * Rotate the camera by euler angle
		 * @param eulerAngle : the euler angle
		 */
		void RotateCamera(Maths::Vec3 eulerAngle);


		/**
		 * Accept mouse input and translate camera (left, right, up or down)
		 * based on the mouse position.
		 * @param newMousePosition
		 */
		void TranslationByMouse(Maths::Vec2 newMousePosition);

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
		void TranslationByKeyBoard(float x, float y);

		/**
		 * Translate
		 * @param movement : the translation direction with magnitude
		 */
		void TranslateCamera(Maths::Vec3 translateMovement);

		/**
		 * The three functions are to calculate new up/forward/right
		 * direction by the newest view matrix
		 * @return glm::vec3 is the new up/forward/right direction
		 */
		Maths::Vec3 GetUpDirection();
		Maths::Vec3 GetForwardDirection();
		Maths::Vec3 GetRightDirection();



		/**
		 * Functions for updating the projection matrix
		 * @param left
		 * @param right
		 * @param bottom
		 * @param top
		 * @param near
		 * @param far
		 * ...
		 */
		void SetFrustumMatrix(float left, float right, float bottom, float top, float near, float far);
		void SetOrthogonalMatrix(float left, float right, float bottom, float top, float zNear, float zFar);
		void SetPerspectiveMatrix(float fovy, float aspect, float near, float far);

		/**
		 * Get camera position const reference
		 * @return : camera position const reference
		 */
		const Maths::Vec3& GetCameraPosition();

		// CONSTRUCTOR
		EditorCamera();


	private:
		// PARAMETER

		// transformation
		Maths::Vec3 _eulerAngle;
		Maths::Vec3 _translationValue;
		Maths::Vec3 _cameraPosition; // need this position is because we need to adjust the camera position
									 // after the user change the fov of the camera)

		// device input data
		float _translationSpeed = 0.1;
		float _rotationSpeed = 0.005;

	};

}