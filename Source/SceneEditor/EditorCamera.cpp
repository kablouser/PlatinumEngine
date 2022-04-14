//
// Created by Shihua on 2022/2/8.
//-----------------------------------------------------
// 0. Camera Control
//------------------------------------------------------
// Mouse:
// 		  left button                     -> do rotation
//
// 	      right button					  -> translation
//
// 	      middle wheel (scroll) 		  -> translation
//
// Keyboard:
// 		  up, down. left. right           -> translation
//------------------------------------------------------
// 1. Other notes
// -----------------------------------------------------


#include "SceneEditor/EditorCamera.h"


namespace PlatinumEngine
{

	EditorCamera::EditorCamera()
			: _eulerAngle(0.f, 0.f, 0.f),
			  _translationValue(0.f, 0.f, -5.f),
			  viewMatrix4(1.f),
			  projectionMatrix4(1.f),
			  isOrthogonal(false),
			  _nearPanelForClippingSpace(4.f),
			  _farPanelForClippingSpace(10000.f),
			  _fov(40)
	{}



	void EditorCamera::RotationByMouse(Maths::Vec2 delta)
	{
		// update euler angles
		_eulerAngle.x += -delta.y * _rotationSpeed;//* deltaClock.getElapsedTime().asSeconds();
		_eulerAngle.y += -delta.x * _rotationSpeed;//* deltaClock.getElapsedTime().asSeconds();

		// update view matrix
		MoveCamera();

	}

	void EditorCamera::RotateCamera(Maths::Vec3 eulerAngle)
	{
		// update translation value
		_eulerAngle += eulerAngle;

		MoveCamera();
	}

	void EditorCamera::TranslationByMouse(Maths::Vec2 delta)
	{
		// update translation value
		_translationValue += GetUpDirection() * delta.y * _translationSpeed;//* deltaClock.getElapsedTime().asSeconds();
		_translationValue += GetRightDirection() * -delta.x * _translationSpeed;//* deltaClock.getElapsedTime().asSeconds();

		// update view matrix
		MoveCamera();

	}

	void EditorCamera::TranslationByMouse(float wheelDelta)
	{

		if (wheelDelta < 0)
			_translationValue += GetForwardDirection() * -_translationSpeed * 5;//* deltaClock.getElapsedTime().asSeconds();
		else
			_translationValue += GetForwardDirection() * _translationSpeed * 5;//* deltaClock.getElapsedTime().asSeconds();
		// update view matrix
		MoveCamera();

	}

	void EditorCamera::TranslationByKeyBoard(float forwardDirectionValue, float rightDirectionValue)
	{

		// move along forward direction
		_translationValue += forwardDirectionValue * GetForwardDirection() * _translationSpeed * 5;
		// * deltaClock.getElapsedTime().asSeconds();

		_translationValue += rightDirectionValue * GetRightDirection() * _translationSpeed * 5;
		// * deltaClock.getElapsedTime().asSeconds();

		// update view matrix
		MoveCamera();

	}

	void EditorCamera::TranslateCamera(Maths::Vec3 translateMovement)
	{
		// update translation value
		_translationValue += translateMovement;

		MoveCamera();
	}

	Maths::Vec3 EditorCamera::GetUpDirection()
	{
		PlatinumEngine::Maths::Mat3 rotationMatrix;

		rotationMatrix.SetRotationMatrix(_eulerAngle);

		return rotationMatrix * PlatinumEngine::Maths::Vec3(0.f, 1.f, 0.f);
	}

	Maths::Vec3 EditorCamera::GetForwardDirection()
	{
		PlatinumEngine::Maths::Mat3 rotationMatrix;

		rotationMatrix.SetRotationMatrix(_eulerAngle);

		return {rotationMatrix[0][2], rotationMatrix[1][2], rotationMatrix[2][2]};
	}

	Maths::Vec3 EditorCamera::GetRightDirection()
	{
		PlatinumEngine::Maths::Mat3 rotationMatrix;

		rotationMatrix.SetRotationMatrix(_eulerAngle);

		return rotationMatrix * PlatinumEngine::Maths::Vec3(1.f, 0.f, 0.f);
	}

	void EditorCamera::MoveCamera()
	{
		// matrices for calculating view matrix
		PlatinumEngine::Maths::Mat4 rotationMatX, rotationMatY, translationMat4;

		// setup matrices
		rotationMatX	.SetRotationMatrix(Maths::Vec3(-_eulerAngle.x, 0, 0 ));
		rotationMatY	.SetRotationMatrix(Maths::Vec3(0, -_eulerAngle.y, 0));
		translationMat4	.SetTranslationMatrix(-_translationValue);

		// calculate the new look at matrix by applying the transformation matrix
		viewMatrix4 = rotationMatX * rotationMatY * translationMat4;
	}

	void EditorCamera::SetFrustumMatrix(float left, const float right, float bottom, float top, float near, float far)
	{

		// update projection matrix
		projectionMatrix4.SetFrustumMatrix( left,  right,  bottom,  top,  near,  far);
	}

	void EditorCamera::SetOrthogonalMatrix(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		// store the near and far panel values
		_nearPanelForClippingSpace = zNear;
		_farPanelForClippingSpace = zFar;
		_fov = -1;

		// update projection matrix
		projectionMatrix4.SetOrthogonalMatrix( left,  right,  bottom,  top,  zNear,  zFar);
	}

	void EditorCamera::SetPerspectiveMatrix(float fovy, float aspect, float near, float far)
	{

		// store the near and far panel values
		_nearPanelForClippingSpace = near;
		_farPanelForClippingSpace = far;
		_fov = fovy;

		// update projection matrix
		projectionMatrix4.SetPerspectiveMatrix( fovy,  aspect,  near,  far);
	}

	const Maths::Vec3& EditorCamera::GetCameraPosition()
	{
		return _translationValue;
	}

	float EditorCamera::GetNearPanelForClippingSpace()
	{
		return _nearPanelForClippingSpace;
	}

	float EditorCamera::GetFarPanelForClippingSpace()
	{
		return _farPanelForClippingSpace;
	}

	float EditorCamera::GetFovForPerspectiveProjection()
	{
		return _fov;
	}


}