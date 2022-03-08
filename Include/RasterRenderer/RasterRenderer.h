#pragma once

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// PlatinumEngine lib
#include <OpenGL/ShaderInput.h>
#include <OpenGL/ShaderProgram.h>
#include <OpenGL/Framebuffer.h>
#include <GLFW/glfw3.h>

namespace PlatinumEngine
{
	class RasterRenderer
	{

	public:

		// VARIABLE
		//PlatinumEngine::EditorCamera camera;

		// Constructors
		RasterRenderer(bool printOpenGLInfo = true);

		~RasterRenderer();

		void ShowGUIWindow(bool* outIsOpen);

		void LoadMesh();

	private:

		// true iff all init steps were successful
		bool _isInitGood;

//		settings for rendering OpenGL
//		sf::ContextSettings _contextSettings;
//		// intermediate output of OpenGL rendering
//		sf::RenderTexture _renderTexture;

		ShaderProgram _shaderProgram;
		ShaderInput _unlitShaderInput;

		Framebuffer _framebuffer;
		int _framebufferWidth;
		int _framebufferHeight;

		// these two parameters are creating for testing the effect of Blinn-Phong-Shading



		void CubeTest();
	};
}
