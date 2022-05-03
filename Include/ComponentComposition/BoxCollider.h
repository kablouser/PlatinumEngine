//
// Created by shawn on 2022/4/22.
//

#pragma once

#include <ComponentComposition/Component.h>
#include <btBulletCollisionCommon.h>

namespace PlatinumEngine
{
	class BoxCollider: public Component
	{
	public:
		BoxCollider();

		static void CreateTypeInfo(TypeDatabase& database);
	public:
		Maths::Vec3 size;
		Maths::Vec3 center;

		btCollisionShape* GetShape();
	private:
		void OnStart(Scene& scene) override;
		void OnEnd(Scene& scene) override;

	private:
		btCollisionShape* _boxShape;
	};
}