//
// Created by shawn on 2022/4/22.
//

#pragma once

#include <ComponentComposition/Collider.h>

namespace PlatinumEngine
{
	class CapsuleCollider: public Collider
	{
	public:
		static void CreateTypeInfo(TypeDatabase& database);

		CapsuleCollider();

		btCollisionShape* GetShape() override;

		float radius;
		float height;

	private:
		btCapsuleShape _capsuleShape; // bullet collision shape
	};
}