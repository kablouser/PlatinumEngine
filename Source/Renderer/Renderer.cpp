#include <Application.h>
#include <Renderer/Renderer.h>
// checking errors
#include <OpenGL/GLCheck.h>
// gui
#include <imgui.h>
// OpenGL extensions
#include <GL/glew.h>
// printing errors
#include <Logger/Logger.h>
#include <ComponentComposition/Light.h>
#include <ComponentComposition/Transform.h>

// test
#include "ComponentComposition/MeshRender.h"

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
		_skyBoxShader.Bind();
	}

	void Renderer::EndSkyBoxShader()
	{

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

	void Renderer::SetTextureParticleShader(SavedReference<Texture> texture, bool useTexture, int numCols, int numRows)
	{
		_particleShader.Bind();
		_particleShader.SetUniform("useTexture", useTexture);
		_particleShader.SetUniform("sampleTexture", (int)0);

		if (texture)
		{
			glActiveTexture(GL_TEXTURE0);
			texture.DeRef()->Bind();

			_particleShader.SetUniform("textureWidth", (float)texture.DeRef()->width);
			_particleShader.SetUniform("textureHeight", (float)texture.DeRef()->height);
			_particleShader.SetUniform("spriteWidth", (float)texture.DeRef()->width / (float) numCols);
			_particleShader.SetUniform("spriteHeight", (float)texture.DeRef()->height / (float) numRows);
			_particleShader.SetUniform("textureRatio", (float)texture.DeRef()->height / texture.DeRef()->width);
		}
		else
		{
			_particleShader.SetUniform("textureWidth", 0.0f);
			_particleShader.SetUniform("textureHeight", 0.0f);
			_particleShader.SetUniform("spriteWidth", 1.0f);
			_particleShader.SetUniform("spriteHeight", 1.0f);
			_particleShader.SetUniform("textureRatio", 1.0f);
		}
	}

	void Renderer::SetShadeByParticleShader(const std::string &shadeBy)
	{
		_particleShader.Bind();

		// Set all shade by to false
		_particleShader.SetUniform("useShadeByLife", false);
		_particleShader.SetUniform("useShadeByPosition", false);
		_particleShader.SetUniform("useShadeBySpeed", false);
		_particleShader.SetUniform("useShadeBySize", false);

		// It must be one of these options,
		if (shadeBy == "Life")
			_particleShader.SetUniform("useShadeByLife", true);
		else if (shadeBy == "Position")
			_particleShader.SetUniform("useShadeByPosition", true);
		else if (shadeBy == "Speed")
			_particleShader.SetUniform("useShadeBySpeed", true);
		else if (shadeBy == "Size")
			_particleShader.SetUniform("useShadeBySize", true);
	}

	void Renderer::SetVec4ParticleShader(const char* name, Maths::Vec4 vec)
	{
		_particleShader.Bind();
		_particleShader.SetUniform(name, vec);
	}

	void Renderer::SetFloatParticleShader(const char* name, float val)
	{
		_particleShader.Bind();
		_particleShader.SetUniform(name, val);
	}

	void Renderer::SetBoolParticleShader(const char* name, bool val)
	{
		_particleShader.Bind();
		_particleShader.SetUniform(name, val);
	}

	void Renderer::SetVec3ParticleShader(const char * name, const Maths::Vec3 &scaleFactors)
	{
		_particleShader.Bind();
		_particleShader.SetUniform(name, scaleFactors);
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
		_phongShader.SetUniform("diffuseColour", material.colour);
		_phongShader.SetUniform("useTexture", material.useTexture);

		// bind diffuse map
		if (material.diffuseTexture)
		{
			_phongShader.SetUniform("diffuseMap", (int)0);
			glActiveTexture(GL_TEXTURE0);
			material.diffuseTexture.DeRef()->Bind();
		}

		_phongShader.SetUniform("useNormalTexture", material.useNormalTexture);
		if (material.normalTexture)
		{
			_phongShader.SetUniform("normalMap", (int)1);
			glActiveTexture(GL_TEXTURE1);
			material.normalTexture.DeRef()->Bind();
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
		_lightShader.Bind();
		_lightShader.SetUniform("view", mat);
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
		_lightShader.Bind();
		_lightShader.SetUniform("projection", mat);
	}

	/*// update model matrix in shader
	void Renderer::SetModelMatrixAnimation(Maths::Mat4 mat)
	{
		//mat.SetRotationMatrix(Maths::Vec3(0.5f * (float)glfwGetTime() * 50.0f / 180.0f * 3.14f, 1.0f, 0.0f));
		_animationShader.SetUniform("model", mat);
	}

	// update view matrix in shader
	void Renderer::SetViewMatrixAnimation(Maths::Mat4 mat)
	{
		//glm::mat4 view = GetViewMatrix();
		_animationShader.SetUniform("view", mat);
	}

	// update perspective matrix in shader
	void Renderer::SetProjectionMatrixAnimation(Maths::Mat4 mat)
	{
		_animationShader.SetUniform("projection", mat);
	}*/

	void Renderer::SetAnimationTransform(unsigned int transformMatrixIndex, Maths::Mat4 mat)
	{
		_phongShader.Bind();
		if(transformMatrixIndex < 128)
			_phongShader.SetUniform("tracks["+std::to_string(transformMatrixIndex)+"]", mat);
		else
			PLATINUM_WARNING_STREAM << "Size of transformation matrices " << transformMatrixIndex << " for animation exceeds 128.";
	}

	void Renderer::SetAnimationStatus(bool isAnimationOn)
	{
		_phongShader.Bind();
		_phongShader.SetUniform("isAnimationDisplay", isAnimationOn);
	}

	void Renderer::SetAnimationAttachmentStatus(bool isAnimationAttachmentOn)
	{
		_phongShader.Bind();
		_phongShader.SetUniform("isAnimationAttachmentDisplay", isAnimationAttachmentOn);
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

	void Renderer::SetupLights(std::vector<SavedReference<LightComponent>> &lights)
	{
		_phongShader.Bind();
		int num_directed_lights, num_point_lights;
		num_directed_lights = num_point_lights = 0;
		bool isDirLight = false, isPointLight = false;

		// Reset uniforms
		_phongShader.SetUniform("ambientLight", Maths::Vec3{0.0f, 0.0f, 0.0f});

		for(auto light:lights)
		{
			auto lightComponent = light;
			if(lightComponent)
			{
				LightComponent::LightType type = lightComponent.DeRef()->type;
				auto transform = lightComponent.DeRef()->GetComponent<Transform>();
				if(transform)
				{
					if (type == LightComponent::LightType::Directional)
					{
						isDirLight = true;
						auto lightDir = transform.DeRef()->GetLocalToWorldMatrix() * Maths::Vec4(0.f, 1.f, 0.f, 0.f);
						_phongShader.SetUniform("ambientLight", lightComponent.DeRef()->ambientLight);
						_phongShader.SetUniform("isDirLight", isDirLight);
						_phongShader.SetUniform("dirLights[" + std::to_string(num_directed_lights) + "].direction",
								Maths::Vec3(lightDir.x, lightDir.y, lightDir.z));
						_phongShader.SetUniform("dirLights[" + std::to_string(num_directed_lights) + "].baseLight",
								(lightComponent.DeRef()->intensity * lightComponent.DeRef()->spectrum).to_vec());
						num_directed_lights++;
					}
					else if (type == LightComponent::LightType::Point)
					{
						isPointLight = true;
						_phongShader.SetUniform("ambientLight", lightComponent.DeRef()->ambientLight);
						_phongShader.SetUniform("isPointLight", isPointLight);
						auto matrix = transform.DeRef()->GetLocalToWorldMatrix();
						auto pos = Maths::Vec3{matrix[3][0], matrix[3][1], matrix[3][2]};
						_phongShader.SetUniform("pointLights[" + std::to_string(num_point_lights) + "].position",
								pos);
						_phongShader.SetUniform("pointLights[" + std::to_string(num_point_lights) + "].baseLight",
								(lightComponent.DeRef()->intensity * lightComponent.DeRef()->spectrum).to_vec());
						_phongShader.SetUniform("pointLights[" + std::to_string(num_point_lights) + "].constant",
								lightComponent.DeRef()->constant);
						_phongShader.SetUniform("pointLights[" + std::to_string(num_point_lights) + "].linear",
								lightComponent.DeRef()->linear);
						_phongShader.SetUniform("pointLights[" + std::to_string(num_point_lights) + "].quadratic",
								lightComponent.DeRef()->quadratic);
						num_point_lights++;
					}
				}
/*
				Begin();
				Mesh mesh;
				mesh.vertices = vertices;
				mesh.indices = indices;
//				MeshRender renderComponent(mesh);
				LoadMesh(mesh);
				End();

				ImGui::Image(_framebuffer.GetColorTexture().GetImGuiHandle(), targetSize);
*/
			}
		}

		_phongShader.SetUniform("numDirLights", num_directed_lights);
		_phongShader.SetUniform("numPointLights", num_point_lights);
	}

	void Renderer::DrawLight(Maths::Mat4 matrix)
	{
		_lightShader.Bind();
		_lightShader.SetUniform("model", matrix);
	}

	void Renderer::SetCameraPos(const Maths::Vec3 &pos)
	{
		cameraPos = pos;
		_reflectRefractShader.Bind();
		_reflectRefractShader.SetUniform("cameraPos", pos);

		_phongShader.Bind();
		_phongShader.SetUniform("viewPos", pos);

		_particleShader.Bind();
		_particleShader.SetUniform("cameraPos", pos);
	}

}
