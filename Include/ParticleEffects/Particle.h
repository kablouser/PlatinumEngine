//
// Created by Matt on 26/04/2022.
//

#pragma once

#include <Maths/Vectors.h>

namespace PlatinumEngine
{
	namespace ParticleEffects
	{
		struct Particle
		{
			Maths::Vec3 position;
			float life;

			// a default constructor for easy creation
			Particle() : position(Maths::Vec3(0.0f, 0.0f, 0.0f)), life(0.0f) {}
		};
	}
}