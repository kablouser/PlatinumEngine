//
// Created by Matt on 26/04/2022.
//

#pragma once

#include <vector>
#include <OpenGL/OpenGL.h>
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
			void UpdateParticles(float deltaTime);
			void SetNumberOfParticles(int numberOfParticles);
			[[nodiscard]] int GetNumberOfParticles();
			[[nodiscard]] const std::vector<Particle> GetParticles() const;
			void SetRespawnLifetime(int lifetime);
			float GetRespawnLifetime();
			void SetNumberNewParticles(int numNewParticles);
			int GetNumberNewParticles();
		private:
			unsigned int FirstUnusedParticle();
			void RespawnParticle(Particle &p);
		private:
			int _numberOfParticles = 10;
			unsigned int LastUsedParticle = 0;
			std::vector<Particle> _particleContainer; // Will be resized to MaxParticles in constructor
			std::vector<Particle> _particles; // The alive particles to send to renderer
			float respawnLifetime = 1.0f; // Particles will have this initial lifetime
			int numberOfNewParticles = 2; // How many particles to spawn each frame
		};
	}
}