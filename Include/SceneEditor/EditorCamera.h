//
// Created by Shihua on 2022/2/8.
//

#pragma once

#include "Maths/Matrices.h"
#include "Maths/Quaternion.h"


namespace PlatinumEngine
{
	class EditorCamera
	{
	public:
		// ___VARIABLE___

		// view Matrix stuff
		Maths::Mat4 viewMatrix4;

		// projection stuff
		Maths::Mat4 projectionMatrix4;


		// Flags
		bool isOrthogonal;
		enum AxisType{horizontalAxis, verticalAxis};

		// Translation?
		float viewPortSize = 100; // for moving closer in orthogonal mode?


		// FUNCTION
		/**
		 * The function calculate the newest view matrix.
		 */
		void UpdateViewMatrix();


		/**
		 * Accept mouse input and rotate camera based on the mouse position.
		 * @param delta
		 */
		void RotationByMouse(Maths::Vec2 delta);


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
		void ChangeSpeedScale(float wheelDelta);

		/**
		 * Accept keyboard input and translate camera
		 * based on the mouse wheel delta value.
		 * @param type
		 */
		void TranslationByKeyBoard(float x, float y);

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
		void SetOrthogonalMatrix(float left, float right, float bottom, float top, float zNear, float zFar);
		void SetPerspectiveMatrix(float fovy, float aspect, float near, float far);


		/**
		 * Get camera position const reference
		 * @return : camera position const reference
		 */
		const Maths::Vec3& GetCameraPosition();

		/**
		 *
		 * @param matrixArray
		 */
		void UpdateCameraQuaternion();

		/**
		 * Set the flag _IsViewMatrixUsed to be true (meaning the flag is passed into the shader?)
		 */
		void MarkViewMatrixAsUsed();

		/**
		 * Return the _IsViewMatrixUsed flag
		 * @return : the _IsViewMatrixUsed flag
		 */
		bool CheckIfViewMatrixUsed();

		/**
		 * Set the flag _IsViewMatrixUsed to be false (meaning the flag is passed into the shader?)
		 */
		void MarkProjectionMatrixAsUsed();

		/**
		 * Return the _IsProjectionMatrixUsed flag
		 * @return : the _IsProjectionMatrixUsed flag
		 */
		bool CheckIfProjectionMatrixUsed();


		/**
		 * Return the rotation part of the view matrix
		 * @return : the rotation part of the view matrix
		 */
		Maths::Mat4 GetRotationOnlyViewMatrix();

		/**
		 * Set camera position by the input position value
		 * @param newPosition : the position that we want to move to
		 */
		void SetCameraPosition(Maths::Vec3 newPosition);

		// ___CONSTRUCTOR___
		EditorCamera();


	private:
		// ___PARAMETER___

		// flag
		bool _IsViewMatrixUsed = false;
		bool _IsProjectionMatrixUsed = false;

		// transformation
		Maths::Quaternion _quaternion;
		Maths::Vec3 _translationValue;

		// device input data
		float _translationSpeed = 0.1;
		float _rotationSpeed = 0.002;
		// the translation speed multiplier
		float _speedScale = 1.0f;

	};

}