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
			Maths::Vec3 velocity;
			float life;
			float distanceFromCamera;

			// a default constructor for easy creation
			Particle() : position(Maths::Vec3(0.0f, 0.0f, 0.0f)),
						 velocity(Maths::Vec3(0.0f, 0.0f, 0.0f)),
						 life(-1.0f),
						 distanceFromCamera(10.0f) {}

			bool operator < (const Particle& p) const
			{
				return (distanceFromCamera > p.distanceFromCamera);
			}
		};
	}
}