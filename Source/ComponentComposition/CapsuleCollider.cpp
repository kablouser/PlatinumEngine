//
// Created by shawn on 2022/4/22.
//

#include <ComponentComposition/CapsuleCollider.h>

namespace PlatinumEngine
{
	void CapsuleCollider::CreateTypeInfo(TypeDatabase& database)
	{
		database.BeginTypeInfo<CapsuleCollider>()
				.WithInherit<Component>()
				.WithField<float>("radius", PLATINUM_OFFSETOF(CapsuleCollider, radius))
				.WithField<float>("height", PLATINUM_OFFSETOF(CapsuleCollider, height));
	}

	CapsuleCollider::CapsuleCollider() :
			radius(0.5f),
			height(2.0f),
			_capsuleShape(radius, height)
	{
	}

	btCollisionShape* CapsuleCollider::GetShape()
	{
		_capsuleShape = btCapsuleShape(radius, height);
		return &_capsuleShape;
	}
}