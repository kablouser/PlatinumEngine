#include <Renderer/Renderer.h>
// checking errors
#include <OpenGL/GLCheck.h>
// gui
#include <imgui.h>
// OpenGL extensions
#include <GL/glew.h>
// printing errors
#include <Logger/Logger.h>
#include <ComponentComposition/LightComponent.h>
#include <ComponentComposition/TransformComponent.h>

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

		if (!_lightShader.Compile(LIGHT_VERTEX_SHADER, LIGHT_FRAGMENT_SHADER))
		{
			PLATINUM_ERROR("Cannot generate the reflect/refract shader.");
			return;
		}

		_framebufferWidth = 1;
		_framebufferHeight = 1;
		if (!_framebuffer.Create(_framebufferWidth, _framebufferHeight))
			return;

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
		_phongShader.SetUniform("materialSpec", Maths::Vec3(0.6f, 0.6f, 0.6f));
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
		_phongShader.Bind();
		_phongShader.SetUniform("model", mat);
	}

	// update view matrix in shader
	void Renderer::SetViewMatrix(Maths::Mat4 mat)
	{
		_reflectRefractShader.Bind();
		_reflectRefractShader.SetUniform("view", mat);
		_phongShader.Bind();
		_phongShader.SetUniform("view", mat);
		_lightShader.Bind();
		_lightShader.SetUniform("view", mat);
	}

	// update perspective matrix in shader
	void Renderer::SetProjectionMatrix(Maths::Mat4 mat)
	{
		_reflectRefractShader.Bind();
		_reflectRefractShader.SetUniform("projection", mat);
		_phongShader.Bind();
		_phongShader.SetUniform("projection", mat);
		_lightShader.Bind();
		_lightShader.SetUniform("projection", mat);
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

	void Renderer::SetupLights(std::vector<GameObject*> &lights)
	{
		_phongShader.Bind();
		int num_directed_lights, num_point_lights;
		num_directed_lights = num_point_lights = 0;
		bool isDirLight = false, isPointLight = false;
		for(auto light:lights)
		{
			auto lightComponent = light->GetComponent<LightComponent>();
			if(lightComponent)
			{
				LightComponent::LightType type = lightComponent->type;
				auto transform = light->GetComponent<TransformComponent>();
				if(transform)
				{
					if (type == LightComponent::LightType::Directional)
					{
						isDirLight = true;
						auto lightDir = transform->GetLocalToWorldMatrix() * Maths::Vec4(0.f, 1.f, 0.f, 0.f);
						_phongShader.SetUniform("ambientLight", lightComponent->ambientLight);
						_phongShader.SetUniform("isDirLight", isDirLight);
						_phongShader.SetUniform("dirLights[" + std::to_string(num_directed_lights) + "].direction",
								Maths::Vec3(lightDir.x, lightDir.y, lightDir.z));
						_phongShader.SetUniform("dirLights[" + std::to_string(num_directed_lights) + "].baseLight",
								(lightComponent->intensity * lightComponent->spectrum).to_vec());
						num_directed_lights++;
					}
					else if (type == LightComponent::LightType::Point)
					{
						isPointLight = true;
						_phongShader.SetUniform("ambientLight", lightComponent->ambientLight);
						_phongShader.SetUniform("isPointLight", isPointLight);
						_phongShader.SetUniform("pointLights[" + std::to_string(num_point_lights) + "].position",
								transform->localPosition);
						_phongShader.SetUniform("pointLights[" + std::to_string(num_point_lights) + "].baseLight",
								(lightComponent->intensity * lightComponent->spectrum).to_vec());
						_phongShader.SetUniform("pointLights[" + std::to_string(num_point_lights) + "].constant",
								lightComponent->constant);
						_phongShader.SetUniform("pointLights[" + std::to_string(num_point_lights) + "].linear",
								lightComponent->linear);
						_phongShader.SetUniform("pointLights[" + std::to_string(num_point_lights) + "].quadratic",
								lightComponent->quadratic);
						num_point_lights++;
					}
				}
			}
		}
	}

	void Renderer::DrawLight(Maths::Mat4 matrix)
	{
		_lightShader.Bind();
		_lightShader.SetUniform("model", matrix);
	}

	void Renderer::SetCameraPos(const Maths::Vec3 &pos)
	{
		_reflectRefractShader.Bind();
		_reflectRefractShader.SetUniform("cameraPos", pos);

		_phongShader.Bind();
		_phongShader.SetUniform("viewPos", pos);
	}
}
