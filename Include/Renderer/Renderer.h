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
#include "OpenGL/Material.h"

namespace PlatinumEngine
{
	class Renderer
	{

	public:
		// temporary light struct for light test
		struct PointLight
		{
			Maths::Vec3 lightPos;

			Maths::Vec3 ambientStrength;
			Maths::Vec3 diffuseStrength;
			Maths::Vec3 specularStrength;

			// coefficient
			float constant;
			float linear;
			float quadratic;
		};

		struct DirectLight
		{
			Maths::Vec3 lightDir;

			Maths::Vec3 ambientStrength;
			Maths::Vec3 diffuseStrength;
			Maths::Vec3 specularStrength;
		};

		struct SpotLight
		{
			Maths::Vec3 lightPos;
			Maths::Vec3 lightDir;

			float cutOff;
			float outerCutOff;

			Maths::Vec3 ambientStrength;
			Maths::Vec3 diffuseStrength;
			Maths::Vec3 specularStrength;

			// coefficient
			float constant;
			float linear;
			float quadratic;
		};

		// Constructors
		Renderer(bool printOpenGLInfo = true);

		~Renderer();

		// initialize framebuffer
		void Begin();

		// unbind framebuffer
		void End();

		// initialize skybox shader
		void BeginSkyBoxShader();

		// unbind skybox shader
		void EndSkyBoxShader();

		// initialize grid shader
		void BeginGrid();

		// unbind grid shader
		void EndGrid();

		/**
		 * resize framebuffer, it's an interface for SceneEditor
		 * @param framebuffer, targetSize
		 * */
		void ResizeFrameBuffer(Framebuffer &framebuffer, ImVec2 targetSize);

		// three temporary functions to update model, view, projection matrix before TransformComponent
		// update model matrix in shader
		void SetModelMatrix(Maths::Mat4 mat = Maths::Mat4(1.0));

		// update view matrix in shader
		void SetViewMatrix(Maths::Mat4 mat = Maths::Mat4(1.0));

		// update projection matrix in shader
		void SetProjectionMatrix(Maths::Mat4 mat = Maths::Mat4(1.0));

		void SetLightProperties();

		// a window for renderer to test
		void ShowGUIWindow(bool* outIsOpen);

		void SetProjectionMatrixSkyBox(Maths::Mat4 mat);

		void SetViewMatrixSkyBox(Maths::Mat4 mat);


		void SetViewMatrixForGridShader(Maths::Mat4 mat);

		void SetProjectionMatrixForGridShader(Maths::Mat4 mat);

		void SetFarValueForGridShader(float far);

		void SetNearValueForGridShader(float near);

		void SetTransparencyForGridShader(float transparency);

		void SetGridAxisForGridShader(int gridAxis);

		/**
		 * mesh component can call this function to input mesh data into shader, if you want to test a mesh renderer,
		 * use this function
		 * @param mesh
		 */
		void LoadMesh(Mesh &mesh);

		/**
		 * mesh component can call this function to activate texture
		 * @param material
		 */
		void LoadMaterial(const Material& material);
		//TODO: LoadLight
		/**
		 * light component can call this function to input light data into shader
		 * @param light
		 */
		// void LoadLight(const Light &light);
		void LoadLight();

	private:

		// true iff all init steps were successful
		bool _isInitGood;

		ShaderProgram _unlitShader;
		ShaderProgram _skyBoxShader;
		ShaderProgram _gridShader;
		ShaderProgram _phongShader;

		// ShaderInput _meshShaderInput, _lightShaderInput;

		Framebuffer _framebuffer;
		int _framebufferWidth;
		int _framebufferHeight;

		PointLight pointLight;

		// vertics
		std::vector<Vertex> vertices = {{{ -0.5f, -0.5f, -0.5f  }, {  0.0f,  0.0f, -1.0f }, { 0, 0 }},
										{{ 0.5f, -0.5f, -0.5f   }, { 0.0f,  0.0f, -1.0f  }, { 0, 0 }},
										{{ 0.5f,  0.5f, -0.5f   }, { 0.0f,  0.0f, -1.0f  }, { 0, 0 }},
										{{ 0.5f,  0.5f, -0.5f   }, { 0.0f,  0.0f, -1.0f  }, { 0, 0 }},
										{{ -0.5f,  0.5f, -0.5f  }, {  0.0f,  0.0f, -1.0f }, { 0, 0 }},
										{{ -0.5f, -0.5f, -0.5f  }, {  0.0f,  0.0f, -1.0f }, { 0, 0 }},
										{{ -0.5f, -0.5f,  0.5f  }, {  0.0f,  0.0f,  1.0f }, { 0, 0 }},
										{{ 0.5f, -0.5f,  0.5f   }, { 0.0f,  0.0f,  1.0f  }, { 0, 0 }},
										{{ 0.5f,  0.5f,  0.5f   }, { 0.0f,  0.0f,  1.0f  }, { 0, 0 }},
										{{ 0.5f,  0.5f,  0.5f   }, { 0.0f,  0.0f,  1.0f  }, { 0, 0 }},
										{{ -0.5f,  0.5f,  0.5f  }, {  0.0f,  0.0f,  1.0f }, { 0, 0 }},
										{{ -0.5f, -0.5f,  0.5f  }, {  0.0f,  0.0f,  1.0f }, { 0, 0 }},
										{{ -0.5f,  0.5f,  0.5f  }, { -1.0f,  0.0f,  0.0f }, { 0, 0 }},
										{{ -0.5f,  0.5f, -0.5f  }, { -1.0f,  0.0f,  0.0f }, { 0, 0 }},
										{{ -0.5f, -0.5f, -0.5f  }, { -1.0f,  0.0f,  0.0f }, { 0, 0 }},
										{{ -0.5f, -0.5f, -0.5f  }, { -1.0f,  0.0f,  0.0f }, { 0, 0 }},
										{{ -0.5f, -0.5f,  0.5f  }, { -1.0f,  0.0f,  0.0f }, { 0, 0 }},
										{{ -0.5f,  0.5f,  0.5f  }, { -1.0f,  0.0f,  0.0f }, { 0, 0 }},
										{{ 0.5f,  0.5f,  0.5f   }, { 1.0f,  0.0f,  0.0f  }, { 0, 0 }},
										{{ 0.5f,  0.5f, -0.5f   }, { 1.0f,  0.0f,  0.0f  }, { 0, 0 }},
										{{ 0.5f, -0.5f, -0.5f   }, { 1.0f,  0.0f,  0.0f  }, { 0, 0 }},
										{{ 0.5f, -0.5f, -0.5f   }, { 1.0f,  0.0f,  0.0f  }, { 0, 0 }},
										{{ 0.5f, -0.5f,  0.5f   }, { 1.0f,  0.0f,  0.0f  }, { 0, 0 }},
										{{ 0.5f,  0.5f,  0.5f   }, { 1.0f,  0.0f,  0.0f  }, { 0, 0 }},
										{{ -0.5f, -0.5f, -0.5f  }, {  0.0f, -1.0f,  0.0f }, { 0, 0 }},
										{{ 0.5f, -0.5f, -0.5f   }, { 0.0f, -1.0f,  0.0f  }, { 0, 0 }},
										{{ 0.5f, -0.5f,  0.5f   }, { 0.0f, -1.0f,  0.0f  }, { 0, 0 }},
										{{ 0.5f, -0.5f,  0.5f   }, { 0.0f, -1.0f,  0.0f  }, { 0, 0 }},
										{{ -0.5f, -0.5f,  0.5f  }, {  0.0f, -1.0f,  0.0f }, { 0, 0 }},
										{{ -0.5f, -0.5f, -0.5f  }, {  0.0f, -1.0f,  0.0f }, { 0, 0 }},
										{{ -0.5f,  0.5f, -0.5f  }, {  0.0f,  1.0f,  0.0f }, { 0, 0 }},
										{{ 0.5f,  0.5f, -0.5f   }, { 0.0f,  1.0f,  0.0f  }, { 0, 0 }},
										{{ 0.5f,  0.5f,  0.5f   }, { 0.0f,  1.0f,  0.0f  }, { 0, 0 }},
										{{ 0.5f,  0.5f,  0.5f   }, { 0.0f,  1.0f,  0.0f  }, { 0, 0 }},
										{{ -0.5f,  0.5f,  0.5f  }, {  0.0f,  1.0f,  0.0f }, { 0, 0 }},
										{{ -0.5f,  0.5f, -0.5f  }, {  0.0f,  1.0f,  0.0  }, { 0, 0 }}};
		std::vector<unsigned int> indices = {
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
		};
	};
}