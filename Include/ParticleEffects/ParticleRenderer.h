//
// Created by Matt on 26/04/2022.
//

#pragma once

#include <Renderer/Renderer.h>
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
			void Render(Renderer& renderer);
			void SetInput(const std::vector<Particle> &particles);
			void Clear();
		private:
			int _numParticles;
			unsigned int _positionLifeVBO, _particleVertexVAO, _particleVertexVBO;
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