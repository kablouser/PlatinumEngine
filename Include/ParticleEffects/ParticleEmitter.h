//
// Created by Matt on 26/04/2022.
//

#pragma once

#include <vector>
#include <random>
#include <ParticleEffects/Particle.h>
#include <OpenGL/Texture.h>
#include <memory>
#include <IDSystem/IDSystem.h>
#include <TypeDatabase/TypeDatabase.h>

namespace PlatinumEngine
{
	namespace ParticleEffects
	{
		static const int MaxParticles = 5000;
		class ParticleEmitter
		{
		public:
			static void CreateTypeInfo(TypeDatabase& typeDatabase);

			ParticleEmitter();
			void UpdateParticles(float deltaTime, const Maths::Vec3 &cameraPos);
		public:
			// Emitter Settings
			int numberOfParticles = 500;
			float respawnLifetime = 4.0f; // Particles will have this initial lifetime
			int numberOfNewParticles = 4; // How many particles to spawn each frame
			float spawnInterval = 0.5;
			bool useCylindricalBillboard = false; // Switch between spherical and cylindrical bill-boarding
			Maths::Vec3 actingForce = {0.0f, 1.0f, 0.0f};
			Maths::Vec3 scaleFactors = {1.0f, 1.0f, 1.0f};

			// Position Settings
			bool useRandomInitPositionX = false;
			bool useUniformRandomPositionX = true;
			bool useRandomInitPositionY = false;
			bool useUniformRandomPositionY = true;
			bool useRandomInitPositionZ = false;
			bool useUniformRandomPositionZ = true;
			float minPositionX = -1.0f;
			float maxPositionX = 1.0f;
			float minPositionY = -1.0f;
			float maxPositionY = 1.0f;
			float minPositionZ = -1.0f;
			float maxPositionZ = 1.0f;

			// Velocity Settings
			Maths::Vec3 initVelocity = {0.0f, 1.0f, 0.0f};
			bool useRandomInitVelocityX = true;
			bool useUniformInitVelocityX = true;
			bool useRandomInitVelocityY = false;
			bool useUniformInitVelocityY = true;
			bool useRandomInitVelocityZ = true;
			bool useUniformInitVelocityZ = true;
			float minVelocityX = -1.0f;
			float maxVelocityX = 1.0f;
			float minVelocityY = -1.0f;
			float maxVelocityY = 1.0f;
			float minVelocityZ = -1.0f;
			float maxVelocityZ = 1.0f;

			// Info needed to calc texture coords appropriately
			SavedReference<Texture> texture;
			int numRowsInTexture = 1;
			int numColsInTexture = 1;

			// Other things
			bool isPlaying = true;
			std::string scaleBy = "Constant";
			float scaleFactor = 1.0f;
			std::unique_ptr<std::vector<Particle>> particles;
		private:
			unsigned int FirstDeadParticle();
			void RespawnParticle(Particle &p);
		private:
			unsigned int _lastDeadParticle = 0;
			float _timeSinceLastSpawn = 0.0f;
			std::unique_ptr<std::vector<Particle>> _particleContainer;
		};
	}
}