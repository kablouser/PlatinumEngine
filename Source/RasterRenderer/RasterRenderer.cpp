#include <RasterRenderer/RasterRenderer.h>
// checking errors
#include <OpenGL/GLCheck.h>
// gui
#include <imgui.h>
// OpenGL extensions
#include <GL/glew.h>
// printing errors
#include <iostream>

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
	RasterRenderer::RasterRenderer(bool printOpenGLInfo) :
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
			std::cout <<
					  "OpenGL context info" << std::endl <<
					  "Vendor: " << glGetString(GL_VENDOR) << std::endl <<
					  "Renderer: " << glGetString(GL_RENDERER) << std::endl <<
					  "OpenGL version: " << glGetString(GL_VERSION) << std::endl <<
					  "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
			// TODO
			// check if version meets minimum requirements here
			// need to parse string
		}

		// glew imports all OpenGL extension methods
		GLenum errorCode = glewInit();
		if (errorCode != GLEW_OK)
		{
			std::cerr << "GLEW initialisation error. " << glewGetErrorString(errorCode) << std::endl;
			// without GLEW, seg faults will happen
			return;
		}

		if (!_shaderProgram.Compile(UNLIT_VERTEX_SHADER, UNLIT_FRAGMENT_SHADER))
			return;

		// in variables
		_unlitShaderInput.Set({
						{{ -0.5f, -0.5f, 0.0f }, { 0, 0, 1 }, { 0, 0 }},
						{{ 0.5f,  -0.5f, 0.0f }, { 0, 0, 1 }, { 0, 0 }},
						{{ 0.0f,  0.5f,  0.0f }, { 0, 0, 1 }, { 0, 0 }}
				},
				{
						0, 1, 2
				});

		_framebufferWidth = 1;
		_framebufferHeight = 1;
		if (!_framebuffer.Create(_framebufferWidth, _framebufferHeight))
			return;

		// check for uncaught errors
		GL_CHECK();

		_isInitGood = true;
	}

	RasterRenderer::~RasterRenderer()
	{
		// nothing to do
	}

	void RasterRenderer::ShowGUIWindow(bool* outIsOpen)
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
}