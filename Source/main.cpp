#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <implot.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <InputManager/InputManager.h>
#include <Renderer/Renderer.h>
#include <WindowManager/WindowManager.h>

#include <AssetDatabase/AssetHelper.h>

#include <OpenGL/GLCheck.h>

#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "SDL_mixer.h"
#include "ComponentComposition/AudioComponent.h"

static void GlfwErrorCallback(int error, const char* description)
{
	std::cerr << "Glfw Error " << error << ": " << description << std::endl;
}

int main(int, char**)
{
	if(SDL_Init(SDL_INIT_AUDIO)<0) printf("SDL could not initialize! SDL Error: %s\n",SDL_GetError());
	if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1024)<0) printf("Failed to open audio! Mix Error: %s\n",Mix_GetError());

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
		ImPlot::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		static const ImWchar ke_icons_ranges[] = { ICON_MIN_KI, ICON_MAX_KI, 0 };
		static const ImWchar aw_icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0};
		ImFontConfig config;
		config.MergeMode = true;
		io.Fonts->AddFontFromFileTTF("./Fonts/NotoSansDisplay-Regular.ttf", 18.0f);
		io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAS, 16.0f, &config, aw_icons_ranges);
		io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_KI, 16.0f, &config, ke_icons_ranges);             // Merge into first font
		io.Fonts->Build();

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
		PlatinumEngine::Profiler profiler;
		PlatinumEngine::AssetDatabase assetDatabase;
		PlatinumEngine::AssetHelper assetHelper(&assetDatabase);
		PlatinumEngine::Logger logger;
		PlatinumEngine::InputManager inputManager;
		PlatinumEngine::Renderer rasterRenderer;
		PlatinumEngine::Scene scene;
		PlatinumEngine::SceneEditor sceneEditor(&inputManager, &scene, &rasterRenderer);
		PlatinumEngine::HierarchyWindow hierarchyWindow;
		PlatinumEngine::InspectorWindow inspectorWindow(&assetHelper);
		PlatinumEngine::GameWindow gameWindow(&inputManager, &scene, &rasterRenderer);
		PlatinumEngine::ProjectWindow projectWindow;
		PlatinumEngine::WindowManager windowManager(&gameWindow, &sceneEditor, &hierarchyWindow, &logger, &inspectorWindow, &profiler, &projectWindow);

		// Main loop
		while (!glfwWindowShouldClose(window))
		{
			{
				PlatinumEngine::Profiler::Frame frame;

				{
					PlatinumEngine::Profiler::Section pollEventsSection("Poll Events");
					glfwPollEvents();
				}

				{
					PlatinumEngine::Profiler::Section imguiSetupSection("ImGui Frame Setup");
					// Start the Dear ImGui frame
					ImGui_ImplOpenGL3_NewFrame();
					ImGui_ImplGlfw_NewFrame();
					ImGui::NewFrame();
				}

				//--------------------------------------------------------------------------------------------------------------
				// GUI HERE
				//--------------------------------------------------------------------------------------------------------------
				{
					PlatinumEngine::Profiler::Section dockingSection("Docking");
					ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
				}
				
				{
					PlatinumEngine::Profiler::Section windowManagerSection("Window Manager");
					windowManager.ShowGUI(scene);
				}

				//--------------------------------------------------------------------------------------------------------------
				// END OF GUI
				//--------------------------------------------------------------------------------------------------------------

				// Rendering
				{
					PlatinumEngine::Profiler::Section imguiRenderingSection("ImGui Rendering");
					ImGui::Render();

					int display_w, display_h;
					glfwGetFramebufferSize(window, &display_w, &display_h);
					GL_CHECK(glViewport(0, 0, display_w, display_h));
					ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
					GL_CHECK(glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
							clear_color.z * clear_color.w, clear_color.w));
					GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));

					ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
				}
			}
			// don't include swap buffers function in the frame for profiling
			// swap buffers waits for vsync
	
			glfwSwapBuffers(window);
		}



		// Cleanup ImGui
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		ImPlot::DestroyContext();
	}

	// Cleanup glfw
	glfwDestroyWindow(window);
	glfwTerminate();

	Mix_Quit();
	SDL_Quit();

	return EXIT_SUCCESS;
}