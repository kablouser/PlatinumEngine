// dear imgui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//#include <InputManager/InputManager.h>
#include <RasterRenderer/RasterRenderer.h>
//#include <SceneManager/SceneManager.h>
//#include <WindowManager/WindowManager.h>

#include <OpenGL/GLCheck.h>

static void glfw_error_callback(int error, const char* description)
{
	std::cerr << "Glfw Error " << error << ": " << description << std::endl;
}

int main(int, char**)
{
	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	// OpenGL+GLSL versions
    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac

	// Create window with graphics context
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
	if (window == NULL)
		return 1;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	{
		// this scope has a valid OpenGL context initialised

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		PlatinumEngine::RasterRenderer rasterRenderer;
		bool isRasterRendererOpen = true;

		// Main loop
		while (!glfwWindowShouldClose(window))
		{
			// Poll and handle events (inputs, window resize, etc.)
			// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
			// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
			// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
			// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
			glfwPollEvents();

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			//--------------------------------------------------------------------------------------------------------------
			// GUI HERE
			//--------------------------------------------------------------------------------------------------------------
			if (isRasterRendererOpen)
				rasterRenderer.ShowGUIWindow(&isRasterRendererOpen);
			//--------------------------------------------------------------------------------------------------------------
			// END OF GUI
			//--------------------------------------------------------------------------------------------------------------

			// Rendering
			GL_CHECK();
			ImGui::Render();

			GL_CHECK();

			int display_w, display_h;
			glfwGetFramebufferSize(window, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);

			GL_CHECK();

			ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
			glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
			glClear(GL_COLOR_BUFFER_BIT);
			GL_CHECK();

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			GL_CHECK();

			glfwSwapBuffers(window);
		}

		// Cleanup ImGui
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	// Cleanup glfw
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

/*
#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <InputManager/InputManager.h>
#include <RasterRenderer/RasterRenderer.h>
#include <SceneManager/SceneManager.h>
#include <WindowManager/WindowManager.h>

int main()
{
	sf::RenderWindow window(sf::VideoMode(1080, 960), "Platinum Engine");
	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window);

	PlatinumEngine::RasterRenderer rasterRenderer(window);
	bool isRasterRendererOpen = true;
	PlatinumEngine::InputManager inputManager;
	bool isInputWindowOpen = true;
	PlatinumEngine::WindowManager windowManager;

	sf::Clock deltaClock;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(window, event);
			windowManager.DoShortCut(event);

			if (event.type == sf::Event::Closed)
			{
				window.close();
				break;
			}
		}
		// if we don't break now, trying to use OpenGL might cause crashes
		if(!window.isOpen())
			break;
		ImGui::SFML::Update(window, deltaClock.restart());

		// Background color of main window
		window.clear(sf::Color(18, 33, 43));


		//--------------------------------------------------------------------------------------------------------------
		// GUI HERE
		//--------------------------------------------------------------------------------------------------------------
		if(isRasterRendererOpen)
			rasterRenderer.ShowGUIWindow(&isRasterRendererOpen);
		if(isInputWindowOpen)
			inputManager.ShowGUIWindow(&isInputWindowOpen);

		windowManager.ShowGUI();
		//--------------------------------------------------------------------------------------------------------------
		// END OF GUI
		//--------------------------------------------------------------------------------------------------------------


		ImGui::SFML::Render(window);
		window.display();
	}
	ImGui::SFML::Shutdown();


	return 0;
}*/