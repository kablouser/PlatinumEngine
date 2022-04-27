//
// Created by Matt on 26/04/2022.
//

#pragma once

#include <ParticleEffects/Particle.h>
#include <ParticleEffects/ParticleEmitter.h>
#include <Logger/Logger.h>

namespace PlatinumEngine
{
	namespace ParticleEffects
	{
		class ParticleRenderer
		{
		public:
			ParticleRenderer();
			~ParticleRenderer();
			void Init();
			void Render();
			void SetInput(const std::unique_ptr<std::vector<Particle>> &particles);
			void Clear();
		private:
			int _numParticles;
			unsigned int _positionLifeVBO = 0, _particleVertexVAO = 0, _particleVertexVBO = 0;
			int _numFloats = 4;

			// These are the vertices of the billboard for a particle
			const float _vertices[12] = {
					-0.5f, -0.5f, 0.0f,
					0.5f, -0.5f, 0.0f,
					-0.5f, 0.5f, 0.0f,
					0.5f, 0.5f, 0.0f,
			};
		};
	}
}