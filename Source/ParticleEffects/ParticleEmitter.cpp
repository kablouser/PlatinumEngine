//
// Created by Matt on 26/04/2022.
//

#include <ParticleEffects/ParticleEmitter.h>
#include <algorithm>

namespace PlatinumEngine
{
	namespace ParticleEffects
	{
		ParticleEmitter::ParticleEmitter()
		{
			// Allocate enough space for all our particles, and so we can index into it straight away
			_particleContainer = std::make_unique<std::vector<Particle>>(MaxParticles);
			particles = std::make_unique<std::vector<Particle>>();
			_mt = std::mt19937(_rd());
		}

		/**
		 * Updates all particles
		 * @param deltaTime
		 */
		void ParticleEmitter::UpdateParticles(float deltaTime, const Maths::Vec3 &cameraPos)
		{
			// Clear what particles are sent to renderer
			if (!particles)
				return;

			particles->clear();

			// Only spawn particles if enough time has passed
			_timeSinceLastSpawn += deltaTime;
			while (_timeSinceLastSpawn > spawnInterval)
			{
				// Spawn a set number of new particles every spawn interval
				for (unsigned int i = 0; i < numberOfNewParticles; ++i)
				{
					// Find which particle to respawn in the particle container, so we don't infinitely spawn too many
					unsigned int indexOfDeadParticle = FirstDeadParticle();

					// Now respawn that particle (hopefully it was dead anyway)
					RespawnParticle(_particleContainer->at(indexOfDeadParticle));
				}

				_timeSinceLastSpawn -= spawnInterval;
			}

			// Edit the particles in the container
			// Store the alive ones separately for renderer (inefficient)
			// TODO: read above
			for (unsigned int i = 0; i < numberOfParticles; ++i)
			{
				// Just so we don't have to index every time
				Particle &p = _particleContainer->at(i);

				// Reduce its life by delta time to start killing it
				p.life -= deltaTime;

				// If the particle remains alive, update its properties
				if (p.life > 0.0f)
				{
					p.velocity += actingForce * deltaTime;
					p.position += p.velocity * deltaTime;
					Maths::Vec3 vector = p.position - cameraPos;
					p.distanceFromCamera = Maths::Length(vector);

					// TODO: Let user choose how to scale particle and by what factor
//					p.scale = 0.5f * p.life;
					p.textureIndex = Maths::Vec2(0,0);
					float lifeAsFraction = p.life / (respawnLifetime);
					int currentIndex = floor(lifeAsFraction * numRowsInTexture * numColsInTexture);
					int j = currentIndex / numColsInTexture;
					int k = currentIndex - j * numColsInTexture;
					p.textureIndex = Maths::Vec2(numRowsInTexture - k - 1, numColsInTexture - j - 1);

					// Adding to list of alive particles
					particles->emplace_back(p);
				}
			}

			// Sort them based on distance from camera
			if (particles->size() > 1)
				std::sort(&particles->at(0), &particles->at(particles->size()-1));
		}

		/**
		 * Finds the first dead particle in the particle container
		 * Is used to index into particle container and respawn the first dead particle we find
		 * @return
		 */
		unsigned int ParticleEmitter::FirstDeadParticle()
		{
			// Fingers crossed it's found this way
			for(unsigned int i = _lastDeadParticle; i < numberOfParticles; ++i)
			{
				if (_particleContainer->at(i).life < 0)
				{
					// Keep track of the particle we just killed as chances are the next one after will be dead
					_lastDeadParticle = i + 1;
					return i;
				}
			}

			// Wasn't found so linear search the rest of the particles :(
			for(int i = 0; i <= _lastDeadParticle; ++i)
			{
				if (_particleContainer->at(i).life < 0)
				{
					// Update so next time we should avoid this loop again
					_lastDeadParticle = i + 1;
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
			float xVelocity = (useRandomInitVelocityX) ? GetRandomFloat(minMaxVelocityX) : initVelocity.x;
			float yVelocity = (useRandomInitVelocityY) ? GetRandomFloat(minMaxVelocityY) : initVelocity.y;
			float zVelocity = (useRandomInitVelocityZ) ? GetRandomFloat(minMaxVelocityZ) : initVelocity.z;
			p.velocity = Maths::Vec3(xVelocity, yVelocity, zVelocity);

			// Init position
			float xPosition = (useRandomInitPositionX) ? GetRandomFloat(minMaxPositionX) : initPosition.x;
			float yPosition = (useRandomInitPositionY) ? GetRandomFloat(minMaxPositionY) : initPosition.y;
			float zPosition = (useRandomInitPositionZ) ? GetRandomFloat(minMaxPositionZ) : initPosition.z;
			p.position = Maths::Vec3(xPosition, yPosition, zPosition);

			p.life = respawnLifetime;
		}

		/**
		 * Simple random number generator to generate a float between two values
		 * @param minMax
		 * @return
		 */
		float ParticleEmitter::GetRandomFloat(float minMax[2])
		{
			// Use next after so distribution is inclusive of max value, i.e.
			// [min, max] instead of [min, max)
			std::uniform_real_distribution<float> dist(minMax[0], std::nextafter(minMax[1], FLT_MAX));
			return dist(_mt);
		}
	}
}
