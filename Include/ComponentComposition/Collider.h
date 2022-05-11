//
// Created by shawn on 2022/4/25.
//

#pragma once

#include <ComponentComposition/Component.h>
#include <btBulletCollisionCommon.h>

namespace PlatinumEngine
{
	class Collider : public Component
	{
	public:
		virtual btCollisionShape* GetShape() = 0;
	};
}