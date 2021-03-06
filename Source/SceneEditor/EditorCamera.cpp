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
// 	      middle wheel (scroll) 		  -> speedup/slowdown translation
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
			: _quaternion(1.f,0.f, 0.f, 0.f),
			  _translationValue(0.f, 0.f, -5.f),
			  viewMatrix4(1.0),
			  projectionMatrix4(1.f),
			  isOrthogonal(false)
	{
		// view matrix is not correct yet...
		UpdateViewMatrix();
	}



	void EditorCamera::RotationByMouse(Maths::Vec2 delta)
	{
		// rotate around with an inverse up direction (which is doing opposite movement of the up direction of the camera)
		Maths::Vec3 upInverse = Maths::Quaternion::Inverse(_quaternion) * Maths::Vec3(0.f,1.f,0.f);

		// multiply it on the left-hand side of the rotation matrix
		_quaternion = _quaternion * Maths::Quaternion(cosf(delta.x * _rotationSpeed),
				sinf(delta.x * _rotationSpeed) * Maths::Normalise(upInverse));


		// rotate around right direction of camera axis
		Maths::Vec3 right = GetRightDirection() ;

		// multiply it on the right-hand side of the rotation matrix
		_quaternion = Maths::Quaternion(cosf(delta.y * _rotationSpeed),
				sinf(delta.y * _rotationSpeed) * Maths::Normalise(right)) * _quaternion;

		// update view matrix
		UpdateViewMatrix();
	}


	void EditorCamera::TranslationByMouse(Maths::Vec2 delta)
	{
		// update translation value
		_translationValue += GetUpDirection() * delta.y * _translationSpeed * speedScale;
		_translationValue += GetRightDirection() * -delta.x * _translationSpeed * speedScale;

		// update view matrix
		UpdateViewMatrix();
	}

	void EditorCamera::ChangeSpeedScale(float wheelDelta)
	{
		// update translation value
		constexpr float SCROLL_SPEED = 1.1f;
		if (0 < wheelDelta)
			speedScale *= SCROLL_SPEED;
		else if (wheelDelta < 0)
			speedScale /= SCROLL_SPEED;
	}

	void EditorCamera::TranslationByKeyBoard(float forwardDirectionValue, float rightDirectionValue)
	{
		// change the rendering view size to zoom in orthogonal mode?
		if(forwardDirectionValue > 0)
		{
			if(isOrthogonal == true)
				_IsProjectionMatrixUsed = false;
			viewPortSize *= (1.2f);
		}
		else if(forwardDirectionValue < 0)
		{
			if(isOrthogonal == true)
				_IsProjectionMatrixUsed = false;
			viewPortSize /= (1.2f);
		}

		// move along forward direction
		_translationValue += forwardDirectionValue * GetForwardDirection() * _translationSpeed * 10.f * speedScale;
		// * deltaClock.getElapsedTime().asSeconds();

		_translationValue += rightDirectionValue * GetRightDirection() * _translationSpeed * 10.f * speedScale;
		// * deltaClock.getElapsedTime().asSeconds();

		// update view matrix
		UpdateViewMatrix();
	}


	Maths::Vec3 EditorCamera::GetUpDirection()
	{
		PlatinumEngine::Maths::Mat3 rotationMatrix;

		rotationMatrix.SetRotationMatrix(_quaternion);

		return {rotationMatrix[1][0], rotationMatrix[1][1], rotationMatrix[1][2]};
	}

	Maths::Vec3 EditorCamera::GetForwardDirection()
	{
		PlatinumEngine::Maths::Mat3 rotationMatrix;

		rotationMatrix.SetRotationMatrix(_quaternion);

		return {rotationMatrix[2][0], rotationMatrix[2][1], rotationMatrix[2][2]};
	}

	Maths::Vec3 EditorCamera::GetRightDirection()
	{
		PlatinumEngine::Maths::Mat3 rotationMatrix;

		rotationMatrix.SetRotationMatrix(_quaternion);

		return {rotationMatrix[0][0], rotationMatrix[0][1], rotationMatrix[0][2]};
	}

	void EditorCamera::UpdateViewMatrix()
	{
		// matrices for calculating view matrix
		PlatinumEngine::Maths::Mat4 rotation, translation;

		// setup matrices
		rotation	.SetRotationMatrix(Maths::Quaternion::Inverse(_quaternion));
		translation	.SetTranslationMatrix(-_translationValue);

		// calculate the new look at matrix by applying the transformation matrix
		viewMatrix4 = rotation * translation;

		// mark the view matrix as not used
		_IsViewMatrixUsed = false;
	}

	void EditorCamera::SetOrthogonalMatrix(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		// update projection matrix
		projectionMatrix4.SetOrthogonalMatrix( left,  right,  bottom,  top,  zNear,  zFar);

		_IsProjectionMatrixUsed = false;
	}

	void EditorCamera::SetPerspectiveMatrix(float fovy, float aspect, float near, float far)
	{
		// update projection matrix
		projectionMatrix4.SetPerspectiveMatrix( fovy,  aspect,  near,  far);

		_IsProjectionMatrixUsed = false;
	}

	const Maths::Vec3& EditorCamera::GetCameraPosition()
	{
		// translation value is the camera position
		return _translationValue;
	}

	void EditorCamera::UpdateCameraQuaternion()
	{
		Maths::Vec3 translation;
		Maths::Quaternion quaternion;
		float scale;
		// get translation and quaternion applied on the camera
		viewMatrix4.Decompose(&translation, &quaternion, &scale);

		quaternion =  Maths::Quaternion::Inverse(quaternion);

		if(quaternion == _quaternion)
			return;

		_quaternion = quaternion;

		// mark the view matrix as not used
		_IsViewMatrixUsed = false;
	}

	void EditorCamera::MarkViewMatrixAsUsed()
	{
		// mark the view matrix as used
		_IsViewMatrixUsed = true;
	}

	bool EditorCamera::CheckIfViewMatrixUsed()
	{
		return _IsViewMatrixUsed;
	}

	void EditorCamera::MarkProjectionMatrixAsUsed()
	{
		_IsProjectionMatrixUsed = true;
	}

	bool EditorCamera::CheckIfProjectionMatrixUsed()
	{
		return _IsProjectionMatrixUsed;
	}

	Maths::Mat4 EditorCamera::GetRotationOnlyViewMatrix()
	{
		Maths::Mat4 matrix;
		matrix.SetRotationMatrix(_quaternion);
		return matrix;
	}

	void EditorCamera::SetCameraPosition(Maths::Vec3 newPosition)
	{
		_translationValue = newPosition;
		UpdateViewMatrix();
	}

}