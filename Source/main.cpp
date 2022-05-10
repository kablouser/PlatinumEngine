#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <implot.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <InputManager/InputManager.h>
#include <Renderer/Renderer.h>
#include <WindowManager/WindowManager.h>
#include <TypeDatabase/TypeDatabase.h>

#include <AssetDatabase/AssetHelper.h>

#include <OpenGL/GLCheck.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_mixer.h>

void GlfwErrorCallback(int error, const char* description)
{
	PLATINUM_ERROR_STREAM << "Glfw Error " << error << ": " << description;
}

int main(int, char**)
{
	PLATINUM_INFO("Platinum Engine Started.");

	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		PLATINUM_ERROR_STREAM << "SDL could not initialize! SDL Error: " << SDL_GetError();
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
	{
		PLATINUM_ERROR_STREAM << "Failed to open audio! Mix Error: " << Mix_GetError();
	}

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
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Platinum Engine", NULL, NULL);
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
		ImGuiIO& io = ImGui::GetIO();

		const ImWchar aw_icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

		ImFontConfig config;
		config.MergeMode = true;
		io.Fonts->AddFontFromFileTTF("./Fonts/NotoSansDisplay-Regular.ttf", 18.0f);
		io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAS, 16.0f, &config, aw_icons_ranges);
		io.Fonts->Build();

		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		//imguizmo
		float viewManipulateRight = io.DisplaySize.x;
		float viewManipulateTop = 0;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		PlatinumEngine::Application application;

		// create assets
		application.assetDatabase.Update(application.idSystem, application.scene);

		// load default scene
		application.scene.LoadFile("Assets/Default.scene");

		// Main loop
		while (!glfwWindowShouldClose(window))
		{
			application.time.Update();
			{
				PlatinumEngine::Profiler::Frame frame;

				{
					PlatinumEngine::Profiler::Section pollEventsSection("Poll Events");
					glfwPollEvents();

					if (glfwWindowShouldClose(window))
					{
						// ask for confirmation
						application.windowManager.SuggestQuit();
					}
				}

				{
					PlatinumEngine::Profiler::Section imguiSetupSection("ImGui Frame Setup");
					// Start the Dear ImGui frame
					ImGui_ImplOpenGL3_NewFrame();
					ImGui_ImplGlfw_NewFrame();
					ImGui::NewFrame();
					ImGuizmo::BeginFrame();
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
					application.windowManager.ShowGUI();
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

		PLATINUM_INFO("Platinum Engine Ended.");

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