//
// Created by shawn on 2022/4/22.
//

#include "ComponentComposition/BoxCollider.h"

namespace PlatinumEngine
{
	BoxCollider::BoxCollider(): _size(Maths::Vec3(1,1,1)),
								_center(Maths::Vec3(0,0,0))
								{
									CreateShape();
								}

	void BoxCollider::CreateShape()
	{
		_boxShape = new btBoxShape(_physics->convertVector(_size));
	}

	Maths::Vec3 BoxCollider::GetSize()
	{
		return _size;
	}

	void BoxCollider::SetSize(Maths::Vec3 size)
	{
		_size = size;
		CreateShape();
	}

	void BoxCollider::SetCenter(Maths::Vec3 center)
	{
		_center = center;
	}
	Maths::Vec3 BoxCollider::GetCenter()
	{
		return _center;
	}

	void BoxCollider::CleanUp()
	{
		delete _boxShape;
		_boxShape = nullptr;
	}
}

