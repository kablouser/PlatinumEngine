#pragma once

#include "glm/gtc/quaternion.hpp"
#include "Maths/Vectors.h"
#include "Maths/Quaternion.h"
#include "Maths/Matrices.h"
#include "ComponentComposition/Component.h"
#include "Maths/Common.h"

namespace PlatinumEngine
{
	class TransformComponent : public Component
	{
	public:

		// A LOT OF EFFORT, WHERE MOST CASES, YOU DON'T NEED THIS
		//enum class RelativeTo{world, local};

		Maths::Vec3 localPosition;
		Maths::Quaternion localRotation;
		Maths::Mat4 transformationMatrix;
		// uniform scaling has nice properties
		float localScale;

		TransformComponent();

		/**
		 * Local transformation
		 * @param axis to rotate around
		 * @param angle in degrees
		 */
		void Rotate(Maths::Vec3 axis, float angle);
		/**
		 * Rotates by an angle around an axis passing through a point
		 * Local transformation
		 * @param point target point
		 * @param axis to rotate around
		 * @param angle in degrees
		 */
		void RotateAround(Maths::Vec3 point, Maths::Vec3 axis, float angle);
		/**
		 * Calculates a rotation that points in the direction of the target
		 * Local transformation
		 * @param target
		 */
		void LookAt(Maths::Vec3 target);

		void SetLocalToWorldMatrix(Maths::Mat4 LocalToWorld);

		//Transforms direction from local space to parent space
		Maths::Vec3 LocalToParentDirection(Maths::Vec3 dir);
		//Transforms direction from parent space to local space
		Maths::Vec3 ParentToLocalDirection(Maths::Vec3 dir);

		//Transforms direction from world space to local space
		Maths::Mat4 GetWorldToLocalMatrix();
		//Returns matrix with translation, rotation, scale which converts from local to world space
		//aka World Matrix
		//aka the most used matrix
		Maths::Mat4 GetLocalToWorldMatrix();
		//Returns matrix which converts from local to world space
		//aka Local Matrix
		Maths::Mat4 GetLocalToParentMatrix();

		Maths::Vec3 GetLocalForward();
		Maths::Vec3 GetLocalUp();
		Maths::Vec3 GetLocalRight();
	};
}
