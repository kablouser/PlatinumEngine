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
			int NumParticles;
			GLfloat data[MaxParticles * 3];
			unsigned int instanceVBO, quadVAO, quadVBO;
			int _numFloats = 4;
		};
	}
}