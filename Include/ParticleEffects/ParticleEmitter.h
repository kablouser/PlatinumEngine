//
// Created by Matt on 26/04/2022.
//

#pragma once

#include <vector>
#include <random>
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
		public:
			// Emitter Settings
			int numberOfParticles = 500;
			float respawnLifetime = 4.0f; // Particles will have this initial lifetime
			int numberOfNewParticles = 2; // How many particles to spawn each frame
			float spawnInterval = 0.5;
			Maths::Vec3 actingForce = {0.0f, 1.0f, 0.0f};

			// Position Settings
			Maths::Vec3 initPosition = {0.0f, 0.0f, 0.0f};
			bool useRandomInitPositionX = false;
			bool useRandomInitPositionY = false;
			bool useRandomInitPositionZ = false;
			float minMaxPositionX[2] = {-1.0f, 1.0f};
			float minMaxPositionY[2] = {0.0f, 0.0f};
			float minMaxPositionZ[2] = {-1.0f, 1.0f};

			// Velocity Settings
			Maths::Vec3 initVelocity = {0.0f, 1.0f, 0.0f};
			bool useRandomInitVelocityX = true;
			bool useRandomInitVelocityY = false;
			bool useRandomInitVelocityZ = true;
			float minMaxVelocityX[2] = {-1.0f, 1.0f};
			float minMaxVelocityY[2] = {0.0f, 1.0f};
			float minMaxVelocityZ[2] = {-1.0f, 1.0f};

			// Shader settings
			Maths::Vec4 startColour = Maths::Vec4(1,1,1,1);
			Maths::Vec4 endColour = Maths::Vec4(0,0,0,0);
			Maths::Vec4 P2Colour = Maths::Vec4(1,1,1,1);
			float P2Time = 0.0f;
			Maths::Vec4 P3Colour = Maths::Vec4(0,0,0,0);
			float P3Time = 1.0f;

			Maths::Vec2 startRed = Maths::Vec2(0.0f, 1.0f);
			Maths::Vec2 P2Red = Maths::Vec2(0.0f, 1.0f);
			Maths::Vec2 P3Red = Maths::Vec2(1.0f, 0.0f);
			Maths::Vec2 endRed = Maths::Vec2(1.0f, 0.0f);

			// Bezier curve values
			// Start and end have fixed time, 0 and 1 respectively
			// RGBA times for P2 and P3
			float P2Times[4] = {0,0,0,0};
			float P3Times[4] = {1,1,1,1};

			std::unique_ptr<std::vector<Particle>> particles;
		private:
			unsigned int FirstDeadParticle();
			void RespawnParticle(Particle &p);
			float GetRandomFloat(float minMax[2]);
		private:
			unsigned int _lastDeadParticle = 0;
//			std::vector<Particle> _particleContainer{MaxParticles}; // Will be resized to MaxParticles in constructor
//			std::vector<Particle> _particles; // The alive particles to send to renderer
			float _timeSinceLastSpawn = 0.0f;
			std::random_device _rd;
			std::mt19937 _mt;
			std::unique_ptr<std::vector<Particle>> _particleContainer;
//			std::unique_ptr<std::vector<Particle>> _particles;
		};
	}
}