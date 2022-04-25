//
// Created by shawn on 2022/4/25.
//

#pragma once

#include <ComponentComposition/Component.h>
#include <Physics/Physics.h>

namespace PlatinumEngine
{
	class PhysicalMaterial
	{
	public:
		//between 0 and 1
		float friction = 0.5f;
		float bounciness = 0.5f;

		PhysicalMaterial() {};

		/**
		 *
		 * @param f: friction
		 * @param b: bounciness
		 */
		PhysicalMaterial(float f, float b)
		{
			f = friction;
			b = bounciness;
		}
	};

	class Collider: public Component
	{
	public:
		//Every collider has a position
		//This is realtive to the transforms position
		void SetCenter(Maths::Vec3 center);
		Maths::Vec3 GetCenter();

		void CleanUp();

	protected:
		btCollisionShape* _shape;
		PhysicalMaterial _material;

		Maths::Vec3 _center;
	};
}