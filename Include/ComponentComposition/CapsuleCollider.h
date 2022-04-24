//
// Created by shawn on 2022/4/22.
//

#pragma once

#include <ComponentComposition/Component.h>
#include <btBulletCollisionCommon.h>
#include <Physics/Physics.h>

namespace PlatinumEngine
{
	class CapsuleCollider: public Component
	{
	public:
		CapsuleCollider();

		void SetCenter(Maths::Vec3 center);
		Maths::Vec3 GetCenter();

		void SetRadius(float radius);
		float GetRadius();

		void SetHeight(float height);
		float GetHeight();

		void CleanUp();

	private:
		bool _directionX, _directionY, _directionZ;

		float _radius;
		float _height;
		Maths::Vec3 _center;

		btCollisionShape* _capsuleShape;
		Physics* _physics;

	private:
		void CreateShape();
	};
}