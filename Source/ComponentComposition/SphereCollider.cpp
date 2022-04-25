//
// Created by shawn on 2022/4/22.
//

#include "ComponentComposition/SphereCollider.h"

namespace PlatinumEngine
{
	SphereCollider::SphereCollider():_radius(0.5f)
	{
		SetCenter(Maths::Vec3(0.f, 0.f, 0.f));
		CreateShape();
	}

	void SphereCollider::SetRadius(float radius)
	{
		_radius = radius;
	}
	float SphereCollider::GetRadius()
	{
		return _radius;
	}

	void SphereCollider::CreateShape()
	{
		_shape = new btSphereShape(_radius);
	}
}