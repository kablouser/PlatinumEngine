//
// Created by shawn on 2022/4/22.
//

#pragma once

#include <ComponentComposition/Collider.h>

namespace PlatinumEngine
{
	class BoxCollider: public Collider
	{
	public:
		static void CreateTypeInfo(TypeDatabase& database);

		BoxCollider();

		btCollisionShape* GetShape() override;

		Maths::Vec3 size;

	private:
		btBoxShape _boxShape;
	};
}