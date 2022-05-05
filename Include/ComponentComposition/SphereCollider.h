//
// Created by shawn on 2022/4/22.
//

#pragma once

#include <ComponentComposition/Collider.h>

namespace PlatinumEngine
{
	class SphereCollider: public Collider
	{
	public:
		static void CreateTypeInfo(TypeDatabase& database);

		SphereCollider();

		btCollisionShape* GetShape() override;

		float radius;

	private:
		btSphereShape _sphereShape;
	};
}