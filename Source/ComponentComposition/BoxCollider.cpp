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
}

