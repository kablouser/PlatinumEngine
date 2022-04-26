//
// Created by Matt on 26/04/2022.
//

#pragma once

#include <vector>
#include <ParticleEffects/Particle.h>

namespace PlatinumEngine
{
	namespace ParticleEffects
	{
		class ParticleEmitter
		{
		public:
			ParticleEmitter() = default;
		public:
			int numberOfParticles = 0;
		private:
			std::vector<Particle> _particles;
		};
	}
}