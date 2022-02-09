//
// Created by shawn on 2022/2/7.
//

#include "imgui.h"
#include "stdio.h"
#include <imgui-SFML.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include "WindowManager/WindowManager.h"
//#include "D:\GithubDesktop\PlatinumEngine\cmake-build-debug\_deps\imgui-src\imgui_demo.cpp"
namespace PlatinumEngine
{
	///--------------------------------------------------------------------------
	/// this function will create a basic window when you open the Platinum Engine
	///--------------------------------------------------------------------------
	void WindowManager::ShowBasicScene()
	{

		///-----------------------------------------------------------------------
		///bools in main menu window list
		///-----------------------------------------------------------------------
		static bool show_mainMenu_window_game                 = false;
		static bool show_mainMenu_window_scene                = false;
		static bool show_mainMenu_window_inspector            = false;
		static bool show_mainMenu_window_hierarchy            = false;
		static bool show_mainMenu_window_project              = false;
		static bool show_mainMenu_window_animation            = false;
		static bool show_mainMenu_window_audio                = false;
		static bool show_mainMenu_window_light                = false;

		///-----------------------------------------------------------------------
		///bools in main menu GameObject list
		///-----------------------------------------------------------------------
		static bool show_mainMenu_gameObject_empty            = false;
		static bool show_mainMenu_gameObject_emptyChild       = false;
		static bool show_mainMenu_gameObject_emptyParent      = false;
		static bool show_mainMenu_gameObject_object_cube      = false;
		static bool show_mainMenu_gameObject_object_sphere    = false;
		static bool show_mainMenu_gameObject_object_plane     = false;
		static bool show_mainMenu_gameObject_object_capsule   = false;
		static bool show_mainMenu_gameObject_effect_particle  = false;
		static bool show_mainMenu_gameObject_camera           = false;
		static bool show_mainMenu_gameObject_light            = false;

		///-----------------------------------------------------------------------
		///bools in main menu file list
		///-----------------------------------------------------------------------
		static bool show_mainMenu_file_newScene               = false;
		static bool show_mainMenu_file_openScene              = false;
		static bool show_mainMenu_file_save                   = false;
		static bool show_mainMenu_file_saveAs                 = false;

		///-----------------------------------------------------------------------
		///ifs in main menu window list to call the function inside
		///-----------------------------------------------------------------------
		if(show_mainMenu_window_game)                ShowWindowGame(&show_mainMenu_window_game);
		if(show_mainMenu_window_scene)               ShowWindowScene(&show_mainMenu_window_scene);
		if(show_mainMenu_window_hierarchy)           ShowWindowHierarchy(&show_mainMenu_window_hierarchy);
		if(show_mainMenu_window_inspector)           ShowWindowInspector(&show_mainMenu_window_inspector);
		if(show_mainMenu_window_project)             ShowWindowProject(&show_mainMenu_window_project);
		if(show_mainMenu_window_animation)           ShowWindowAnimation(&show_mainMenu_window_animation);
		if(show_mainMenu_window_audio)               ShowWindowAudio(&show_mainMenu_window_audio);
		if(show_mainMenu_window_light)               ShowWindowLight(&show_mainMenu_window_light);


		sf::RenderWindow window(sf::VideoMode(1080, 960), "Platinum Engine");
		window.setFramerateLimit(60);
		ImGui::SFML::Init(window);

		sf::Clock deltaClock;
		while (window.isOpen())
		{

			sf::Event event;
			while (window.pollEvent(event))
			{
				ImGui::SFML::ProcessEvent(window, event);

				if (event.type == sf::Event::Closed)
				{
					window.close();
				}
			}
			ImGui::SFML::Update(window, deltaClock.restart());

			///-------------------------------------------------------------------
			/// set up the main menu bar
			///-------------------------------------------------------------------
			if(ImGui::BeginMainMenuBar())
			{
				///---------------------------------------------------------------
				/// File component
				///---------------------------------------------------------------
				if(ImGui::BeginMenu("File"))
				{
					ShowMenuFile();
					ImGui::EndMenu();
				}

				///---------------------------------------------------------------
				/// GameObject component
				///---------------------------------------------------------------
				if(ImGui::BeginMenu("GameObject"))
				{
					ShowMenuGameObject();
					ImGui::EndMenu();
				}

				///---------------------------------------------------------------
				/// Window component
				///---------------------------------------------------------------
                if(ImGui::BeginMenu("Window"))
                {
					if(ImGui::MenuItem("Game", "Ctrl+1")){/*TODO:*/}
					if(ImGui::MenuItem("Hierarchy","Ctrl+2")){/*TODO:*/}
					if(ImGui::MenuItem("Inspector","Ctrl+3")){/*TODO:*/}
					if(ImGui::MenuItem("Project","Ctrl+4")){/*TODO:*/}
					if(ImGui::MenuItem("Scene", "Ctrl+5")){/*TODO:*/}
					if(ImGui::MenuItem("Animation", "Ctrl+6")){/*TODO:*/}
					if(ImGui::MenuItem("Audio","Ctrl+7")){/*TODO:*/}
					if(ImGui::MenuItem("Lighting","Ctrl+8")){/*TODO:*/}
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}

			ImGui::ShowDemoWindow();
			window.clear();
			ImGui::SFML::Render(window);
			window.display();
		}
		ImGui::ShowDemoWindow();
		ImGui::SFML::Shutdown();
	}

	///--------------------------------------------------------------------------
	/// this function helps to create a list of
	/// operations of "File" in the menu Bar
	///--------------------------------------------------------------------------
	void WindowManager::ShowMenuFile()
	{
		if(ImGui::MenuItem("New Scene"))
		{
			//TODO:
		}

		if(ImGui::MenuItem("Open Scene"))
		{
			//TODO:
		}

		if(ImGui::MenuItem("Save", "Ctrl+S"))
		{
			//TODO:
		}

		if(ImGui::MenuItem("Save as"))
		{
			//TODO:
		}
	}

	///--------------------------------------------------------------------------
	/// this function helps to create a list of
	/// operations of "GameObject" in the menu Bar
	///--------------------------------------------------------------------------
	void WindowManager::ShowMenuGameObject()
	{
		if(ImGui::MenuItem("Create Empty"))
		{
			//TODO:
		}

		if(ImGui::MenuItem("Create Empty Child"))
		{
			//TODO:
		}

		if(ImGui::MenuItem("Create Empty Parent"))
		{
			//TODO:
		}

		if(ImGui::BeginMenu("3D Object"))
		{
			if(ImGui::MenuItem("Cube")){/*TODO:*/}
			if(ImGui::MenuItem("Sphere")){/*TODO:*/}
			if(ImGui::MenuItem("Capsule")){/*TODO:*/}
			if(ImGui::MenuItem("Plane")){/*TODO:*/}
			ImGui::EndMenu();
		}

		if(ImGui::BeginMenu("Effect"))//maybe we will have a particle system
		{
			if(ImGui::MenuItem("Particle System")){/*TODO:*/}
			ImGui::EndMenu();
		}

		if(ImGui::MenuItem("Camera"))
		{
			//TODO:
		}

		if(ImGui::MenuItem("Light"))
		{
			//TODO:
		}
	}


	///--------------------------------------------------------------------------
	///   ---                                                               ---
	///   | Section: Please implement GUI in the corresponding function below |
	///   ---                                                               ---
	///--------------------------------------------------------------------------

	//Please implement Animation Window below
	void WindowManager::ShowWindowAnimation(bool* p_open)
	{
		//TODO:
	}

	//Please implement Project Window below
	void WindowManager::ShowWindowProject(bool* p_open)
	{
		//TODO:
	}

	//Please implement Light Window below
	void WindowManager::ShowWindowLight(bool* p_open)
	{
		//TODO:
	}
	//Please implement Game Window below
	void WindowManager::ShowWindowGame(bool* p_open)
	{
		//TODO:
	}
	//Please implement Scene Window below
	void WindowManager::ShowWindowScene(bool* p_open)
	{
		//TODO:
	}
	//Please implement Inspector Window below
	void WindowManager::ShowWindowInspector(bool* p_open)
	{
		//TODO:
	}
	//Please implement Audio Window below
	void WindowManager::ShowWindowAudio(bool* p_open)
	{
		//TODO:
	}
	//Please implement Hierarchy Window below
	void WindowManager::ShowWindowHierarchy(bool* p_open)
	{
		//TODO:
	}

}