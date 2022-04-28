//
// Created by Matt on 26/04/2022.
//

#include <ParticleEffects/ParticleRenderer.h>
#include <OpenGL/GLCheck.h>
#include <GL/glew.h>


namespace PlatinumEngine
{
	namespace ParticleEffects
	{
		ParticleRenderer::ParticleRenderer()
		{
		}

		ParticleRenderer::~ParticleRenderer()
		{
			Clear();
		}

		void ParticleRenderer::Render()
		{
			// Particles will want to use a nice blending function as a lof of them will be crammed into one space
			// Creates a glow effect
			GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE));

			// Make sure depth test is on and reset if needed later
			GLboolean isUsingDepthTest;
			GL_CHECK(glGetBooleanv(GL_DEPTH_TEST, &isUsingDepthTest));
			glDisable(GL_DEPTH_TEST);

			// The good stuff, instanced rendering
			GL_CHECK(glBindVertexArray(_particleVertexVAO));
			GL_CHECK(glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, _numParticles));
			GL_CHECK(glBindVertexArray(0));

			// Reset for other rendering
			if (isUsingDepthTest)
				glEnable(GL_DEPTH_TEST);
			GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		}

		void ParticleRenderer::SetInput(const std::unique_ptr<std::vector<Particle>>& particles)
		{
			// We know we want to send a set number of floats to the shader so allocate space for that
			GLfloat newData[particles->size() * 4];
			GLfloat newData2[particles->size() * 4];

			// Now, loop each alive particle and send the data across
			for (unsigned int i = 0; i < particles->size(); ++i)
			{
				newData[i * 4 + 0] = particles->at(i).position.x;
				newData[i * 4 + 1] = particles->at(i).position.y;
				newData[i * 4 + 2] = particles->at(i).position.z;
				newData[i * 4 + 3] = particles->at(i).life;
				newData2[i * 4 + 0] = particles->at(i).velocity.x;
				newData2[i * 4 + 1] = particles->at(i).velocity.y;
				newData2[i * 4 + 2] = particles->at(i).velocity.z;
				newData2[i * 4 + 3] = particles->at(i).scale;
			}

			// Now, set the sub-data of the big buffer we created earlier, that way we only render particles in use
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _positionLifeVBO));
			GL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * _numFloats * particles->size(), &newData));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _positionLifeVBO));
			GL_CHECK(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _velocityScaleVBO));
			GL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * _numFloats * particles->size(), &newData));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _velocityScaleVBO));
			GL_CHECK(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

			// Need to store this to know how many particles to draw in instance call
			_numParticles = (int) particles->size();
		}

		void ParticleRenderer::Clear()
		{
			// Should delete our buffers etc. now
		}

		void ParticleRenderer::Init()
		{
			// This stuff is for the vertices of a particle, i.e. the quad
			if (_particleVertexVAO == 0)
				GL_CHECK(glGenVertexArrays(1, &_particleVertexVAO));
			GL_CHECK(glBindVertexArray(_particleVertexVAO));

			if (_particleVertexVBO == 0)
				GL_CHECK(glGenBuffers(1, &_particleVertexVBO));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _particleVertexVBO));
			GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW));
			GL_CHECK(glEnableVertexAttribArray(0));

			// Vertex coords
			GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
			GL_CHECK(glEnableVertexAttribArray(1));
			GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),  (void*)(3 * sizeof(float))));

			// Now it gets funky
			// Because we're instancing set VBO sizes to max size possible and init as NULL
			// Then, when we render we set sub-data of the buffer which is actually renderer
			if (_positionLifeVBO == 0)
				GL_CHECK(glGenBuffers(1, &_positionLifeVBO));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _positionLifeVBO));
			GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _numFloats * MaxParticles, NULL, GL_STATIC_DRAW));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
			GL_CHECK(glBindBufferRange(GL_UNIFORM_BUFFER, 0, _positionLifeVBO, 0, sizeof(GLfloat) * _numFloats * MaxParticles));

			// Need to tell opengl what values to send to shader and in what format
			GL_CHECK(glEnableVertexAttribArray(2));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _positionLifeVBO));
			GL_CHECK(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

			if (_velocityScaleVBO == 0)
				GL_CHECK(glGenBuffers(1, &_velocityScaleVBO));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _velocityScaleVBO));
			GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _numFloats * MaxParticles, NULL, GL_STATIC_DRAW));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
			GL_CHECK(glBindBufferRange(GL_UNIFORM_BUFFER, 0, _velocityScaleVBO, 0, sizeof(GLfloat) * _numFloats * MaxParticles));

			// Need to tell opengl what values to send to shader and in what format
			GL_CHECK(glEnableVertexAttribArray(3));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _velocityScaleVBO));
			GL_CHECK(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

			GL_CHECK(glVertexAttribDivisor(2, 1)); // tell OpenGL this is an instanced vertex attribute.
			GL_CHECK(glVertexAttribDivisor(3, 1)); // tell OpenGL this is an instanced vertex attribute.
		}
	}
}