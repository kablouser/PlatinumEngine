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
			  _translationValue(0.f, 0.f, 1.f),
			  isOrthogonal(false)
	{}


	void EditorCamera::RotationByMouse(glm::vec2 newMousePosition)
	{

		// calculate the movement of mouse
		glm::vec2 delta = newMousePosition - _initialMousePosition;

		_eulerAngle.x += delta.y * _rotationSpeed;//* deltaClock.getElapsedTime().asSeconds();
		_eulerAngle.y += delta.x * _rotationSpeed;//* deltaClock.getElapsedTime().asSeconds();

		// update mouse initial position
		_initialMousePosition = newMousePosition;

		// update view matrix
		MoveCamera(PlatinumEngine::Maths::Vec3(0.0, 0.0, 0),
				PlatinumEngine::Maths::Vec3(0.0, 0.0, 0.0));

	}

	void EditorCamera::TranslationByMouse(glm::vec2 newMousePosition)
	{

		// calculate the movement of mouse
		glm::vec2 delta = newMousePosition - _initialMousePosition;

		_translationValue += GetUpDirection() * delta.y * _translationSpeed;//* deltaClock.getElapsedTime().asSeconds();
		_translationValue +=
				GetRightDirection() * delta.x * (-_translationSpeed);//* deltaClock.getElapsedTime().asSeconds();



		// update mouse initial position
		_initialMousePosition = newMousePosition;



		// update view matrix
		MoveCamera(PlatinumEngine::Maths::Vec3(0.0, 0.0, 0),
				PlatinumEngine::Maths::Vec3(0.0, 0.0, 0.0));

	}

	void EditorCamera::TranslationByMouse(float wheelDelta)
	{

		if (wheelDelta > 0)
			_translationValue +=
					GetForwardDirection() * (-_translationSpeed) * 5.f;//* deltaClock.getElapsedTime().asSeconds();
		else
			_translationValue +=
					GetForwardDirection() * (_translationSpeed) * 5.f;//* deltaClock.getElapsedTime().asSeconds();
		// update view matrix
		MoveCamera(PlatinumEngine::Maths::Vec3(0.0, 0.0, 0), PlatinumEngine::Maths::Vec3(0.0, 0.0, 0.0));

	}

	void EditorCamera::TranslationByKeyBoard(KeyType type)
	{

		// move along forward direction

		// get forward direction
		switch (type)
		{
		case KeyType::down:
			_translationValue +=
					GetForwardDirection() * _translationSpeed * 5.f;//* deltaClock.getElapsedTime().asSeconds();
			break;

		case KeyType::up:
			_translationValue +=
					GetForwardDirection() * (-_translationSpeed) * 5.f;//* deltaClock.getElapsedTime().asSeconds();
			break;

		case KeyType::left:
			_translationValue +=
					GetRightDirection() * _translationSpeed * 5.f;// * deltaClock.getElapsedTime().asSeconds();
			break;

		case KeyType::right:
			_translationValue +=
					GetRightDirection() * (-_translationSpeed) * 5.f;// * deltaClock.getElapsedTime().asSeconds();
			break;
		}

		// update view matrix
		MoveCamera(PlatinumEngine::Maths::Vec3(0.0, 0.0, 0),
				PlatinumEngine::Maths::Vec3(0.0, 0.0, 0.0));

	}

	void EditorCamera::UpdateInitialMousePosition(glm::vec2 initialMousePosition)
	{
		_initialMousePosition = initialMousePosition;

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

	void EditorCamera::MoveCamera(PlatinumEngine::Maths::Vec3 eulerAngle,
			PlatinumEngine::Maths::Vec3 translationValue)
	{
		// update euler angle
		_eulerAngle += eulerAngle;

		PlatinumEngine::Maths::Mat4 rotationMat4, translationMat4;

		rotationMat4.SetRotationMatrix(eulerAngle);


		// update translation value
		_translationValue.x += translationValue.x;
		_translationValue.y += translationValue.y;
		_translationValue.z += translationValue.z;

		// get translation matrix
		translationMat4.SetTranslationMatrix(translationValue);



		// calculate the new look at matrix by applying the transformation matrix
		viewMatrix4 = rotationMat4 * translationMat4;


	}



	void EditorCamera::SetFrustumMatrix(float left, float right, float bottom, float top, float near, float far)
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

}