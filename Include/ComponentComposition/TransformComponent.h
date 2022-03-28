#pragma once

#include "Maths/Vectors.h"
#include "Maths/Quaternion.h"
#include "Maths/Matrices.h"
#include "ComponentComposition/Component.h"

namespace PlatinumEngine
{
	class TransformComponent : public Component
	{
	public:
		Maths::Vec3 position;
		Maths::Quaternion rotation;
		Maths::Vec3 up;
		Maths::Vec3 forward;
		Maths::Vec3 right;
		Maths::Vec3 scale;

	public:
		TransformComponent();
		~TransformComponent();

		void Translate(Maths::Vec3 translation);
		void Rotate(Maths::Vec3 euler);
		void Rotate(Maths::Quaternion q);
		void RotateAround(Maths::Vec3 point, Maths::Vec3 axis, float angle);
		void Scale(Maths::Vec3 scale);
		void LookAt(Maths::Vec3 target);
	};
}
