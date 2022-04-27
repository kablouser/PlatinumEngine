//
// Created by shawn on 2022/4/25.
//

#pragma once

#include <ComponentComposition/Component.h>
#include <btBulletCollisionCommon.h>

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
}