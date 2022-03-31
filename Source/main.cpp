#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <InputManager/InputManager.h>
#include <Inspector/InspectorWindow.h>
#include <Renderer/Renderer.h>
#include <WindowManager/WindowManager.h>
#include <SceneManager/HierarchyWindow.h>
#include <Logger/Logger.h>
#include <SceneEditor/SceneEditor.h>
#include <GameWindow/GameWindow.h>


#include <OpenGL/GLCheck.h>
static void GlfwErrorCallback(int error, const char* description)
{
	std::cerr << "Glfw Error " << error << ": " << description << std::endl;
}


int main(int, char**)
{
	// Setup window
	glfwSetErrorCallback(GlfwErrorCallback);
	if (!glfwInit())
		return EXIT_FAILURE;

	// OpenGL+GLSL versions
    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac

	// Create window with graphics context
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
	if (window == NULL)
		return EXIT_FAILURE;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync


	{
		// this scope has a valid OpenGL context initialised

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		// construct logger before everything to save all logs

		PlatinumEngine::Logger logger;
		PlatinumEngine::Renderer rasterRenderer;
		PlatinumEngine::InputManager inputManager;
		PlatinumEngine::Scene scene;
		PlatinumEngine::SceneEditor sceneEditor(&inputManager, &scene, &rasterRenderer);
		PlatinumEngine::HierarchyWindow hierarchyWindow;
		PlatinumEngine::InspectorWindow inspectorWindow;
		PlatinumEngine::GameWindow gameWindow(&inputManager, &scene, &rasterRenderer);
		PlatinumEngine::WindowManager windowManager(&gameWindow, &sceneEditor, &hierarchyWindow, &logger, &inspectorWindow);

		// Main loop
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			//--------------------------------------------------------------------------------------------------------------
			// GUI HERE
			//--------------------------------------------------------------------------------------------------------------
			ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
			windowManager.ShowGUI(scene);
			//ImGui::ShowDemoWindow();
			//--------------------------------------------------------------------------------------------------------------
			// END OF GUI
			//--------------------------------------------------------------------------------------------------------------

			// Rendering
			ImGui::Render();

			int display_w, display_h;
			glfwGetFramebufferSize(window, &display_w, &display_h);
			GL_CHECK(glViewport(0, 0, display_w, display_h));
			ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
			GL_CHECK(glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w));
			GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

	return EXIT_SUCCESS;
}