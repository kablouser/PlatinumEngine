//
// Created by Matt on 26/04/2022.
//

#include <ParticleEffects/ParticleRenderer.h>

namespace PlatinumEngine
{

	ParticleEffects::ParticleRenderer::ParticleRenderer()
	{
		// set up mesh and attribute properties
		float particle_quad[] = {
				0.0f, 1.0f, 0.0f, 1.0f,
				1.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,

				0.0f, 1.0f, 0.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 0.0f, 1.0f, 0.0f
		};
		_shaderInput.Set(vertices, indices);
	}

	void ParticleEffects::ParticleRenderer::Render(Renderer& renderer)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		renderer.BeginParticleShader();

//		for (Particle particle : this->particles)
//		{
//			if (particle.Life > 0.0f)
//			{
//				this->shader.SetVector2f("offset", particle.Position);
//				this->shader.SetVector4f("color", particle.Color);
//				this->texture.Bind();
				_shaderInput.Draw();
//			}
//		}

		renderer.EndParticleShader();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}