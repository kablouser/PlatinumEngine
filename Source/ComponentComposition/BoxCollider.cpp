//
// Created by shawn on 2022/4/22.
//

#include "ComponentComposition/BoxCollider.h"

namespace PlatinumEngine
{
	BoxCollider::BoxCollider(): _size(Maths::Vec3(1,1,1))
								{
									SetCenter(Maths::Vec3(0.f, 0.f, 0.f));
									CreateShape();
								}

	void BoxCollider::CreateShape()
	{
		_shape = new btBoxShape(_physics->convertVector(_size));
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
}

