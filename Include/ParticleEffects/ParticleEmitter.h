//
// Created by Matt on 26/04/2022.
//

#pragma once

#include <vector>
#include <random>
#include <OpenGL/OpenGL.h>
#include <ParticleEffects/Particle.h>

namespace PlatinumEngine
{
	namespace ParticleEffects
	{
		static const int MaxParticles = 5000;
		class ParticleEmitter
		{
		public:
			ParticleEmitter();
			void UpdateParticles(float deltaTime, const Maths::Vec3 &cameraPos);
			[[nodiscard]] const std::vector<Particle> GetParticles() const;
		public:
			// Emitter Settings
			int numberOfParticles = 100;
			float respawnLifetime = 1.0f; // Particles will have this initial lifetime
			int numberOfNewParticles = 2; // How many particles to spawn each frame

			// Velocity Settings
			float initVelocityX = 0.0f;
			float initVelocityY = 2.0f;
			float initVelocityZ = 0.0f;
			bool useRandomX = false;
			bool useRandomY = false;
			bool useRandomZ = false;
			float minRandomX = 0.0f;
			float maxRandomX = 1.0f;
			float minRandomY = 0.0f;
			float maxRandomY = 1.0f;
			float minRandomZ = 0.0f;
			float maxRandomZ = 1.0f;

			// Colour settings
			Maths::Vec4 startColour = Maths::Vec4(1,0,0,1);
			Maths::Vec4 endColour = Maths::Vec4(0,0,1,0);
		private:
			unsigned int FirstUnusedParticle();
			void RespawnParticle(Particle &p);
			float GetRandomFloat(float min, float max);
		private:
			unsigned int LastUsedParticle = 0;
			std::vector<Particle> _particleContainer; // Will be resized to MaxParticles in constructor
			std::vector<Particle> _particles; // The alive particles to send to renderer
		};
	}
}