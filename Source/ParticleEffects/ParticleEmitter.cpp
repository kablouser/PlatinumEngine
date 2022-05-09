//
// Created by Matt on 26/04/2022.
//

#include <ParticleEffects/ParticleEmitter.h>
#include <algorithm>
#include <Maths/Common.h>

namespace PlatinumEngine
{
	namespace ParticleEffects
	{
		void ParticleEmitter::CreateTypeInfo(TypeDatabase& typeDatabase)
		{
			typeDatabase.BeginTypeInfo<ParticleEmitter>()
			        .WithField<int>("numberOfParticles", PLATINUM_OFFSETOF(ParticleEmitter, numberOfParticles))
					.WithField<float>("respawnLifetime", PLATINUM_OFFSETOF(ParticleEmitter, respawnLifetime))
					.WithField<int>("numberOfNewParticles", PLATINUM_OFFSETOF(ParticleEmitter, numberOfNewParticles))
					.WithField<float>("spawnInterval", PLATINUM_OFFSETOF(ParticleEmitter, spawnInterval))
					.WithField<Maths::Vec3>("actingForce", PLATINUM_OFFSETOF(ParticleEmitter, actingForce))
					.WithField<bool>("useRandomInitPositionX", PLATINUM_OFFSETOF(ParticleEmitter, useRandomInitPositionX))
					.WithField<bool>("useRandomInitPositionY", PLATINUM_OFFSETOF(ParticleEmitter, useRandomInitPositionY))
					.WithField<bool>("useRandomInitPositionZ", PLATINUM_OFFSETOF(ParticleEmitter, useRandomInitPositionZ))
					.WithField<float>("minPositionX", PLATINUM_OFFSETOF(ParticleEmitter, minPositionX))
					.WithField<float>("maxPositionX", PLATINUM_OFFSETOF(ParticleEmitter, maxPositionX))
					.WithField<float>("minPositionY", PLATINUM_OFFSETOF(ParticleEmitter, minPositionY))
					.WithField<float>("maxPositionY", PLATINUM_OFFSETOF(ParticleEmitter, maxPositionY))
					.WithField<float>("minPositionZ", PLATINUM_OFFSETOF(ParticleEmitter, minPositionZ))
					.WithField<float>("maxPositionZ", PLATINUM_OFFSETOF(ParticleEmitter, maxPositionZ))
					.WithField<Maths::Vec3>("initVelocity", PLATINUM_OFFSETOF(ParticleEmitter, initVelocity))
					.WithField<bool>("useRandomInitVelocityX", PLATINUM_OFFSETOF(ParticleEmitter, useRandomInitVelocityX))
					.WithField<bool>("useRandomInitVelocityY", PLATINUM_OFFSETOF(ParticleEmitter, useRandomInitVelocityY))
					.WithField<bool>("useRandomInitVelocityZ", PLATINUM_OFFSETOF(ParticleEmitter, useRandomInitVelocityZ))
					.WithField<float>("minVelocityX", PLATINUM_OFFSETOF(ParticleEmitter, minVelocityX))
					.WithField<float>("maxVelocityX", PLATINUM_OFFSETOF(ParticleEmitter, maxVelocityX))
					.WithField<float>("minVelocityY", PLATINUM_OFFSETOF(ParticleEmitter, minVelocityY))
					.WithField<float>("maxVelocityY", PLATINUM_OFFSETOF(ParticleEmitter, maxVelocityY))
					.WithField<float>("minVelocityZ", PLATINUM_OFFSETOF(ParticleEmitter, minVelocityZ))
					.WithField<float>("maxVelocityZ", PLATINUM_OFFSETOF(ParticleEmitter, maxVelocityZ))
					.WithField<SavedReference<Texture>>("texture", PLATINUM_OFFSETOF(ParticleEmitter, texture))
					.WithField<int>("numRowsInTexture", PLATINUM_OFFSETOF(ParticleEmitter, numRowsInTexture))
					.WithField<int>("numColsInTexture", PLATINUM_OFFSETOF(ParticleEmitter, numColsInTexture));
		}


		ParticleEmitter::ParticleEmitter()
		{
			// Allocate enough space for all our particles, and so we can index into it straight away
			_particleContainer = std::make_unique<std::vector<Particle>>(MaxParticles);
			particles = std::make_unique<std::vector<Particle>>();
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
					if (scaleBy == "Constant")
						p.scale = scaleFactor;
					if (scaleBy == "Life")
						p.scale = scaleFactor * p.life;
					if (scaleBy == "Position")
						p.scale = scaleFactor * Maths::Length(p.position);
					if (scaleBy == "Speed")
						p.scale = scaleFactor * Maths::Length(p.velocity);
					p.textureIndex = Maths::Vec2(0,0);
					float lifeAsFraction = (2.0f*p.life) / (respawnLifetime);
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
			float xVelocity = (useRandomInitVelocityX) ? Maths::Random::GetRandom(minVelocityX, maxVelocityX, !useUniformInitVelocityX) : initVelocity.x;
			float yVelocity = (useRandomInitVelocityY) ? Maths::Random::GetRandom(minVelocityY, maxVelocityY, !useUniformInitVelocityY) : initVelocity.y;
			float zVelocity = (useRandomInitVelocityZ) ? Maths::Random::GetRandom(minVelocityZ, maxVelocityZ, !useUniformInitVelocityZ) : initVelocity.z;
			p.velocity = Maths::Vec3(xVelocity, yVelocity, zVelocity);

			// Init position
			float xPosition = (useRandomInitPositionX) ? Maths::Random::GetRandom(minPositionX, maxPositionX, !useUniformRandomPositionX) : 0.0f;
			float yPosition = (useRandomInitPositionY) ? Maths::Random::GetRandom(minPositionY, maxPositionY, !useUniformRandomPositionY)  : 0.0f;
			float zPosition = (useRandomInitPositionZ) ? Maths::Random::GetRandom(minPositionZ, maxPositionZ, !useUniformRandomPositionZ)  : 0.0f;
			p.position = Maths::Vec3(xPosition, yPosition, zPosition);

			p.life = respawnLifetime;
			p.scale = 1.0f; // Normal scale to begin with
		}
	}
}
