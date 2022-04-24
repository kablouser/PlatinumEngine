//
// Created by shawn on 2022/4/22.
//

#include "ComponentComposition/CapsuleCollider.h"

namespace PlatinumEngine
{
	CapsuleCollider::CapsuleCollider():_radius(0.5f),
									   _height(1.5f),
									   _center(Maths::Vec3(0.f, 0.f, 0.f))

	{
		CreateShape();
	}


	void CapsuleCollider::SetCenter(Maths::Vec3 center)
	{
		_center = center;
	}
	Maths::Vec3 CapsuleCollider::GetCenter()
	{
		return _center;
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
		if(_directionX)
			_capsuleShape = new btCapsuleShapeX(_physics->convertScalar(_radius), _physics->convertScalar(_height));
		else if(_directionY)
			_capsuleShape = new btCapsuleShape(_physics->convertScalar(_radius), _physics->convertScalar(_height));
		else if(_directionZ)
			_capsuleShape = new btCapsuleShapeZ(_physics->convertScalar(_radius), _physics->convertScalar(_height));
	}

	void CapsuleCollider::CleanUp()
	{
		delete _capsuleShape;
		_capsuleShape = nullptr;
	}
}