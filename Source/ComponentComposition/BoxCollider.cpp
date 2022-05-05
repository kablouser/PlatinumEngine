//
// Created by shawn on 2022/4/22.
//

#include <ComponentComposition/BoxCollider.h>
#include <Physics/Physics.h>

namespace PlatinumEngine
{
	void BoxCollider::CreateTypeInfo(TypeDatabase& database)
	{
		database.BeginTypeInfo<BoxCollider>()
		        // Skip Collider, not needed
				.WithInherit<Component>()
				.WithField<Maths::Vec3>("size", PLATINUM_OFFSETOF(BoxCollider, size));
	}

	BoxCollider::BoxCollider() :
			size(Maths::Vec3(1, 1, 1)),
			_boxShape(Physics::ConvertVector(size))
	{
	}

	btCollisionShape* BoxCollider::GetShape()
	{
		// Update size of shape
		_boxShape = btBoxShape(Physics::ConvertVector(size));
		return &_boxShape;
	}
}

