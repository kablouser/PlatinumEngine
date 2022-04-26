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
		static const int MaxParticles = 1000;
		class ParticleEmitter
		{
		public:
			ParticleEmitter();
			void SetNumberOfParticles(int numberOfParticles);
			[[nodiscard]] int GetNumberOfParticles();
			[[nodiscard]] const std::vector<Particle> GetParticles() const;
		private:
			int _numberOfParticles = 10;
			std::vector<Particle> _particleContainer;
			std::vector<Particle> _particles;
		};
	}
}