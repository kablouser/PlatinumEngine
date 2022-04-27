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
			// Allocate enough space for all our particles, and so we can index into it straight away
			_particleContainer.resize(MaxParticles);
		}

		/**
		 * Updates all particles
		 * @param deltaTime
		 */
		void ParticleEmitter::UpdateParticles(float deltaTime, const Maths::Vec3 &cameraPos)
		{
			// Clear what particles are sent to renderer
			_particles.clear();

			// Spawn a set number of new particles every frame
			for (unsigned int i = 0; i < numberOfNewParticles; ++i)
			{
				// Find which particle to respawn in the particle container, so we don't infinitely spawn too many
				unsigned int indexOfDeadParticle = FirstDeadParticle();

				// Now respawn that particle (hopefully it was dead anyway)
				RespawnParticle(_particleContainer[indexOfDeadParticle]);
			}

			// Edit the particles in the container
			// Store the alive ones separately for renderer (inefficient)
			// TODO: the above but better
			for (unsigned int i = 0; i < numberOfParticles; ++i)
			{
				// Just so we don't have to index every time
				Particle &p = _particleContainer[i];

				// Reduce its life by delta time to start killing it
				p.life -= deltaTime;

				// If the particle remains alive, update its properties
				if (p.life >= 0.0f)
				{
					p.velocity += -actingForce * (float)deltaTime;
					p.position -= p.velocity * deltaTime;
					Maths::Vec3 vector = p.position - cameraPos;
					p.distanceFromCamera = Maths::Length(vector);

					// Adding to list of alive particles
					_particles.emplace_back(p);
				}
			}

			// Sort them based on distance from camera
			std::sort(&_particles[0], &_particles[_particles.size()]);
		}

		std::vector<Particle> ParticleEmitter::GetParticles() const
		{
			return _particles;
		}

		/**
		 * Finds the first dead particle in the particle container
		 * Is used to index into particle container and respawn the first dead particle we find
		 * @return
		 */
		unsigned int ParticleEmitter::FirstDeadParticle()
		{
			// Fingers crossed it's found this way
			for(int i = _lastDeadParticle; i < numberOfParticles; ++i)
			{
				if (_particleContainer[i].life < 0)
				{
					// Keep track of the particle we just killed as chances are the next one after will be dead
					_lastDeadParticle = i;
					return i;
				}
			}

			// Wasn't found so linear search the rest of the particles :(
			for(int i = 0; i < _lastDeadParticle; ++i)
			{
				if (_particleContainer[i].life < 0)
				{
					// Update so next time we should avoid this loop again
					_lastDeadParticle = i;
					return i;
				}
			}

			// Just overwrite the first one it will still look nice
			return 0;
		}

		/**
		 * Given a particle in the particle container, reset its properties
		 * @param p
		 */
		void ParticleEmitter::RespawnParticle(Particle& p)
		{
			// Respawn by resetting values
			// Init velocity values
			float x = (useRandomInitVelocityX) ? GetRandomFloat(minMaxVelocityX) : initVelocity.x;
			float y = (useRandomInitVelocityY) ? GetRandomFloat(minMaxVelocityY) : initVelocity.y;
			float z = (useRandomInitVelocityZ) ? GetRandomFloat(minMaxVelocityZ) : initVelocity.z;
			// For some reason positive is the wrong way :(, probably a problem in the shader (also acting force
			// negated in update function)
			p.velocity = Maths::Vec3(-x, -y, -z);

			// Init position
			p.position = Maths::Vec3(0,0,0);

			p.life = respawnLifetime;
		}

		/**
		 * Simple random number generator to generate a float between two values
		 * @param minMax
		 * @return
		 */
		float ParticleEmitter::GetRandomFloat(float minMax[2])
		{
			// Swap values to ensure generator works as expected
			if (minMax[0] < minMax[1])
			{
				float temp = minMax[1];
				minMax[1] = minMax[0];
				minMax[0] = temp;
			}

			std::random_device rd;
			std::mt19937 mt(rd());
			// Use next after so distribution is inclusive of max value, i.e.
			// [min, max] instead of [min, max)
			std::uniform_real_distribution<float> dist(minMax[0], std::nextafter(minMax[1], FLT_MAX));
			return dist(mt);
		}
	}
}
