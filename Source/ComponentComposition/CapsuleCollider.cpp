//
// Created by shawn on 2022/4/22.
//

#include "ComponentComposition/CapsuleCollider.h"

namespace PlatinumEngine
{
	CapsuleCollider::CapsuleCollider():_radius(0.5f),
									   _height(1.5f)
	{
		SetCenter(Maths::Vec3(0.f, 0.f, 0.f));
		CreateShape();
	}

	void CapsuleCollider::SetRadius(float radius)
	{
		_radius = radius;
	}
	float CapsuleCollider::GetRadius()
	{
		return _radius;
	}

	void CapsuleCollider::SetHeight(float height)
	{
		_height = height;
	}
	float CapsuleCollider::GetHeight()
	{
		return _height;
	}

	void CapsuleCollider::CreateShape()
	{
		_shape = new btCapsuleShape(_radius, _height);
	}

}