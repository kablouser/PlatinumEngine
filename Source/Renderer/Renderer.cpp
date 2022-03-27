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
const std::string MESH_VERTEX_SHADER =
#include <Shaders/Unlit/MeshShader.vert>
;
const std::string MESH_FRAGMENT_SHADER =
#include <Shaders/Unlit/MeshShader.frag>
;
const std::string LIGHT_VERTEX_SHADER =
#include <Shaders/Unlit/LightShader.vert>
;
const std::string LIGHT_FRAGMENT_SHADER =
#include <Shaders/Unlit/LightShader.frag>
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

		if (!_meshShader.Compile(MESH_VERTEX_SHADER, MESH_FRAGMENT_SHADER))
			return;

		if(!_lightShader.Compile(LIGHT_VERTEX_SHADER, LIGHT_FRAGMENT_SHADER))
			return;

		_framebufferWidth = 1;
		_framebufferHeight = 1;
		if (!_framebuffer.Create(_framebufferWidth, _framebufferHeight))
			return;

		// initialize light
		light.ambientStrength = Maths::Vec3(0.2f, 0.2f, 0.2f);
		light.diffuseStrength = Maths::Vec3(0.5f, 0.5f, 0.5f);
		light.specularStrength = Maths::Vec3(1.0f, 1.0f, 1.0f);
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
		_framebuffer.Bind();
		glEnable(GL_DEPTH_TEST);
		GL_CHECK(glViewport(0, 0, _framebufferWidth, _framebufferHeight));
		GL_CHECK(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
	}

	void Renderer::End()
	{
		glDisable(GL_DEPTH_TEST);
		_framebuffer.Unbind();
		GL_CHECK(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
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
		mesh.Render();
	}

	// update model matrix in shader
	void Renderer::SetModelMatrix(Maths::Mat4 mat)
	{
		//mat.SetRotationMatrix(Maths::Vec3(0.5f * (float)glfwGetTime() * 50.0f / 180.0f * 3.14f, 1.0f, 0.0f));
		_meshShader.SetUniform("model", mat);
	}

	// update view matrix in shader
	void Renderer::SetViewMatrix(Maths::Mat4 mat)
	{
		//glm::mat4 view = GetViewMatrix();
		_meshShader.SetUniform("view", mat);
	}

	// update perspective matrix in shader
	void Renderer::SetProjectionMatrix(Maths::Mat4 mat)
	{
		_meshShader.SetUniform("projection", mat);
	}

	void Renderer::Render()
	{
		_meshShaderInput.Draw();
		_meshShader.Unbind();
	}

	void Renderer::LoadLight()
	{
		_lightShader.Bind();

		// model view projection matrix
		Maths::Mat4 translateMat, scaleMat, matrix;
		scaleMat.SetScaleMatrix(Maths::Vec3(0.2f,0.2f,0.2f));
		translateMat.SetTranslationMatrix(light.lightPos);
		matrix = translateMat * scaleMat;
		_lightShader.SetUniform("model", matrix);
		glm::mat4 view = GetViewMatrix();
		_lightShader.SetUniform("view", view);
		_lightShader.SetUniform("projection", matrix);

		_lightShaderInput.Set(vertices, indices);
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
				RenderComponent renderComponent(mesh);
				LoadMesh(mesh);

				LoadLight();
				_lightShaderInput.Draw();
				_lightShader.Unbind();
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
		light.lightPos = Maths::Vec3(0.9f * std::cos(glfwGetTime()),0.9f * std::sin(glfwGetTime()),0.9f);
		// set basic properties
		_meshShader.SetUniform("objectColour", Maths::Vec3(1.0f,0.5f,0.31f));
		_meshShader.SetUniform("isTextureEnabled",false);

		_meshShader.SetUniform("light.position", light.lightPos);
		_meshShader.SetUniform("light.ambientStrength", light.ambientStrength);
		_meshShader.SetUniform("light.diffuseStrength", light.diffuseStrength);
		_meshShader.SetUniform("light.specularStrength", light.specularStrength);
		_meshShader.SetUniform("viewPos", Maths::Vec3(0.0f,0.0f,1.0f));
	}

	glm::mat4 Renderer::GetViewMatrix()
	{
		// calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(-90.0f)) * cos(glm::radians(0.0f));
		front.y = sin(glm::radians(0.0f));
		front.z = sin(glm::radians(-90.0f)) * cos(glm::radians(0.0f));
		glm::vec3 Front = glm::normalize(front);
		glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		// also re-calculate the Right and Up vector
		glm::vec3 Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		glm::vec3 Up    = glm::normalize(glm::cross(Right, Front));
		return glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f) + Front, Up);
	}

	void Renderer::CubeTest()
	{
		_meshShader.Bind();
		Maths::Mat4 matrix;
		matrix.SetIdentityMatrix();
		SetModelMatrix(matrix);
		SetViewMatrix(matrix);
		SetLightProperties();

		// in variables;
		//		SetProjectionMatrix
		_meshShaderInput.Set({
						{{ -0.5f, -0.5f, -0.5f  }, {  0.0f,  0.0f, -1.0f }, { 0.0f,  0.0f, }},
						{{ 0.5f, -0.5f, -0.5f   }, { 0.0f,  0.0f, -1.0f  }, { 1.0f,  0.0f, }},
						{{ 0.5f,  0.5f, -0.5f   }, { 0.0f,  0.0f, -1.0f  }, { 1.0f,  1.0f, }},
						{{ 0.5f,  0.5f, -0.5f   }, { 0.0f,  0.0f, -1.0f  }, { 1.0f,  1.0f, }},
						{{ -0.5f,  0.5f, -0.5f  }, {  0.0f,  0.0f, -1.0f }, { 0.0f,  1.0f, }},
						{{ -0.5f, -0.5f, -0.5f  }, {  0.0f,  0.0f, -1.0f }, { 0.0f,  0.0f, }},
						{{ -0.5f, -0.5f,  0.5f  }, {  0.0f,  0.0f,  1.0f }, { 0.0f,  0.0f, }},
						{{ 0.5f, -0.5f,  0.5f   }, { 0.0f,  0.0f,  1.0f  }, { 1.0f,  0.0f, }},
						{{ 0.5f,  0.5f,  0.5f   }, { 0.0f,  0.0f,  1.0f  }, { 1.0f,  1.0f, }},
						{{ 0.5f,  0.5f,  0.5f   }, { 0.0f,  0.0f,  1.0f  }, { 1.0f,  1.0f, }},
						{{ -0.5f,  0.5f,  0.5f  }, {  0.0f,  0.0f,  1.0f }, { 0.0f,  1.0f, }},
						{{ -0.5f, -0.5f,  0.5f  }, {  0.0f,  0.0f,  1.0f }, { 0.0f,  0.0f, }},
						{{ -0.5f,  0.5f,  0.5f  }, { -1.0f,  0.0f,  0.0f }, { 1.0f,  0.0f, }},
						{{ -0.5f,  0.5f, -0.5f  }, { -1.0f,  0.0f,  0.0f }, { 1.0f,  1.0f, }},
						{{ -0.5f, -0.5f, -0.5f  }, { -1.0f,  0.0f,  0.0f }, { 0.0f,  1.0f, }},
						{{ -0.5f, -0.5f, -0.5f  }, { -1.0f,  0.0f,  0.0f }, { 0.0f,  1.0f, }},
						{{ -0.5f, -0.5f,  0.5f  }, { -1.0f,  0.0f,  0.0f }, { 0.0f,  0.0f, }},
						{{ -0.5f,  0.5f,  0.5f  }, { -1.0f,  0.0f,  0.0f }, { 1.0f,  0.0f, }},
						{{ 0.5f,  0.5f,  0.5f   }, { 1.0f,  0.0f,  0.0f  }, { 1.0f,  0.0f, }},
						{{ 0.5f,  0.5f, -0.5f   }, { 1.0f,  0.0f,  0.0f  }, { 1.0f,  1.0f, }},
						{{ 0.5f, -0.5f, -0.5f   }, { 1.0f,  0.0f,  0.0f  }, { 0.0f,  1.0f, }},
						{{ 0.5f, -0.5f, -0.5f   }, { 1.0f,  0.0f,  0.0f  }, { 0.0f,  1.0f, }},
						{{ 0.5f, -0.5f,  0.5f   }, { 1.0f,  0.0f,  0.0f  }, { 0.0f,  0.0f, }},
						{{ 0.5f,  0.5f,  0.5f   }, { 1.0f,  0.0f,  0.0f  }, { 1.0f,  0.0f, }},
						{{ -0.5f, -0.5f, -0.5f  }, {  0.0f, -1.0f,  0.0f }, { 0.0f,  1.0f, }},
						{{ 0.5f, -0.5f, -0.5f   }, { 0.0f, -1.0f,  0.0f  }, { 1.0f,  1.0f, }},
						{{ 0.5f, -0.5f,  0.5f   }, { 0.0f, -1.0f,  0.0f  }, { 1.0f,  0.0f, }},
						{{ 0.5f, -0.5f,  0.5f   }, { 0.0f, -1.0f,  0.0f  }, { 1.0f,  0.0f, }},
						{{ -0.5f, -0.5f,  0.5f  }, {  0.0f, -1.0f,  0.0f }, { 0.0f,  0.0f, }},
						{{ -0.5f, -0.5f, -0.5f  }, {  0.0f, -1.0f,  0.0f }, { 0.0f,  1.0f, }},
						{{ -0.5f,  0.5f, -0.5f  }, {  0.0f,  1.0f,  0.0f }, { 0.0f,  1.0f, }},
						{{ 0.5f,  0.5f, -0.5f   }, { 0.0f,  1.0f,  0.0f  }, { 1.0f,  1.0f, }},
						{{ 0.5f,  0.5f,  0.5f   }, { 0.0f,  1.0f,  0.0f  }, { 1.0f,  0.0f, }},
						{{ 0.5f,  0.5f,  0.5f   }, { 0.0f,  1.0f,  0.0f  }, { 1.0f,  0.0f, }},
						{{ -0.5f,  0.5f,  0.5f  }, {  0.0f,  1.0f,  0.0f }, { 0.0f,  0.0f, }},
						{{ -0.5f,  0.5f, -0.5f  }, {  0.0f,  1.0f,  0.0  }, { 0.0f,  1.0f }},
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
//}