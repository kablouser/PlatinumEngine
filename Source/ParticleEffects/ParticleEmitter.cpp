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

			// Seed random generator
			std::random_device rd;
			std::mt19937 mt(rd());
		}

		/**
		 * Updates all particles
		 * @param deltaTime
		 */
		void ParticleEmitter::UpdateParticles(float deltaTime, const Maths::Vec3 &cameraPos)
		{
			// Clear what particles are sent to renderer
			_particles.clear();

			// Spawn new particles every frame
			for (unsigned int i = 0; i < numberOfNewParticles; ++i)
			{
				unsigned int unusedParticle = FirstUnusedParticle();
				RespawnParticle(_particleContainer[unusedParticle]);
			}

			for (unsigned int i = 0; i < numberOfParticles; ++i)
			{
				Particle &p = _particleContainer[i];
				// Reduce its life by delta time
				p.life -= deltaTime;

				// If the particle remains alive, update its position
				if (p.life >= 0.0f)
				{
					p.velocity += Maths::Vec3(0.0f,-1.0f, 0.0f) * (float)deltaTime;
					p.position -= p.velocity * deltaTime;
					Maths::Vec3 vector = p.position - cameraPos;
					p.distanceFromCamera = Maths::Length(vector);

					_particles.emplace_back(p);
				}
			}

			// Sort them based on distance from camera
			std::sort(&_particles[0], &_particles[_particles.size()]);
		}

		const std::vector<Particle> ParticleEmitter::GetParticles() const
		{
			return _particles;
		}

		unsigned int ParticleEmitter::FirstUnusedParticle()
		{
			for(int i = LastUsedParticle; i < numberOfParticles; ++i){
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
			float x = (useRandomX) ? GetRandomFloat(minRandomX, maxRandomX) : initVelocityX;
			float y = (useRandomY) ? GetRandomFloat(minRandomY, maxRandomY) : initVelocityY;
			float z = (useRandomZ) ? GetRandomFloat(minRandomZ, maxRandomZ) : initVelocityZ;

			p.position = Maths::Vec3(0,0,0);
			// For some reason positive is the wrong way :(, probably a problem in the shader
			p.velocity = Maths::Vec3(-x, -y, -z);
			p.life = respawnLifetime;
		}

		float ParticleEmitter::GetRandomFloat(float min, float max)
		{
			// Swap values if there is a problem
			if (max < min)
			{
				float temp = max;
				max = min;
				min = temp;
			}

			std::random_device rd;
			std::mt19937 mt(rd());
			std::uniform_real_distribution<float> dist(min, std::nextafter(max, FLT_MAX));
			return dist(mt);
		}
	}
}
