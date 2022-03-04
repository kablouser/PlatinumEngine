#include <RasterRenderer/RasterRenderer.h>
#include <OpenGL/GLCheck.h>
// #include <imgui.h>
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

		// auto windowSize = parentWindow.getSize();
		// _renderTexture.create(windowSize.x,windowSize.y,_contextSettings);

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

		_isInitGood = true;
	}

	RasterRenderer::~RasterRenderer()
	{
		// nothing to do
	}


	//--------------------------------------------------------------------------------------------------------------
	// Public functions implementation.
	//--------------------------------------------------------------------------------------------------------------

	void RasterRenderer::RenderObjects(sf::RenderTexture& renderTexture, ImVec2 targetSize)
	{
		// when init is bad, don't render anything
		if(_isInitGood)
		{
			CubeTest();
			assert(renderTexture.setActive(true));

			// check window size
			//auto targetSize = ImGui::GetContentRegionAvail();
			auto textureSize = renderTexture.getSize();

			if(targetSize.x != textureSize.x || targetSize.y != textureSize.y)
			{

				renderTexture.create(targetSize.x, targetSize.y, _contextSettings);
				GL_CHECK(glViewport(0, 0, targetSize.x, targetSize.y));


				// prevent the window size to be negative
				targetSize.x = std::max(0.f, targetSize.x);
				targetSize.y = std::max(0.f, targetSize.y);

				// prevent creating rendertexture with x or y == 0,
				// which will cause warning
				if(targetSize.y != 0 && targetSize.x != 0){

					renderTexture.create(targetSize.x, targetSize.y, _contextSettings);
					GL_CHECK(glViewport(0, 0, targetSize.x, targetSize.y));

				}
			}

			// first, clear OpenGL target. same as glClear
			renderTexture.clear(sf::Color(18, 33, 43));

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
			assert(renderTexture.setActive(false));

			// apply drawings onto the target
			renderTexture.display();
			// put target into GUI window
			//ImGui::Image(_renderTexture, targetSize);
		}
		//ImGui::End();
	}


	//--------------------------------------------------------------------------------------------------------------
	// Private functions implementation.
	//--------------------------------------------------------------------------------------------------------------
	void RasterRenderer::CubeTest()
	{
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

		// set basic properties
		_unlitShader.setUniform("objectColour",sf::Glsl::Vec3(1.0f,0.5f,0.31f));
		_unlitShader.setUniform("isTextureEnabled",false);
		time = clock.getElapsedTime();

		_unlitShader.setUniform("lightPosition", sf::Glsl::Vec3(0.9f * std::sin(time.asSeconds()),0.9f * std::sin(time.asSeconds()),0.9f));
		_unlitShader.setUniform("lightColour", sf::Glsl::Vec3(1.0f,1.0f,1.0f));
		_unlitShader.setUniform("viewPosition", sf::Glsl::Vec3(0.0f,0.0f,1.0f));

		// in variables
		_unlitShaderInput.Set({
						{{ -0.5f, -0.5f, -0.5f  }, {  0.0f,  0.0f, -1.0f }, { 0, 0 }},
						{{ 0.5f, -0.5f, -0.5f   }, { 0.0f,  0.0f, -1.0f  }, { 0, 0 }},
						{{ 0.5f,  0.5f, -0.5f   }, { 0.0f,  0.0f, -1.0f  }, { 0, 0 }},
						{{ 0.5f,  0.5f, -0.5f   }, { 0.0f,  0.0f, -1.0f  }, { 0, 0 }},
						{{ -0.5f,  0.5f, -0.5f  }, {  0.0f,  0.0f, -1.0f }, { 0, 0 }},
						{{ -0.5f, -0.5f, -0.5f  }, {  0.0f,  0.0f, -1.0f }, { 0, 0 }},
						{{ -0.5f, -0.5f,  0.5f  }, {  0.0f,  0.0f,  1.0f }, { 0, 0 }},
						{{ 0.5f, -0.5f,  0.5f   }, { 0.0f,  0.0f,  1.0f  }, { 0, 0 }},
						{{ 0.5f,  0.5f,  0.5f   }, { 0.0f,  0.0f,  1.0f  }, { 0, 0 }},
						{{ 0.5f,  0.5f,  0.5f   }, { 0.0f,  0.0f,  1.0f  }, { 0, 0 }},
						{{ -0.5f,  0.5f,  0.5f  }, {  0.0f,  0.0f,  1.0f }, { 0, 0 }},
						{{ -0.5f, -0.5f,  0.5f  }, {  0.0f,  0.0f,  1.0f }, { 0, 0 }},
						{{ -0.5f,  0.5f,  0.5f  }, { -1.0f,  0.0f,  0.0f }, { 0, 0 }},
						{{ -0.5f,  0.5f, -0.5f  }, { -1.0f,  0.0f,  0.0f }, { 0, 0 }},
						{{ -0.5f, -0.5f, -0.5f  }, { -1.0f,  0.0f,  0.0f }, { 0, 0 }},
						{{ -0.5f, -0.5f, -0.5f  }, { -1.0f,  0.0f,  0.0f }, { 0, 0 }},
						{{ -0.5f, -0.5f,  0.5f  }, { -1.0f,  0.0f,  0.0f }, { 0, 0 }},
						{{ -0.5f,  0.5f,  0.5f  }, { -1.0f,  0.0f,  0.0f }, { 0, 0 }},
						{{ 0.5f,  0.5f,  0.5f   }, { 1.0f,  0.0f,  0.0f  }, { 0, 0 }},
						{{ 0.5f,  0.5f, -0.5f   }, { 1.0f,  0.0f,  0.0f  }, { 0, 0 }},
						{{ 0.5f, -0.5f, -0.5f   }, { 1.0f,  0.0f,  0.0f  }, { 0, 0 }},
						{{ 0.5f, -0.5f, -0.5f   }, { 1.0f,  0.0f,  0.0f  }, { 0, 0 }},
						{{ 0.5f, -0.5f,  0.5f   }, { 1.0f,  0.0f,  0.0f  }, { 0, 0 }},
						{{ 0.5f,  0.5f,  0.5f   }, { 1.0f,  0.0f,  0.0f  }, { 0, 0 }},
						{{ -0.5f, -0.5f, -0.5f  }, {  0.0f, -1.0f,  0.0f }, { 0, 0 }},
						{{ 0.5f, -0.5f, -0.5f   }, { 0.0f, -1.0f,  0.0f  }, { 0, 0 }},
						{{ 0.5f, -0.5f,  0.5f   }, { 0.0f, -1.0f,  0.0f  }, { 0, 0 }},
						{{ 0.5f, -0.5f,  0.5f   }, { 0.0f, -1.0f,  0.0f  }, { 0, 0 }},
						{{ -0.5f, -0.5f,  0.5f  }, {  0.0f, -1.0f,  0.0f }, { 0, 0 }},
						{{ -0.5f, -0.5f, -0.5f  }, {  0.0f, -1.0f,  0.0f }, { 0, 0 }},
						{{ -0.5f,  0.5f, -0.5f  }, {  0.0f,  1.0f,  0.0f }, { 0, 0 }},
						{{ 0.5f,  0.5f, -0.5f   }, { 0.0f,  1.0f,  0.0f  }, { 0, 0 }},
						{{ 0.5f,  0.5f,  0.5f   }, { 0.0f,  1.0f,  0.0f  }, { 0, 0 }},
						{{ 0.5f,  0.5f,  0.5f   }, { 0.0f,  1.0f,  0.0f  }, { 0, 0 }},
						{{ -0.5f,  0.5f,  0.5f  }, {  0.0f,  1.0f,  0.0f }, { 0, 0 }},
						{{ -0.5f,  0.5f, -0.5f  }, {  0.0f,  1.0f,  0.0  }, { 0, 0 }},
				},
				{
						0,   1,   2,
						3,   4,   5,
						6,   7,   8,
						9,   10,  11,
						12,  13,  14,
						15,  16,  17,
						18,  19,  20,
						21,  22,  23,
						24,  25,  26,
						27,  28,  29,
						30,  31,  32,
						33,  34,  35,
						36,  37,  38,
						39,  40,  41,
						42,  43,  44,
						45,  46,  47,
						48,  49,  50,
						51,  52,  53,
						54,  55,  56,
						57,  59,  59,
						60,  61,  62,
						63,  64,  65,
						66,  67,  68,
						69,  70,  71,
						72,  73,  74,
						75,  76,  77,
						78,  79,  80,
						81,  82,  83,
						84,  85,  86,
						87,  88,  89,
						90,  91,  92,
						93,  94,  95,
						96,  97,  98,
						99,  100, 101,
						102, 103, 104,
						105, 106, 107
				});
	}




}