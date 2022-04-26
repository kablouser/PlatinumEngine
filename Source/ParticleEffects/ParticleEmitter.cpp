//
// Created by Matt on 26/04/2022.
//

#include <ParticleEffects/ParticleEmitter.h>

namespace PlatinumEngine
{
	namespace ParticleEffects
	{
		ParticleEmitter::ParticleEmitter()
		{
			_particleContainer.resize(MaxParticles);

			// For now refresh particle list
			for (unsigned int i = 0; i < 10; ++i)
			{
				auto p = Particle();
				p.position.x += (float)i * 1.0f;
				_particles.emplace_back(p);
			}
		}

		void ParticleEmitter::SetNumberOfParticles(const int numberOfParticles)
		{
			_particles.clear();
			_numberOfParticles = numberOfParticles;

			// For now refresh particle list
			for (unsigned int i = 0; i < numberOfParticles; ++i)
			{
				auto p = Particle();
				p.position.x += (float)i * 0.01f;
				_particles.emplace_back(p);
			}
		}

		int ParticleEmitter::GetNumberOfParticles()
		{
			return _numberOfParticles;
		}

		const std::vector<Particle> ParticleEmitter::GetParticles() const
		{
			return _particles;
		}
	}
}
