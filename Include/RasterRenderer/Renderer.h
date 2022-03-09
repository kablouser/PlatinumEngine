#pragma once

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// PlatinumEngine lib
#include <OpenGL/ShaderInput.h>
#include <OpenGL/ShaderProgram.h>
#include <OpenGL/Framebuffer.h>
#include <OpenGL/Mesh.h>
#include <GLFW/glfw3.h>

namespace PlatinumEngine
{
	class Renderer
	{

	public:

		// VARIABLE
		//PlatinumEngine::EditorCamera camera;

		// Constructors
		Renderer(bool printOpenGLInfo = true);

		~Renderer();

		void Render(bool* outIsOpen);

		void LoadMesh(const Mesh &mesh);

	private:

		// true iff all init steps were successful
		bool _isInitGood;

		ShaderProgram _shaderProgram;
		ShaderInput _unlitShaderInput;

		Framebuffer _framebuffer;
		int _framebufferWidth;
		int _framebufferHeight;

		void SetShaderProperties();

		void CubeTest();
	};
}
