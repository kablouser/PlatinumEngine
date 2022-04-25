//
// Created by shawn on 2022/4/22.
//

#pragma once

#include <ComponentComposition/Collider.h>

namespace PlatinumEngine
{
	class BoxCollider: public Collider
	{
	public:
		BoxCollider();

		void SetSize(Maths::Vec3 size);

		Maths::Vec3 GetSize();


	private:
		Maths::Vec3 _size;

	private:
		void CreateShape();
	};
}