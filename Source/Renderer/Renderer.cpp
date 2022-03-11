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

		if (!_shaderProgram.Compile(UNLIT_VERTEX_SHADER, UNLIT_FRAGMENT_SHADER))
			return;


		_framebufferWidth = 1;
		_framebufferHeight = 1;
		if (!_framebuffer.Create(_framebufferWidth, _framebufferHeight))
			return;

		// check for uncaught errors
		GL_CHECK();

		_isInitGood = true;
	}

	Renderer::~Renderer()
	{
		// nothing to do
	}

	void Renderer::Render(bool* outIsOpen)
	{
		if(ImGui::Begin("Raster Renderer", outIsOpen) &&
			// when init is bad, don't render anything
			_isInitGood)
		{

			// check ImGui's window size, can't render when area=0
			ImVec2 targetSize = ImGui::GetContentRegionAvail();
			if(1.0f < targetSize.x && 1.0f < targetSize.y)
			{
				// resize framebuffer if necessary
				if (_framebufferWidth != (int)targetSize.x || _framebufferHeight != (int)targetSize.y)
				{
					_framebufferWidth = (int)targetSize.x;
					_framebufferHeight = (int)targetSize.y;
					_framebuffer.Create(_framebufferWidth, _framebufferHeight);
				}

				_framebuffer.Bind();

				GL_CHECK(glViewport(0, 0, targetSize.x, targetSize.y));
				GL_CHECK(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
				GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

				_shaderProgram.Bind();
				_unlitShaderInput.Draw();
				_shaderProgram.Unbind();

				_framebuffer.Unbind();

				ImGui::Image(_framebuffer.GetColorTexture().GetImGuiHandle(), targetSize);

				GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
			}
		}

		ImGui::End();
	}

	void Renderer::LoadMesh(const Mesh &mesh)
	{
		_unlitShaderInput.Set(mesh.GetVertices(), mesh.GetIndices());
		SetShaderProperties();
	}

	//--------------------------------------------------------------------------------------------------------------
	// Private functions implementation.
	//--------------------------------------------------------------------------------------------------------------
	void Renderer::SetShaderProperties()
	{
		_shaderProgram.Bind();
		glm::mat4 matrix = glm::mat4(1.0f);


		_shaderProgram.SetUniform("modelToProjection", matrix);
		_shaderProgram.SetUniform("modelToProjection", matrix);

		// set basic properties
		_shaderProgram.SetUniform("objectColour", glm::vec3(1.0f,0.5f,0.31f));
		_shaderProgram.SetUniform("isTextureEnabled",false);

		_shaderProgram.SetUniform("lightPosition", glm::vec3(0.9f * std::cos(glfwGetTime()),0.9f * std::sin(glfwGetTime()),0.9f));
		_shaderProgram.SetUniform("lightColour", glm::vec3(1.0f,1.0f,1.0f));
		_shaderProgram.SetUniform("viewPosition", glm::vec3(0.0f,0.0f,1.0f));
	}




}
//}