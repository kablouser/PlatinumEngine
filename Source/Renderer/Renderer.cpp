#include <Renderer/Renderer.h>
// checking errors
#include <OpenGL/GLCheck.h>
// gui
#include <imgui.h>
// OpenGL extensions
#include <GL/glew.h>
// printing errors
#include <Logger/Logger.h>

// test
#include "ComponentComposition/RenderComponent.h"

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

		if (!_unlitShader.Compile(UNLIT_VERTEX_SHADER, UNLIT_FRAGMENT_SHADER))
			return;

		_framebufferWidth = 1;
		_framebufferHeight = 1;
		if (!_framebuffer.Create(_framebufferWidth, _framebufferHeight))
			return;

		// initialize light
		pointLight.constant = 1.0f;
		pointLight.linear = 0.09f;
		pointLight.quadratic = 0.032f;

		pointLight.ambientStrength = Maths::Vec3(0.2f, 0.2f, 0.2f);
		pointLight.diffuseStrength = Maths::Vec3(0.5f, 0.5f, 0.5f);
		pointLight.specularStrength = Maths::Vec3(1.0f, 1.0f, 1.0f);
		// check for uncaught errors
//		GL_CHECK();

		_isInitGood = true;
	}

	Renderer::~Renderer()
	{
		// nothing to do
	}

	void Renderer::Begin()
	{
//		_framebuffer.Bind();

//		glEnable(GL_DEPTH_TEST);
//		GL_CHECK(glViewport(0, 0, _framebufferWidth, _framebufferHeight));
//		GL_CHECK(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
//		GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
		_unlitShader.Bind();
	}

	void Renderer::End()
	{
//		glDisable(GL_DEPTH_TEST);
//		_framebuffer.Unbind();
//		GL_CHECK(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
//		GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
		_unlitShader.Unbind();
	}

	void Renderer::SetFramebuffer(Framebuffer* framebuffer)
	{
		_framebuffer = *framebuffer;
	}

	void Renderer::ResizeFrameBuffer(Framebuffer &framebuffer, ImVec2 targetSize)
	{
		_framebufferWidth = (int)targetSize.x;
		_framebufferHeight = (int)targetSize.y;
		_framebuffer.Create(_framebufferWidth, _framebufferHeight);
	}

	void Renderer::LoadMesh(Mesh &mesh)
	{
	}

	void Renderer::LoadTexture(const Material& material)
	{
		_unlitShader.Bind();
		// bind diffuse map
		if (material.diffuseTexture)
		{
			glActiveTexture(GL_TEXTURE0);
			material.diffuseTexture->Bind();
		}
		// bind specular map
		if(material.specularTexture)
		{
			glActiveTexture(GL_TEXTURE1);
			material.specularTexture->Bind();
		}
		// bind normal map
		if(material.normalTexture)
		{
			glActiveTexture(GL_TEXTURE2);
			material.normalTexture->Bind();
		}
		_unlitShader.SetUniform("shininess", material.shininessFactor);
	}
	// update model matrix in shader
	void Renderer::SetModelMatrix(Maths::Mat4 mat)
	{
		//mat.SetRotationMatrix(Maths::Vec3(0.5f * (float)glfwGetTime() * 50.0f / 180.0f * 3.14f, 1.0f, 0.0f));
		_unlitShader.SetUniform("model", mat);
	}

	// update view matrix in shader
	void Renderer::SetViewMatrix(Maths::Mat4 mat)
	{
		//glm::mat4 view = GetViewMatrix();
		_unlitShader.SetUniform("view", mat);
	}

	// update perspective matrix in shader
	void Renderer::SetProjectionMatrix(Maths::Mat4 mat)
	{
		_unlitShader.SetUniform("projection", mat);
	}

	// if you want to test a mesh use
	// void Renderer::ShowGUIWindow(bool* outIsOpen, const Mesh &mesh)
	// then comment CubeTest(), cancel comment on LoadMesh(const Mesh &mesh)
	void Renderer::ShowGUIWindow(bool* outIsOpen)
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
					ResizeFrameBuffer(_framebuffer, targetSize);
				}

				Begin();
				Mesh mesh(vertices, indices);
//				RenderComponent renderComponent(mesh);
				LoadMesh(mesh);
				End();

				ImGui::Image(_framebuffer.GetColorTexture().GetImGuiHandle(), targetSize);

			}
		}

		ImGui::End();
	}
	//--------------------------------------------------------------------------------------------------------------
	// Private functions implementation.
	//--------------------------------------------------------------------------------------------------------------
	void Renderer::SetLightProperties()
	{
		pointLight.lightPos = Maths::Vec3(0.9f * (float)std::cos(glfwGetTime()),0.9f * (float)std::sin(glfwGetTime()),0.9f);
		// set basic properties
		_unlitShader.SetUniform("objectColour", Maths::Vec3(1.0f,0.5f,0.31f));

		_unlitShader.SetUniform("pointLight.position", pointLight.lightPos);
		_unlitShader.SetUniform("pointLight.ambient", pointLight.ambientStrength);
		_unlitShader.SetUniform("pointLight.diffuse", pointLight.diffuseStrength);
		_unlitShader.SetUniform("pointLight.specular", pointLight.specularStrength);

		_unlitShader.SetUniform("pointLight.constant", pointLight.constant);
		_unlitShader.SetUniform("pointLight.linear", pointLight.linear);
		_unlitShader.SetUniform("pointLight.quadratic", pointLight.quadratic);
		_unlitShader.SetUniform("viewPosition", Maths::Vec3(0.0, 0.0, 10.0));

		_unlitShader.SetUniform("isPointLight", true);
	}

}
//}