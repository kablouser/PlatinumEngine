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
				RespawnParticle(p);
				_particleContainer.emplace_back(p);
			}
		}

		/**
		 * Updates all particles
		 * @param deltaTime
		 */
		void ParticleEmitter::UpdateParticles(float deltaTime)
		{
			// Clear what particles are sent to renderer
			_particles.clear();

			// Spawn new particles every frame
			for (unsigned int i = 0; i < numberOfNewParticles; ++i)
			{
				unsigned int unusedParticle = FirstUnusedParticle();
				RespawnParticle(_particleContainer[unusedParticle]);
			}

			for (unsigned int i = 0; i < _numberOfParticles; ++i)
			{
				Particle &p = _particleContainer[i];
				// Reduce its life by delta time
				p.life -= deltaTime;

				// If the particle remains alive, update its position
				if (p.life > 0.0f)
				{
					p.velocity += Maths::Vec3(0.0f,-1.0f, 0.0f) * (float)deltaTime;
					p.position -= p.velocity * deltaTime;

					_particles.emplace_back(p);
				}
			}
		}

		void ParticleEmitter::SetNumberOfParticles(const int numberOfParticles)
		{
			_particles.clear();
			_numberOfParticles = numberOfParticles;
		}

		int ParticleEmitter::GetNumberOfParticles()
		{
			return _numberOfParticles;
		}

		const std::vector<Particle> ParticleEmitter::GetParticles() const
		{
			return _particles;
		}

		unsigned int ParticleEmitter::FirstUnusedParticle()
		{
			for(int i = LastUsedParticle; i < _numberOfParticles; ++i){
				if (_particleContainer[i].life < 0){
					LastUsedParticle = i;
					return i;
				}
			}

			for(int i = 0; i < LastUsedParticle; ++i){
				if (_particleContainer[i].life < 0){
					LastUsedParticle = i;
					return i;
				}
			}

			return 0;
		}

		void ParticleEmitter::RespawnParticle(Particle& p)
		{
			// Respawn by resetting values
			float x = float(rand())/float((RAND_MAX)) * 1.0f - 0.5f;
			float y = -1.0f;
			float z = float(rand())/float((RAND_MAX)) * 1.0f - 0.5f;
//			float x = 0.0f;
//			float y = 0.0f;
//			float z = 2.0f;
			p.position = Maths::Vec3(0,0,0);
			p.velocity = Maths::Vec3(x, y, z);
			p.life = respawnLifetime;
		}

		void ParticleEmitter::SetRespawnLifetime(float lifetime)
		{
			respawnLifetime = lifetime;
		}

		float ParticleEmitter::GetRespawnLifetime()
		{
			return respawnLifetime;
		}

		void ParticleEmitter::SetNumberNewParticles(int numNewParticles)
		{
			numberOfNewParticles = numNewParticles;
		}

		int ParticleEmitter::GetNumberNewParticles()
		{
			return numberOfNewParticles;
		}
	}
}
