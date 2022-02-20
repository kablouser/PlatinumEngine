#include <RasterRenderer/RasterRenderer.h>
#include <OpenGL/GLCheck.h>
#include <imgui.h>
#include <imgui-SFML.h>
// glew.h replaces gl.h. So don't #include <SFML/OpenGL.hpp>
#include <GL/glew.h>
#include <SFML/Graphics/Glsl.hpp>

// std
#include <iostream>

// shaders
// DON'T FORMAT THESE LINES, OTHERWISE IT BREAKS
const std::string UNLIT_VERTEX_SHADER =
#include <Shaders/Unlit/Unlit.vert>
;
const std::string UNLIT_FRAGMENT_SHADER =
#include <Shaders/Unlit/Unlit.frag>
;

namespace PlatinumEngine
{
	RasterRenderer::RasterRenderer(
			const sf::Window& parentWindow,
			unsigned int depthBits,
			unsigned int stencilBits,
			unsigned int antiAliasingLevel
	) :
			_isInitGood(false),
			_contextSettings(depthBits, stencilBits, antiAliasingLevel)
	{
		sf::ContextSettings contextSettings = parentWindow.getSettings();
		std::cout << "OpenGL version:" << contextSettings.majorVersion << "." << contextSettings.minorVersion << std::endl;
		// TODO
		// check if version meets minimum requirements here

		auto windowSize = parentWindow.getSize();
		_renderTexture.create(windowSize.x,windowSize.y,_contextSettings);

		// glew imports all OpenGL extension methods
		GLenum errorCode = glewInit();
		if (errorCode != GLEW_OK)
		{
			std::cerr << "GLEW initialisation error. " << glewGetErrorString(errorCode) << std::endl;
			// without GLEW, seg faults will happen
			return;
		}

		GLuint test = 0;
		GL_CHECK(glDeleteBuffers(1, &test));

		if (!_unlitShader.isAvailable())
		{
			std::cerr << "OpenGL shaders are not supported on this device!" << std::endl;
			return;
		}

		if (!_unlitShader.loadFromMemory(UNLIT_VERTEX_SHADER, UNLIT_FRAGMENT_SHADER))
		{
			std::cerr << "Shaders compilation failed!" << std::endl;
			return;
		}

		/*
		Shader uniform variables:

		uniform mat4 modelToProjection; // projection * view * model
		uniform vec3 color = vec3(1.0, 1.0, 1.0);
		uniform bool isTextureEnabled = false;
		uniform sampler2D sampleTexture;
		 */
		float matrixArray[16] = {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
		};
		sf::Glsl::Mat4 matrix(matrixArray);

		_unlitShader.setUniform("modelToProjection",matrix);
		_unlitShader.setUniform("color",sf::Glsl::Vec3(1,0,0));
		_unlitShader.setUniform("isTextureEnabled",false);

		// in variables
		_unlitShaderInput.Set({
						{{ -0.5f, -0.5f, 0.0f }, { 0, 0, 1 }, { 0, 0 }},
						{{ 0.5f,  -0.5f, 0.0f }, { 0, 0, 1 }, { 0, 0 }},
						{{ 0.0f,  0.5f,  0.0f }, { 0, 0, 1 }, { 0, 0 }}
				},
				{
						0, 1, 2
				});

		_isInitGood = true;
	}

	RasterRenderer::~RasterRenderer()
	{
		// nothing to do
	}

	void RasterRenderer::ShowGUIWindow(bool* outIsOpen)
	{
		if(ImGui::Begin("Raster Renderer", outIsOpen) &&
			// when init is bad, don't render anything
			_isInitGood)
		{
			assert(_renderTexture.setActive(true));

			// check window size
			auto targetSize = ImGui::GetContentRegionAvail();
			auto textureSize = _renderTexture.getSize();
			if(targetSize.x != textureSize.x || targetSize.y != textureSize.y)
			{
				_renderTexture.create(targetSize.x, targetSize.y, _contextSettings);
				GL_CHECK(glViewport(0, 0, targetSize.x, targetSize.y));
			}

			// first, clear OpenGL target. same as glClear
			_renderTexture.clear(sf::Color(18, 33, 43));

			{
				// bind any shader
				sf::Shader::bind(&_unlitShader);

				// opengl draw calls
				_unlitShaderInput.Draw();

				// this still works, but don't use it. because the input layout could be wrong.
				// glBegin(GL_TRIANGLES);
				// glVertex3f(-1.0f, -0.5f, 0.0f);
				// glVertex3f(-0.3f, -0.5f, 0.0f);
				// glVertex3f(-0.65f, +0.5f, 0.0f);
				// glEnd();
			}

			// regularly check for any uncaught OpenGL errors
			GL_CHECK();

			// undo global state changes
			sf::Shader::bind(nullptr);
			assert(_renderTexture.setActive(false));

			// apply drawings onto the target
			_renderTexture.display();
			// put target into GUI window
			ImGui::Image(_renderTexture, targetSize);
		}
		ImGui::End();
	}
}