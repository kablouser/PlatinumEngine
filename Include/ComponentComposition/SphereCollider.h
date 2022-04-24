//
// Created by shawn on 2022/4/22.
//

#pragma once

#include <ComponentComposition/Component.h>
#include <btBulletCollisionCommon.h>
#include <Physics/Physics.h>

namespace PlatinumEngine
{
	class SphereCollider: public Component
	{
	public:
		SphereCollider();

		void SetCenter(Maths::Vec3 center);
		Maths::Vec3 GetCenter();

		void SetRadius(float radius);
		float GetRadius();

		void CleanUp();

	private:
		float _radius;
		Maths::Vec3 _center;

		btCollisionShape* _sphereShape;
		Physics* _physics;

	private:
		void CreateShape();
	};
}