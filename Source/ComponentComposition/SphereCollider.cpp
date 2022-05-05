//
// Created by shawn on 2022/4/22.
//

#include "ComponentComposition/SphereCollider.h"

namespace PlatinumEngine
{
	void SphereCollider::CreateTypeInfo(TypeDatabase& database)
	{
		database.BeginTypeInfo<SphereCollider>()
				.WithInherit<Component>()
				.WithField<float>("radius", PLATINUM_OFFSETOF(SphereCollider, radius));
	}

	SphereCollider::SphereCollider() :
			radius(1.f),
			_sphereShape(radius)
	{
	}

	btCollisionShape* SphereCollider::GetShape()
	{
		_sphereShape = btSphereShape(radius);
		return &_sphereShape;
	}
}