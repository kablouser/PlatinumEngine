//
// Created by shawn on 2022/4/22.
//

#pragma once

#include <ComponentComposition/Component.h>
#include <btBulletCollisionCommon.h>

namespace PlatinumEngine
{
	class CapsuleCollider: public Component
	{
	public:
		CapsuleCollider();

		btCollisionShape* GetShape();
	public:
		float radius;
		float height;
		Maths::Vec3 center; // center of the collision shape

	private:
		void OnStart(Scene& scene) override; // construct the bullet collision shape
		void OnEnd(Scene& scene) override; // delete the collision shape

	private:
		btCollisionShape* _capsuleShape; // bullet collision shape
	};
}