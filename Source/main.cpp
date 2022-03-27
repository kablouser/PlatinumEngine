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
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		// construct logger before everything to save all logs
		bool isLoggerOpen = true;
		PlatinumEngine::Logger logger;

		bool isRasterRendererOpen = true;
		PlatinumEngine::Renderer rasterRenderer;

		bool isInputWindowOpen = true;
		PlatinumEngine::InputManager inputManager;


		bool isHierarchyWindowOpen = true;
		PlatinumEngine::HierarchyWindow hierarchyWindow;

		bool isInspectorWindowOpen = true;
		PlatinumEngine::InspectorWindow inspectorWindow;

		// TEST CODE FOR INSPECTOR WINDOW
		auto* obj = new PlatinumEngine::GameObject("Object");
		auto* meshComp = new PlatinumEngine::MeshComponent();
		auto* transformerComp = new PlatinumEngine::TransformComponent();
		obj->AddComponent(meshComp);
		obj->AddComponent(transformerComp);
		inspectorWindow.SetActiveGameObject(obj);
		// END TEST CODE

		PlatinumEngine::WindowManager windowManager;


		// Create scene
		PlatinumEngine::Scene scene;

		for(int i = 0; i<5; i++)
		{
			scene.AddGameObject("GameObject");

		}

		for(int i = 0; i< 5; i++)
		{

			scene.AddGameObject("GameObject2",scene.GetRootGameObject(i));


		}


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
			if (isRasterRendererOpen)
				rasterRenderer.ShowGUIWindow(&isRasterRendererOpen);
			if(isInputWindowOpen)
				inputManager.ShowGUIWindow(&isInputWindowOpen);
			if(isHierarchyWindowOpen)
				hierarchyWindow.ShowGUIWindow(&isHierarchyWindowOpen, scene);
			if(isLoggerOpen)
				logger.ShowGUIWindow(&isLoggerOpen);
			if(isInspectorWindowOpen)
				inspectorWindow.ShowGUIWindow(&isInspectorWindowOpen);
			windowManager.ShowGUI();
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

		// TEST CODE FOR INSPECTOR WINDOW
		delete transformerComp;
		delete meshComp;
		delete obj;
		// END TEST CODE

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