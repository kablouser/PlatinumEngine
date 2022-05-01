#pragma once

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// PlatinumEngine lib
#include <ComponentComposition/GameObject.h>
#include <OpenGL/ShaderInput.h>
#include <OpenGL/ShaderProgram.h>
#include <OpenGL/Framebuffer.h>
#include <GLFW/glfw3.h>
#include <OpenGL/Material.h>
#include <OpenGL/Mesh.h>
namespace PlatinumEngine
{
	class Renderer
	{

	public:
		// Constructors
		Renderer(bool printOpenGLInfo = true);

		~Renderer();

		// set framebuffer from SceneEditor
		void SetFramebuffer(Framebuffer *framebuffer);

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

		void SetCameraPos(const Maths::Vec3 &pos);

		// a window for renderer to test

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

		/**
		 * light component can call this function to input light data into shader
		 * @param light
		 */
		// void LoadLight(const Light &light);
		void SetupLights(std::vector<GameObject*> &lights);

		/**
		 * draw light
		 * @param model matrix
		 */
		void DrawLight(Maths::Mat4 matrix = Maths::Mat4(1.0f));
	private:

		// true iff all init steps were successful
		bool _isInitGood;

		ShaderProgram _unlitShader;
		ShaderProgram _skyBoxShader;
		ShaderProgram _gridShader;
		ShaderProgram _phongShader;
		ShaderProgram _reflectRefractShader;
		ShaderProgram _lightShader;

		// ShaderInput _meshShaderInput, _lightShaderInput;

		Framebuffer _framebuffer;
		int _framebufferWidth;
		int _framebufferHeight;
	};
}