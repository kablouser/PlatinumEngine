//
// Created by Matt on 26/04/2022.
//

#pragma once

#include <OpenGL/ShaderInput.h>
#include <Renderer/Renderer.h>

namespace PlatinumEngine
{
	namespace ParticleEffects
	{
		class ParticleRenderer
		{
		public:
			ParticleRenderer();
			void Render(Renderer& renderer);
		private:
			ShaderInput _shaderInput;
			std::vector<Vertex> vertices = {{{1.0f, -1.0f, 0.0f}},
											{{-1.0f, -1.0f, 0.0f}},
											{{1.0f, 1.0f, 0.0f}},
											{{-1.0f, 1.0f, 0.0f}}};
			std::vector<unsigned int> indices = {1, 2, 0, 1, 3, 2};
		};
	}
}