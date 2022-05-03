//
// Created by shawn on 2022/4/22.
//

#include "ComponentComposition/CapsuleCollider.h"

namespace PlatinumEngine
{
	CapsuleCollider::CapsuleCollider():radius(0.5f),
									   height(1.5f),
									   center(Maths::Vec3(0.f, 0.f, 0.f))
	{

	}

	void CapsuleCollider::OnStart(Scene& scene)
	{
		_capsuleShape = new btCapsuleShape(radius, height);
	}

	void CapsuleCollider::OnEnd(Scene& scene)
	{
		delete _capsuleShape;
	}

	btCollisionShape* CapsuleCollider::GetShape()
	{
		return _capsuleShape;
	}

	void CapsuleCollider::CreateTypeInfo(TypeDatabase& database)
	{
		database.BeginTypeInfo<CapsuleCollider>()
		        .WithInherit<Component>()
				.WithField<float>("radius", PLATINUM_OFFSETOF(CapsuleCollider, radius))
				.WithField<float>("height", PLATINUM_OFFSETOF(CapsuleCollider, height))
				.WithField<Maths::Vec3>("center", PLATINUM_OFFSETOF(CapsuleCollider, center));
	}

}