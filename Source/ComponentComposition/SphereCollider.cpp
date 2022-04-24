//
// Created by shawn on 2022/4/22.
//

#include "ComponentComposition/SphereCollider.h"

namespace PlatinumEngine
{
	SphereCollider::SphereCollider():_center(Maths::Vec3(0.f,0.f,0.f)),
									 _radius(0.5f)
	{
		CreateShape();
	}

	void SphereCollider::SetCenter(Maths::Vec3 center)
	{
		_center = center;
	}
	Maths::Vec3 SphereCollider::GetCenter()
	{
		return _center;
	}

	void SphereCollider::SetRadius(float radius)
	{
		_radius = radius;
	}
	float SphereCollider::GetRadius()
	{
		return _radius;
	}

	void SphereCollider::CleanUp()
	{
		delete _sphereShape;
		_sphereShape = nullptr;
	}

	void SphereCollider::CreateShape()
	{
		_sphereShape = new btSphereShape(_physics->convertScalar(_radius));
	}
}