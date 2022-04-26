//
// Created by Matt on 26/04/2022.
//

#include <ParticleEffects/ParticleRenderer.h>

namespace PlatinumEngine
{
	namespace ParticleEffects
	{
		ParticleRenderer::ParticleRenderer()
		{
			static const GLfloat vertices[] = {
					-0.5f, -0.5f, 0.0f,
					0.5f, -0.5f, 0.0f,
					-0.5f, 0.5f, 0.0f,
					0.5f, 0.5f, 0.0f,
			};

			// Quad data for sprite rendering
			GL_CHECK(glGenVertexArrays(1, &quadVAO));
			GL_CHECK(glGenBuffers(1, &quadVBO));
			GL_CHECK(glBindVertexArray(quadVAO));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, quadVBO));
			GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
			GL_CHECK(glEnableVertexAttribArray(0));
			GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));

			// also set instance data
			glGenBuffers(1, &instanceVBO);
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, instanceVBO));
			GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * MaxParticles, NULL, GL_STATIC_DRAW));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
			glBindBufferRange(GL_UNIFORM_BUFFER, 0, instanceVBO, 0, sizeof(GLfloat) * 3 * MaxParticles);

			// Instance data
			GL_CHECK(glEnableVertexAttribArray(1));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, instanceVBO));
			// this attribute comes from a different vertex buffer
			GL_CHECK(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
			GL_CHECK(glVertexAttribDivisor(1, 1)); // tell OpenGL this is an instanced vertex attribute.
		}

		ParticleRenderer::~ParticleRenderer()
		{
			Clear();
		}

		void ParticleRenderer::Render(Renderer& renderer)
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			renderer.BeginParticleShader();
			GL_CHECK(glBindVertexArray(quadVAO));
			GL_CHECK(glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 6, NumParticles)); // 100 triangles of 6 vertices each
			GL_CHECK(glBindVertexArray(0));
			renderer.EndParticleShader();
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		void ParticleRenderer::SetInput(const std::vector<Particle>& particles)
		{
			GLfloat newData[particles.size() * 3];
			for (unsigned int i = 0; i < particles.size(); ++i)
			{
				newData[i * 3 + 0] = particles[i].position.x;
				newData[i * 3 + 1] = particles[i].position.y;
				newData[i * 3 + 2] = particles[i].position.z;
			}

			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, instanceVBO));
			GL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 3 * particles.size(), &newData));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, instanceVBO));
			GL_CHECK(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
			NumParticles = (int) particles.size();
		}

		void ParticleRenderer::Clear()
		{
		}
	}
}