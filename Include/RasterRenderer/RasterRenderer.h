#pragma once

// PlatinumEngine lib
#include <OpenGL/ShaderInput.h>
#include <OpenGL/ShaderProgram.h>
#include <OpenGL/Framebuffer.h>
//#include <RasterRenderer/EditorCamera.h>

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
	};
}
