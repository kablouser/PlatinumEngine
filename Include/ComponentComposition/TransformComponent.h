#pragma once

#include "glm/gtc/quaternion.hpp"
#include "Maths/Vectors.h"
#include "Maths/Quaternion.h"
#include "Maths/Matrices.h"
#include "ComponentComposition/Component.h"
#include "ComponentComposition/GameObject.h"
#include "Maths/Common.h"

namespace PlatinumEngine
{
	class TransformComponent : public Component
	{
	public:
		//Stored in World space
		Maths::Vec3 position;
		//Stored in Local space
		Maths::Quaternion rotation;
		Maths::Vec3 scale;
		enum class RelativeTo{world, local};

	public:
		TransformComponent();
		~TransformComponent();

		void Translate(Maths::Vec3 translation, RelativeTo space=RelativeTo::world);
		void Translate(float x, float y, float z, RelativeTo space=RelativeTo::world);
		void Rotate(Maths::Vec3 euler, RelativeTo space=RelativeTo::local);
		void Rotate(Maths::Quaternion q, RelativeTo space=RelativeTo::local);
		void Rotate(Maths::Vec3 axis, float angle, RelativeTo space=RelativeTo::local);
		void RotateAround(Maths::Vec3 point, Maths::Vec3 axis, float angle);
		void LookAt(Maths::Vec3 target);

		Maths::Vec3 TransformDirection(Maths::Vec3 dir);
		Maths::Vec3 InverseTransformDirection(Maths::Vec3 dir);

		Maths::Mat4 GetWorldToLocalMatrix();
		Maths::Mat4 GetLocalToWorldMatrix();

		Maths::Vec3 GetLocalPosition();

		Maths::Vec3 GetForward();
		Maths::Vec3 GetUp();
		Maths::Vec3 GetRight();
	};
}
