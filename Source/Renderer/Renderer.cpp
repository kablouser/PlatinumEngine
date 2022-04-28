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
const std::string REFLECT_REFRACT_VERTEX_SHADER =
#include <Shaders/Unlit/ReflectionRefraction.vert>
;
const std::string REFLECT_REFRACT_FRAGMENT_SHADER =
#include <Shaders/Unlit/ReflectionRefraction.frag>
;
const std::string PARTICLE_VERTEX_SHADER =
#include <Shaders/Unlit/ParticleShader.vert>
;
const std::string PARTICLE_FRAGMENT_SHADER =
#include <Shaders/Unlit/ParticleShader.frag>
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

		if (!_reflectRefractShader.Compile(REFLECT_REFRACT_VERTEX_SHADER, REFLECT_REFRACT_FRAGMENT_SHADER))
		{
			PLATINUM_ERROR("Cannot generate the reflect/refract shader.");
			return;
		}
		
		if (!_particleShader.Compile(PARTICLE_VERTEX_SHADER, PARTICLE_FRAGMENT_SHADER))
		{
			PLATINUM_ERROR("Cannot generate the particle shader.");
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

		particleRenderer.Init();

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

	void Renderer::BeginParticleShader()
	{
		_particleShader.Bind();
	}

	void Renderer::EndParticleShader()
	{
		_particleShader.Unbind();
	}

	void Renderer::SetMaxLifeParticleShader(const float maxLife)
	{
		_particleShader.Bind();
		_particleShader.SetUniform("maxLife", maxLife);
	}

	void Renderer::SetStartColourParticleShader(Maths::Vec4 startColour)
	{
		_particleShader.Bind();
		_particleShader.SetUniform("StartColour", startColour);
	}

	void Renderer::SetEndColourParticleShader(Maths::Vec4 endColour)
	{
		_particleShader.Bind();
		_particleShader.SetUniform("EndColour", endColour);
	}

	void Renderer::SetControlPointParticleShader(float time[4], Maths::Vec4 colour, int P)
	{
		_particleShader.Bind();
		switch (P)
		{
		case 2:
		{
			_particleShader.SetUniform("P2r", Maths::Vec2(time[0], colour.r));
			_particleShader.SetUniform("P2g", Maths::Vec2(time[1], colour.g));
			_particleShader.SetUniform("P2b", Maths::Vec2(time[2], colour.b));
			_particleShader.SetUniform("P2a", Maths::Vec2(time[3], colour.a));
			break;
		}
		case 3:
		{
			_particleShader.SetUniform("P3r", Maths::Vec2(time[0], colour.r));
			_particleShader.SetUniform("P3g", Maths::Vec2(time[1], colour.g));
			_particleShader.SetUniform("P3b", Maths::Vec2(time[2], colour.b));
			_particleShader.SetUniform("P3a", Maths::Vec2(time[3], colour.a));
			break;
		}
		default:
		{
			_particleShader.SetUniform("P2r", Maths::Vec2(time[0], colour.r));
			_particleShader.SetUniform("P2g", Maths::Vec2(time[1], colour.g));
			_particleShader.SetUniform("P2b", Maths::Vec2(time[2], colour.b));
			_particleShader.SetUniform("P2a", Maths::Vec2(time[3], colour.a));
			break;
		}

		}
	}

	void Renderer::SetTextureParticleShader(Texture* texture, bool useTexture)
	{
		_particleShader.Bind();
		_particleShader.SetUniform("useTexture", useTexture);
		if (texture)
		{
			_particleShader.SetUniform("sampleTexture", (int)0);
			glActiveTexture(GL_TEXTURE0);
			texture->Bind();
		}
	}

	void Renderer::SetShadeBy(const std::string &shadeBy)
	{
		_particleShader.Bind();

		// Set all shade by to false
		_particleShader.SetUniform("useShadeByLife", false);
		_particleShader.SetUniform("useShadeByPosition", false);
		_particleShader.SetUniform("useShadeBySpeed", false);
		_particleShader.SetUniform("useShadeBySize", false);
		// Then set the selected shade by to true
		if (shadeBy == "Life")
			_particleShader.SetUniform("useShadeByLife", true);
		else if (shadeBy == "Position")
			_particleShader.SetUniform("useShadeByPosition", true);
		else if (shadeBy == "Speed")
			_particleShader.SetUniform("useShadeBySpeed", true);
		else if (shadeBy == "Size")
			_particleShader.SetUniform("useShadeBySize", true);
		else
			_particleShader.SetUniform("useShadeByLife", true); // as a back up plan
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
		if (material.useReflectionShader)
		{
			_reflectRefractShader.Bind();
			_reflectRefractShader.SetUniform("useReflection", true);
			_reflectRefractShader.SetUniform("refractionIndex", material.refractionIndex);
			_reflectRefractShader.SetUniform("skybox", 7);
			return;
		}
		if (material.useRefractionShader)
		{
			_reflectRefractShader.Bind();
			_reflectRefractShader.SetUniform("useReflection", false);
			_reflectRefractShader.SetUniform("refractionIndex", material.refractionIndex);
			_reflectRefractShader.SetUniform("skybox", 7);
			return;
		}

		_phongShader.Bind();
		_phongShader.SetUniform("useTexture", material.useTexture);
		if (material.diffuseTexture)
		{
			_phongShader.SetUniform("diffuseMap", (int)0);
			glActiveTexture(GL_TEXTURE0);
			material.diffuseTexture->Bind();
		}

		_phongShader.SetUniform("useNormalTexture", material.useNormalTexture);
		if (material.normalTexture)
		{
			_phongShader.SetUniform("normalMap", (int)1);
			glActiveTexture(GL_TEXTURE1);
			material.normalTexture->Bind();
		}

		// Other uniforms
		_phongShader.SetUniform("materialSpec", Maths::Vec3(0.1f, 0.1f, 0.1f));
		_phongShader.SetUniform("shininess", material.shininessFactor);
		_phongShader.SetUniform("useBlinnPhong", material.useBlinnPhong);

		// Reset state
		glActiveTexture(GL_TEXTURE0);
	}

	// update model matrix in shader
	void Renderer::SetModelMatrix(Maths::Mat4 mat)
	{
		_reflectRefractShader.Bind();
		_reflectRefractShader.SetUniform("model", mat);
		_particleShader.Bind();
		_particleShader.SetUniform("model", mat);
		_phongShader.Bind();
		_phongShader.SetUniform("model", mat);
	}

	// update view matrix in shader
	void Renderer::SetViewMatrix(Maths::Mat4 mat)
	{
		_reflectRefractShader.Bind();
		_reflectRefractShader.SetUniform("view", mat);
		_particleShader.Bind();
		_particleShader.SetUniform("view", mat);
		_phongShader.Bind();
		_phongShader.SetUniform("view", mat);
	}

	// update perspective matrix in shader
	void Renderer::SetProjectionMatrix(Maths::Mat4 mat)
	{
		_reflectRefractShader.Bind();
		_reflectRefractShader.SetUniform("projection", mat);
		_particleShader.Bind();
		_particleShader.SetUniform("projection", mat);
		_phongShader.Bind();
		_phongShader.SetUniform("projection", mat);
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
		auto lightPos = Maths::Vec3(5.0f, 5.0f, -5.0f);
		auto isPointLight = true;
		auto lightAmbient = Maths::Vec3(0.2f, 0.2f, 0.2f);
		auto lightDiffuse = Maths::Vec3(0.5f, 0.5f, 0.5f);
		auto lightSpecular = Maths::Vec3(1.0f, 1.0f, 1.0f);

		// Phong shader light properties
		_phongShader.Bind();
		_phongShader.SetUniform("lightPos", lightPos);
		_phongShader.SetUniform("isPointLight", isPointLight);
		_phongShader.SetUniform("lightAmbient", lightAmbient);
		_phongShader.SetUniform("lightDiffuse", lightDiffuse);
		_phongShader.SetUniform("lightSpecular", lightSpecular);
		_phongShader.SetUniform("viewPos", Maths::Vec3(0.0, 0.0, 0.0));
	}

	void Renderer::SetCameraPos(const Maths::Vec3 &pos)
	{
		cameraPos = pos;
		_reflectRefractShader.Bind();
		_reflectRefractShader.SetUniform("cameraPos", pos);
		_particleShader.Bind();
		_particleShader.SetUniform("cameraPos", pos);
	}
}
