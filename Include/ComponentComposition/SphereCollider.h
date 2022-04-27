//
// Created by shawn on 2022/4/22.
//

#pragma once

#include <ComponentComposition/Component.h>
#include <btBulletCollisionCommon.h>

namespace PlatinumEngine
{
	class SphereCollider: public Component
	{
	public:
		SphereCollider();

		btCollisionShape* GetShape();
	public:
		float radius;
		Maths::Vec3 center;

	private:
		void OnStart(Scene& scene);
		void OnEnd(Scene& scene);

	private:
		btCollisionShape* _sphereShape;
	};
}