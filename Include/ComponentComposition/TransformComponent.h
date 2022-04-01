#pragma once

#include "Maths/Vectors.h"
#include "Maths/Quaternion.h"
#include "Maths/Matrices.h"
#include "ComponentComposition/Component.h"

namespace PlatinumEngine
{
	enum relativeTo{WORLD, LOCAL};
	class TransformComponent : public Component
	{
	public:
		//Stored in World space
		Maths::Vec3 position;
		//Stored in Local space
		Maths::Quaternion rotation;
		Maths::Vec3 scale;

	public:
		TransformComponent();
		~TransformComponent();

		void Translate(Maths::Vec3 translation);
		void Translate(Maths::Vec3 translation, relativeTo space);
		void Translate(float x, float y, float z);
		void Translate(float x, float y, float z, relativeTo space);
		void Rotate(Maths::Vec3 euler);
		void Rotate(Maths::Quaternion q);
		void Rotate(Maths::Vec3 axis, float angle);
		void Rotate(Maths::Vec3 euler, relativeTo space);
		void Rotate(Maths::Quaternion q, relativeTo space);
		void Rotate(Maths::Vec3 axis, float angle, relativeTo space);
		void RotateAround(Maths::Vec3 point, Maths::Vec3 axis, float angle);
		void LookAt(Maths::Vec3 target);
		Maths::Vec3 TransformDirection(Maths::Vec3 dir);
		Maths::Vec3 InverseTransformDirection(Maths::Vec3 dir);
		Maths::Vec3 TransformPoint(Maths::Vec3 point);
		Maths::Vec3 InverseTransformPoint(Maths::Vec3 point);

		Maths::Mat4 GetWorldToLocalMatrixNoScale();
		Maths::Mat4 GetWorldToLocalMatrix();
		Maths::Mat4 GetLocalToWorldMatrix();

		Maths::Vec3 GetLocalPosition();
		Maths::Quaternion GetWorldRotation();

		Maths::Vec3 forward();
		void forward(Maths::Vec3 v);
		Maths::Vec3 up();
		void up(Maths::Vec3 v);
		Maths::Vec3 right();
		void right(Maths::Vec3 v);

	private:
		static Maths::Vec3 _forward;
		static Maths::Vec3 _up;
		static Maths::Vec3 _right;
		static float _eps;
		static float _PI;

		Maths::Vec3 _localposition;

	private:
		Maths::Mat4 SetTRInverse(Maths::Vec3 pos, Maths::Quaternion rot);

	};
}
