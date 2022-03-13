#include <Renderer/Renderer.h>
// checking errors
#include <OpenGL/GLCheck.h>
// gui
#include <imgui.h>
// OpenGL extensions
#include <GL/glew.h>
// printing errors
#include <Logger/Logger.h>

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
	Renderer::Renderer(bool printOpenGLInfo) :
			_isInitGood(false),
			_framebufferWidth(0),
			_framebufferHeight(0)
	{
// 		TODO seperated OpenGL context for cleaner global state
//		glfwDefaultWindowHints
//		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
//		GLFWwindow* offscreen_context = glfwCreateWindow(640, 480, "", NULL, NULL);

		if (printOpenGLInfo)
		{
			PLATINUM_INFO("OpenGL context info");
			PLATINUM_INFO("Vendor: "+std::string((char*)glGetString(GL_VENDOR)));
			PLATINUM_INFO("Renderer: "+std::string((char*)glGetString(GL_RENDERER)));
			PLATINUM_INFO("OpenGL version: "+std::string((char*)glGetString(GL_VERSION)));
			PLATINUM_INFO("GLSL version: "+std::string((char*)glGetString(GL_SHADING_LANGUAGE_VERSION)));
			// TODO
			// check if version meets minimum requirements here
			// need to parse string
		}

		// glew imports all OpenGL extension methods
		GLenum errorCode = glewInit();
		if (errorCode != GLEW_OK)
		{
			PLATINUM_ERROR(std::string("GLEW initialisation error. ") + std::string((char*)glewGetErrorString(errorCode)));
			// without GLEW, seg faults will happen
			return;
		}

		if (!_meshShader.Compile(UNLIT_VERTEX_SHADER, UNLIT_FRAGMENT_SHADER))
			return;


		_framebufferWidth = 1;
		_framebufferHeight = 1;

		// check for uncaught errors
		GL_CHECK();

		_isInitGood = true;
	}

	Renderer::~Renderer()
	{
		// nothing to do
	}

	void Renderer::Begin()
	{
		_framebuffer->Bind();
		GL_CHECK(glViewport(0, 0, _framebufferWidth, _framebufferHeight));
		GL_CHECK(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
	}

	void Renderer::End()
	{
		_framebuffer->Unbind();
	}

	void Renderer::SetFramebuffer(Framebuffer* framebuffer)
	{
		_framebuffer = framebuffer;
	}

	void Renderer::ResizeFrameBuffer(Framebuffer *framebuffer, ImVec2 targetSize)
	{
		_framebufferWidth = (int)targetSize.x;
		_framebufferHeight = (int)targetSize.y;
		_framebuffer->Create(_framebufferWidth, _framebufferHeight);
	}

	void Renderer::LoadMesh(const Mesh &mesh)
	{
		_meshShader.Bind();
		_unlitShaderInput.Set(mesh.GetVertices(), mesh.GetIndices());
		SetLightProperties();
	}

	// update model matrix in shader
	void Renderer::SetModelMatrix(Maths::Mat4 mat)
	{
		_meshShader.SetUniform("model", mat);
	}

	// update view matrix in shader
	void Renderer::SetViewMatrix(Maths::Mat4 mat)
	{
		_meshShader.SetUniform("view", mat);
	}

	// update perspective matrix in shader
	void Renderer::SetProjectionMatrix(Maths::Mat4 mat)
	{
		_meshShader.SetUniform("projection", mat);
	}

	void Renderer::Render()
	{
		_unlitShaderInput.Draw();
		_meshShader.Unbind();
	}
	//--------------------------------------------------------------------------------------------------------------
	// Private functions implementation.
	//--------------------------------------------------------------------------------------------------------------
	void Renderer::SetLightProperties()
	{
		// set basic properties
		_meshShader.SetUniform("objectColour", Maths::Vec3(1.0f,0.5f,0.31f));
		_meshShader.SetUniform("isTextureEnabled",false);

		_meshShader.SetUniform("lightPosition", Maths::Vec3(0.9f * std::cos(glfwGetTime()),0.9f * std::sin(glfwGetTime()),0.9f));
		_meshShader.SetUniform("lightColour", Maths::Vec3(1.0f,1.0f,1.0f));
		_meshShader.SetUniform("viewPosition", Maths::Vec3(0.0f,0.0f,1.0f));
	}


}
//}