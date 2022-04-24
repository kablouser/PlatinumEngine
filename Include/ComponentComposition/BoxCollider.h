//
// Created by shawn on 2022/4/22.
//

#pragma once

#include <ComponentComposition/Component.h>
#include <btBulletCollisionCommon.h>
#include <Physics/Physics.h>

namespace PlatinumEngine
{
	class BoxCollider: public Component
	{
	public:
		BoxCollider();

		void SetSize(Maths::Vec3 size);

		Maths::Vec3 GetSize();

		void SetCenter(Maths::Vec3 center);

		Maths::Vec3 GetCenter();

		void CleanUp();

	private:
		btCollisionShape* _boxShape;
		Physics* _physics;

		Maths::Vec3 _size;
		Maths::Vec3 _center;

	private:
		void CreateShape();
	};
}