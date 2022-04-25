//
// Created by 肖宜辰 on 2022/4/25.
//

#include "ComponentComposition/Collider.h"

namespace PlatinumEngine
{
	void Collider::SetCenter(Maths::Vec3 center)
	{
		_center = center;
	}

	Maths::Vec3 Collider::GetCenter()
	{
		return _center;
	}

	void Collider::CleanUp()
	{
		delete _shape;
	}

	btCollisionShape* Collider::GetShape()
	{
		return _shape;
	}
}

