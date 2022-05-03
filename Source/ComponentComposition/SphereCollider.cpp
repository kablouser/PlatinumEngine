//
// Created by shawn on 2022/4/22.
//

#include "ComponentComposition/SphereCollider.h"

namespace PlatinumEngine
{
	SphereCollider::SphereCollider():radius(1.f), center(Maths::Vec3(0.f, 0.f, 0.f))
	{
	}

	void SphereCollider::OnStart(Scene& scene)
	{
		_sphereShape = new btSphereShape(radius);
	}

	void SphereCollider::OnEnd(Scene& scene)
	{
		delete _sphereShape;
	}

	btCollisionShape* SphereCollider::GetShape()
	{
		return _sphereShape;
	}

	void SphereCollider::CreateTypeInfo(TypeDatabase& database)
	{
		database.BeginTypeInfo<SphereCollider>()
		        .WithInherit<Component>()
				.WithField<float>("radius", PLATINUM_OFFSETOF(SphereCollider, radius))
				.WithField<Maths::Vec3>("center", PLATINUM_OFFSETOF(SphereCollider, center));
	}
}