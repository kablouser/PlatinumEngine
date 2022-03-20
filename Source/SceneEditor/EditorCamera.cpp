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


#include "SceneEditor/EditorCamera.h"



namespace PlatinumEngine
{

	EditorCamera::EditorCamera()
			: _eulerAngle(0.f, 0.f, 0.f),
			  _translationValue(0.f, 0.f, 10.f),
			  viewMatrix4(1.f),
			  projectionMatrix4(1.f),
			  isOrthogonal(false)
	{}


	void EditorCamera::RotationByMouse(Maths::Vec2 delta)
	{
		// update euler angles
		_eulerAngle.x += delta.y * -_rotationSpeed;//* deltaClock.getElapsedTime().asSeconds();
		_eulerAngle.y += delta.x * _rotationSpeed;//* deltaClock.getElapsedTime().asSeconds();



		// update view matrix
		MoveCamera(PlatinumEngine::Maths::Vec3(0.0, 0.0, 0),
				PlatinumEngine::Maths::Vec3(0.0, 0.0, 0.0));

	}

	void EditorCamera::RotateCamera(Maths::Vec3 rotationMovement)
	{
		// update translation value
		_eulerAngle += rotationMovement;

		MoveCamera(PlatinumEngine::Maths::Vec3(0.0, 0.0, 0.0),
				PlatinumEngine::Maths::Vec3(0.0, 0.0, 0.0));
	}

	void EditorCamera::TranslationByMouse(Maths::Vec2 delta)
	{
		// update translation value
		_translationValue += GetUpDirection() * -delta.y * _translationSpeed;//* deltaClock.getElapsedTime().asSeconds();
		_translationValue += GetRightDirection() * -delta.x * _translationSpeed;//* deltaClock.getElapsedTime().asSeconds();


		// update view matrix
		MoveCamera(PlatinumEngine::Maths::Vec3(0.0, 0.0, 0.0),
				PlatinumEngine::Maths::Vec3(0.0, 0.0, 0.0));

	}

	void EditorCamera::TranslationByMouse(float wheelDelta)
	{

		if (wheelDelta > 0)
			_translationValue += GetForwardDirection() * (-_translationSpeed) * 10.f;//* deltaClock.getElapsedTime().asSeconds();
		else
			_translationValue += GetForwardDirection() * (_translationSpeed) * 10.f;//* deltaClock.getElapsedTime().asSeconds();
		// update view matrix
		MoveCamera(PlatinumEngine::Maths::Vec3(0.0, 0.0, 0.0),
				PlatinumEngine::Maths::Vec3(0.0, 0.0, 0.0));

	}

	void EditorCamera::TranslationByKeyBoard(float forwardDirectionValue, float rightDirectionValue)
	{

		// move along forward direction

		_translationValue += forwardDirectionValue * GetForwardDirection() * _translationSpeed * 5.f;
		// * deltaClock.getElapsedTime().asSeconds();


		_translationValue += rightDirectionValue * GetRightDirection() * -_translationSpeed * 5.f;
		// * deltaClock.getElapsedTime().asSeconds();


		// update view matrix
		MoveCamera(PlatinumEngine::Maths::Vec3(0.0, 0.0, 0),
				PlatinumEngine::Maths::Vec3(0.0, 0.0, 0.0));

	}

	void EditorCamera::TranslateCamera(Maths::Vec3 translateMovement)
	{
		// update translation value
		_translationValue += translateMovement;

		MoveCamera(PlatinumEngine::Maths::Vec3(0.0, 0.0, 0.0),
				PlatinumEngine::Maths::Vec3(0.0, 0.0, 0.0));
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

		return rotationMatrix * PlatinumEngine::Maths::Vec3(0.f, 0.f, 1.f);

	}

	Maths::Vec3 EditorCamera::GetRightDirection()
	{
		PlatinumEngine::Maths::Mat3 rotationMatrix;

		rotationMatrix.SetRotationMatrix(_eulerAngle);

		return rotationMatrix * PlatinumEngine::Maths::Vec3(1.f, 0.f, 0.f);

	}

	void EditorCamera::MoveCamera(PlatinumEngine::Maths::Vec3 eulerAngle, PlatinumEngine::Maths::Vec3 translationValue)
	{
		// update euler angle
		_eulerAngle += eulerAngle;

		PlatinumEngine::Maths::Mat4 rotationMat4, translationMat4;

		rotationMat4.SetRotationMatrix(-_eulerAngle);


		// update translation value
		_translationValue.x += translationValue.x;
		_translationValue.y += translationValue.y;
		_translationValue.z += translationValue.z;

		// get translation matrix
		translationMat4.SetTranslationMatrix(-_translationValue - _cameraPosition);


		// calculate the new look at matrix by applying the transformation matrix
		viewMatrix4 = rotationMat4 * translationMat4;


	}


	void EditorCamera::SetFrustumMatrix(float left, const float right, float bottom, float top, float near, float far)
	{

		projectionMatrix4.SetFrustumMatrix( left,  right,  bottom,  top,  near,  far);

	}

	void EditorCamera::SetOrthogonalMatrix(float left, float right, float bottom, float top, float zNear, float zFar)
	{

		projectionMatrix4.SetOrthogonalMatrix( left,  right,  bottom,  top,  zNear,  zFar);

	}

	void EditorCamera::SetPerspectiveMatrix(float fovy, float aspect, float near, float far)
	{
		projectionMatrix4.SetPerspectiveMatrix( fovy,  aspect,  near,  far);

	}

	void EditorCamera::SetCameraPosition(Maths::Vec3 cameraPosition)
	{

		_cameraPosition = cameraPosition;

		MoveCamera(PlatinumEngine::Maths::Vec3(0.0, 0.0, 0.0),
				PlatinumEngine::Maths::Vec3(0.0, 0.0, 0.0));

	}
}