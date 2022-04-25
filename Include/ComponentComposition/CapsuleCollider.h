//
// Created by shawn on 2022/4/22.
//

#pragma once

#include <ComponentComposition/Collider.h>

namespace PlatinumEngine
{
	class CapsuleCollider: public Collider
	{
	public:
		CapsuleCollider();

		void SetRadius(float radius);
		float GetRadius();

		void SetHeight(float height);
		float GetHeight();


	private:
		bool _directionX, _directionY, _directionZ;

		float _radius;
		float _height;

	private:
		void CreateShape();
	};
}