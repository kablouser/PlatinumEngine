//
// Created by shawn on 2022/4/22.
//

#pragma once

#include <ComponentComposition/Collider.h>

namespace PlatinumEngine
{
	class SphereCollider: public Collider
	{
	public:
		SphereCollider();

		void SetRadius(float radius);
		float GetRadius();


	private:
		float _radius;

	private:
		void CreateShape();
	};
}