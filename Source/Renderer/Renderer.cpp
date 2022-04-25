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
const std::string PHONG_VERTEX_SHADER =
#include <Shaders/Lit/Phong.vert>
;
const std::string PHONG_FRAGMENT_SHADER =
#include <Shaders/Lit/Phong.frag>
;
const std::string NORMAL_VERTEX_SHADER =
#include <Shaders/Lit/NormalMappingVertShader.vert>
;
const std::string NORMAL_FRAGMENT_SHADER =
#include <Shaders/Lit/NormalMappingFragShader.frag>
;
const std::string SKYBOX_VERTEX_SHADER =
#include <Shaders/Unlit/SkyBoxShader.vert>
;
const std::string SKYBOX_FRAGMENT_SHADER =
#include <Shaders/Unlit/SkyBoxShader.frag>
;
const std::string GRID_VERTEX_SHADER =
#include <Shaders/Unlit/GridShader.vert>
;
const std::string GRID_FRAGMENT_SHADER =
#include <Shaders/Unlit/GridShader.frag>
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
			PLATINUM_INFO("Vendor: " + std::string((char*)glGetString(GL_VENDOR)));
			PLATINUM_INFO("Renderer: " + std::string((char*)glGetString(GL_RENDERER)));
			PLATINUM_INFO("OpenGL version: " + std::string((char*)glGetString(GL_VERSION)));
			PLATINUM_INFO("GLSL version: " + std::string((char*)glGetString(GL_SHADING_LANGUAGE_VERSION)));
			// TODO
			// check if version meets minimum requirements here
			// need to parse string
		}

		// glew imports all OpenGL extension methods
		GLenum errorCode = glewInit();
		if (errorCode != GLEW_OK)
		{
			PLATINUM_ERROR(
					std::string("GLEW initialisation error. ") + std::string((char*)glewGetErrorString(errorCode)));
			// without GLEW, seg faults will happen
			return;
		}

		if (!_unlitShader.Compile(UNLIT_VERTEX_SHADER, UNLIT_FRAGMENT_SHADER))
			return;

		if (!_skyBoxShader.Compile(SKYBOX_VERTEX_SHADER, SKYBOX_FRAGMENT_SHADER))
		{
			PLATINUM_ERROR("Cannot generate the sky box shader.");
			return;
		}

		if(!_gridShader.Compile(GRID_VERTEX_SHADER, GRID_FRAGMENT_SHADER))
		{
			PLATINUM_ERROR("Cannot generate the grid shader.");
			return;
		}

		if (!_phongShader.Compile(PHONG_VERTEX_SHADER, PHONG_FRAGMENT_SHADER))
		{
			PLATINUM_ERROR("Cannot generate the phong shader.");
			return;
		}

		if (!_normalMapShader.Compile(NORMAL_VERTEX_SHADER, NORMAL_FRAGMENT_SHADER))
		{
			PLATINUM_ERROR("Cannot generate the normal map shader.");
			return;
		}

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
//		_unlitShader.Bind();
		_phongShader.Bind();
	}

	void Renderer::End()
	{
//		glDisable(GL_DEPTH_TEST);
//		_framebuffer.Unbind();
//		GL_CHECK(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
//		GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
//		_unlitShader.Unbind();
		_phongShader.Unbind();
	}

	void Renderer::BeginSkyBoxShader()
	{
//		_framebuffer.Bind();

//		glEnable(GL_DEPTH_TEST);
//		GL_CHECK(glViewport(0, 0, _framebufferWidth, _framebufferHeight));
//		GL_CHECK(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
//		GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
		_skyBoxShader.Bind();
	}

	void Renderer::EndSkyBoxShader()
	{
//		glDisable(GL_DEPTH_TEST);
//		_framebuffer.Unbind();
//		GL_CHECK(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
//		GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
		_skyBoxShader.Unbind();
	}

	void Renderer::BeginGrid()
	{
		_gridShader.Bind();
	}

	void Renderer::EndGrid()
	{
		_gridShader.Unbind();
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

	void Renderer::LoadMaterial(const Material& material)
	{

		// Use diffuse texture on its own
		// Use normal texture on its own
		// Use normal and diffuse texture

		// Check case for using normal and diffuse texture as this will require normal shader
		if (material.useNormalTexture && material.useTexture && material.normalTexture && material.diffuseTexture)
		{
			_normalMapShader.Bind();
			// Bind diffuse map
			_normalMapShader.SetUniform("diffuseMap", (int)0);
			glActiveTexture(GL_TEXTURE0);
			material.diffuseTexture->Bind();

			// bind normal map
			_normalMapShader.SetUniform("normalMap", (int)1);
			glActiveTexture(GL_TEXTURE1);
			material.normalTexture->Bind();

			// Other uniforms
			_normalMapShader.SetUniform("materialSpec", Maths::Vec3(0.5f, 0.5f, 0.5f));
			_normalMapShader.SetUniform("shininess", material.shininessFactor);
			_normalMapShader.SetUniform("useBlinnPhong", material.useBlinnPhong);
		}
		else
		{
			_phongShader.Bind();

			// Check if normal texture is given but no diffuse texture is given
			if (material.normalTexture && material.useNormalTexture && !material.useTexture)
			{
				// Use the normal texture as the diffuse texture so a user can see it on screen
				_phongShader.SetUniform("useTexture", true);
				_phongShader.SetUniform("diffuseMap", (int) 0);
				glActiveTexture(GL_TEXTURE0);
				material.normalTexture->Bind();
			}
			// Only diffuse texture given
			else if (material.diffuseTexture && material.useTexture)
			{
				_phongShader.SetUniform("useTexture", true);
				_phongShader.SetUniform("diffuseMap", (int) 0);
				glActiveTexture(GL_TEXTURE0);
				material.diffuseTexture->Bind();
			}
			// No texture given
			else {
				_phongShader.SetUniform("useTexture", false);
			}

			// Other uniforms
			_phongShader.SetUniform("materialSpec", Maths::Vec3(0.5f, 0.5f, 0.5f));
			_phongShader.SetUniform("shininess", material.shininessFactor);
			_phongShader.SetUniform("useBlinnPhong", material.useBlinnPhong);

			// TODO: This code is what was being used for unlit shader by Jinyuan, will leave this here for now
//			_unlitShader.Bind();
//
//			// bind diffuse map
//			if (material.diffuseTexture)
//			{
//				_unlitShader.SetUniform("diffuseMap", 0);
//				glActiveTexture(GL_TEXTURE0);
//				material.diffuseTexture->Bind();
//				glActiveTexture(GL_TEXTURE0);
//			}
//			// TODO: bind specular map
////			if(material.specularTexture)
////			{
////				_unlitShader.SetUniform("specularMap", 1);
////				glActiveTexture(GL_TEXTURE1);
////				material.specularTexture->Bind();
////				glActiveTexture(GL_TEXTURE0);
////			}
//			// TODO: bind normal map (should this be here I don't think so?)
////			if(material.normalTexture)
////			{
////				_unlitShader.SetUniform("normalMap", 2);
////				glActiveTexture(GL_TEXTURE2);
////				material.normalTexture->Bind();
////			}
//			_unlitShader.SetUniform("shininess", material.shininessFactor);
		}
	}

	// update model matrix in shader
	void Renderer::SetModelMatrix(Maths::Mat4 mat)
	{
		_normalMapShader.Bind();
		_normalMapShader.SetUniform("model", mat);
		_normalMapShader.Unbind();
		_phongShader.Bind();
		_phongShader.SetUniform("model", mat);
//		_unlitShader.Bind();
//		_unlitShader.SetUniform("model", mat);
	}

	// update view matrix in shader
	void Renderer::SetViewMatrix(Maths::Mat4 mat)
	{
		//glm::mat4 view = GetViewMatrix();
		_normalMapShader.Bind();
		_normalMapShader.SetUniform("view", mat);
		_normalMapShader.Unbind();
		_phongShader.Bind();
		_phongShader.SetUniform("view", mat);
//		_unlitShader.Bind();
//		_unlitShader.SetUniform("view", mat);
	}

	// update perspective matrix in shader
	void Renderer::SetProjectionMatrix(Maths::Mat4 mat)
	{
		_normalMapShader.Bind();
		_normalMapShader.SetUniform("projection", mat);
		_normalMapShader.Unbind();
		_phongShader.Bind();
		_phongShader.SetUniform("projection", mat);
//		_unlitShader.Bind();
//		_unlitShader.SetUniform("projection", mat);
	}

	// update view matrix in shader
	void Renderer::SetViewMatrixSkyBox(Maths::Mat4 mat)
	{
		_skyBoxShader.SetUniform("view", mat);
	}

	// update perspective matrix in shader
	void Renderer::SetProjectionMatrixSkyBox(Maths::Mat4 mat)
	{
		_skyBoxShader.SetUniform("projection", mat);
	}

	// update view matrix in shader
	void Renderer::SetViewMatrixForGridShader(Maths::Mat4 mat)
	{
		//glm::mat4 view = GetViewMatrix();
		_gridShader.SetUniform("view", mat);
	}

	// update perspective matrix in shader
	void Renderer::SetProjectionMatrixForGridShader(Maths::Mat4 mat)
	{
		_gridShader.SetUniform("projection", mat);
	}

	void Renderer::SetFarValueForGridShader(float far)
	{
		_gridShader.SetUniform("far", far);
	}

	void Renderer::SetNearValueForGridShader(float near)
	{
		_gridShader.SetUniform("near", near);
	}

	void Renderer::SetTransparencyForGridShader(float transparency)
	{
		_gridShader.SetUniform("transparency", transparency);
	}

	void Renderer::SetGridAxisForGridShader(int gridAxis)
	{
		_gridShader.SetUniform("GridAxis", gridAxis);
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
		// Normal shader light properties
		auto lightPos = Maths::Vec3(100.0f, 100.0f, 100.0f);
		auto isPointLight = true;
		auto lightAmbient = Maths::Vec3(0.2f, 0.2f, 0.2f);
		auto lightDiffuse = Maths::Vec3(0.5f, 0.5f, 0.5f);
		auto lightSpecular = Maths::Vec3(1.0f, 1.0f, 1.0f);
		_normalMapShader.Bind();
		_normalMapShader.SetUniform("lightPos", lightPos);
		_normalMapShader.SetUniform("isPointLight", isPointLight);
		_normalMapShader.SetUniform("lightAmbient", lightAmbient);
		_normalMapShader.SetUniform("lightDiffuse", lightDiffuse);
		_normalMapShader.SetUniform("lightSpecular", Maths::Vec3(1.0f, 1.0f, 1.0f));

		// Phong shader light properties
		_phongShader.Bind();
		_phongShader.SetUniform("lightPos", lightPos);
		_phongShader.SetUniform("isPointLight", isPointLight);
		_phongShader.SetUniform("lightAmbient", lightAmbient);
		_phongShader.SetUniform("lightDiffuse", lightDiffuse);
		_phongShader.SetUniform("lightSpecular", lightSpecular);

//		pointLight.lightPos = Maths::Vec3(0.0f, 2.0f, 2.0f);
		// pointLight.lightPos = Maths::Vec3(0.9f * (float)std::cos(glfwGetTime()),0.9f * (float)std::sin(glfwGetTime()),0.9f);

//		_unlitShader.Bind();
//		_unlitShader.SetUniform("lightColor", Maths::Vec3(1.0f, 1.0f, 1.0f));
//		_unlitShader.SetUniform("lightPos", pointLight.lightPos);
//		// set basic properties
//		_unlitShader.SetUniform("objectColour", Maths::Vec3(1.0f,0.5f,0.31f));
//
//		_unlitShader.SetUniform("pointLight.position", pointLight.lightPos);
//		_unlitShader.SetUniform("pointLight.ambient", pointLight.ambientStrength);
//		_unlitShader.SetUniform("pointLight.diffuse", pointLight.diffuseStrength);
//		_unlitShader.SetUniform("pointLight.specular", pointLight.specularStrength);
//
//		_unlitShader.SetUniform("pointLight.constant", pointLight.constant);
//		_unlitShader.SetUniform("pointLight.linear", pointLight.linear);
//		_unlitShader.SetUniform("pointLight.quadratic", pointLight.quadratic);
//		_unlitShader.SetUniform("viewPosition", Maths::Vec3(0.0, 0.0, 10.0));
//
//		_unlitShader.SetUniform("isPointLight", true);
	}
}
//}