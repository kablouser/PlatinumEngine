#include <RasterRenderer/RasterRenderer.h>
#include <OpenGL/GLCheck.h>
#include <imgui-SFML.h>
#include <imgui.h>
// glew.h replaces gl.h. So don't #include <SFML/OpenGL.hpp>
#include <GL/glew.h>
#include <SFML/Graphics/Glsl.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

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
			unsigned int depthBits,
			unsigned int stencilBits,
			unsigned int antialiasingLevel,
			unsigned int width,
			unsigned int height
	) :
			_renderWindow(
					sf::VideoMode(width, height),
					"OpenGL",
					sf::Style::Default,
					sf::ContextSettings(depthBits, stencilBits, antialiasingLevel)),
			_isInitGood(false)
	{
		_renderWindow.setVerticalSyncEnabled(true);

		// debug OpenGL settings
		sf::ContextSettings settings = _renderWindow.getSettings();
		std::cout << "depth bits:" << settings.depthBits << std::endl;
		std::cout << "stencil bits:" << settings.stencilBits << std::endl;
		std::cout << "antialiasing level:" << settings.antialiasingLevel << std::endl;
		std::cout << "version:" << settings.majorVersion << "." << settings.minorVersion << std::endl;

		// Enable ImGui in SFML
		ImGui::SFML::Init(_renderWindow);

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

		GL_CHECK(glViewport(0, 0, _renderWindow.getSize().x, _renderWindow.getSize().y));

		_isInitGood = true;
	}

	RasterRenderer::~RasterRenderer()
	{
		ImGui::SFML::Shutdown(_renderWindow);
	}

	bool RasterRenderer::Update(const sf::Clock& deltaClock)
	{
		// stop rendering if window is closed OR init was bad
		if (!_renderWindow.isOpen() || !_isInitGood)
			return false;

		// handle events
		sf::Event event;
		while (_renderWindow.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed)
			{
				// if window is closed, stop the loop
				_renderWindow.close();
				return false;
			}
			else if(event.type == sf::Event::Resized)
			{
				// resize should change OpenGL viewport
				GL_CHECK(glViewport(0, 0, event.size.width, event.size.height));
			}
		}

		// first, clear window. equal to glClear
		_renderWindow.clear(sf::Color(18, 33, 43));

		// set _renderWindow to the current target for any shaders
		assert(_renderWindow.setActive(true));
		{
			// bind any shader
			sf::Shader::bind(&_unlitShader);

			// opengl draw calls
			_unlitShaderInput.Draw();

//	 		this still works, but don't use it. because the input layout could be wrong.
//			glBegin(GL_TRIANGLES);
//			glVertex3f(-1.0f, -0.5f, 0.0f);
//			glVertex3f(-0.3f, -0.5f, 0.0f);
//			glVertex3f(-0.65f, +0.5f, 0.0f);
//			glEnd();
		}
		// undo global state changes
		sf::Shader::bind(nullptr);
		assert(_renderWindow.setActive(false));

		// update imgui last, so imgui draws on top of OpenGL
		ImGui::SFML::Update(_renderWindow, deltaClock.getElapsedTime());

		ImGui::SetNextWindowBgAlpha(0.2f);
		ImGui::Begin("Window title");
		ImGui::End();
		ImGui::SetNextWindowBgAlpha(0.2f);
		ImGui::Begin("Window title2");
		ImGui::End();

		ImGui::SFML::Render(_renderWindow);

		// display window
		_renderWindow.display();

		// regularly check for any uncaught OpenGL errors
		GL_CHECK();

		return true;
	}
}