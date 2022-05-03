//
// Created by shawn on 2022/4/22.
//

#include "ComponentComposition/BoxCollider.h"
#include "Physics/Physics.h"
namespace PlatinumEngine
{
	BoxCollider::BoxCollider(): size(Maths::Vec3(1,1,1)),
								center(Maths::Vec3(0.f, 0.f, 0.f))
								{

								}

	void BoxCollider::OnStart(Scene& scene)
	{
		_boxShape = new btBoxShape(Physics::ConvertVector(size));
	}

	void BoxCollider::OnEnd(Scene& scene)
	{
		delete _boxShape;
	}

	btCollisionShape* BoxCollider::GetShape()
	{
		return _boxShape;
	}

	void BoxCollider::CreateTypeInfo(TypeDatabase& database)
	{
		database.BeginTypeInfo<BoxCollider>()
		        .WithInherit<Component>()
				.WithField<Maths::Vec3>("size", PLATINUM_OFFSETOF(BoxCollider, size))
				.WithField<Maths::Vec3>("center", PLATINUM_OFFSETOF(BoxCollider, center));
	}
}

