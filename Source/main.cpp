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
#define CHANNELS 32

#include "SDL.h"
#include "SDL_mixer.h"

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
	//Is also equivalent to the number of audio components we want to have
	Mix_AllocateChannels(32);

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
//		ImGui::StyleColorsDark();
		{
//			61, 153, 73
//			ImVec4 buttonColour = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
			ImVec4 buttonColour = ImVec4(61.0f/255, 153.0f/255, 73.0f/255, 1.00f);
			ImGuiStyle& style = ImGui::GetStyle();
			style.Colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
			style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
			style.Colors[ImGuiCol_ChildBg]               = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
			style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
			style.Colors[ImGuiCol_Border]                = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
			style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
			style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
			style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
			style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
			style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
			style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
			style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
			style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
			style.Colors[ImGuiCol_CheckMark]             = buttonColour;
			style.Colors[ImGuiCol_SliderGrab]            = buttonColour;
			style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
			style.Colors[ImGuiCol_Button]                = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
			style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
			style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
			style.Colors[ImGuiCol_Header]                = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
			style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
			style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
			style.Colors[ImGuiCol_Separator]             = style.Colors[ImGuiCol_Border];
			style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
			style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
			style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
			style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
			style.Colors[ImGuiCol_Tab]                   = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
			style.Colors[ImGuiCol_TabHovered]            = ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
			style.Colors[ImGuiCol_TabActive]             = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
			style.Colors[ImGuiCol_TabUnfocused]          = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
			style.Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
			style.Colors[ImGuiCol_DockingPreview]        = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
			style.Colors[ImGuiCol_DockingEmptyBg]        = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
			style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
			style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
			style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
			style.Colors[ImGuiCol_DragDropTarget]        = buttonColour;
			style.Colors[ImGuiCol_NavHighlight]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
			style.Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
			style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.30f, 0.30f, 0.30f, 0.5f);
//			style.GrabRounding                           = style.FrameRounding = 2.3f;

			style.WindowRounding    = 10.0f;
			style.ChildRounding     = 5.0f;
			style.FrameRounding     = 5.0f;
			style.GrabRounding      = 10.0f;
			style.PopupRounding     = 10.0f;
			style.ScrollbarRounding = 10.0f;
			style.TabRounding       = 10.0f;
		}

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

	// Close Audio
	Mix_Quit();
	SDL_Quit();

	return EXIT_SUCCESS;
}