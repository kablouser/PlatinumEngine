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
#include <ImGuiFileDialog.h>
namespace PlatinumEngine
{
	///--------------------------------------------------------------------------
	/// this function will create a basic window when you open the Platinum Engine
	///--------------------------------------------------------------------------
	void WindowManager::ShowBasicScene()
	{

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

			///-------------------------------------------------------------------
			/// set up the main menu bar
			///-------------------------------------------------------------------
            SetUpMainMenu();
			//ImGui::ShowDemoWindow();
			window.clear();
			ImGui::SFML::Render(window);
			window.display();
		}
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
	/// Set up the main menu for basic Window
	///--------------------------------------------------------------------------
	void WindowManager::SetUpMainMenu()
	{
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
				if(ImGui::MenuItem("Game", "Ctrl+1", &show_mainMenu_window_game)){}
				if(ImGui::MenuItem("Hierarchy","Ctrl+2", &show_mainMenu_window_hierarchy)){}
				if(ImGui::MenuItem("Inspector","Ctrl+3", &show_mainMenu_window_inspector)){}
				if(ImGui::MenuItem("Project","Ctrl+4",&show_mainMenu_window_project)){}
				if(ImGui::MenuItem("Scene", "Ctrl+5", &show_mainMenu_window_scene)){}
				if(ImGui::MenuItem("Animation", "Ctrl+6", &show_mainMenu_window_animation)){}
				if(ImGui::MenuItem("Audio","Ctrl+7", &show_mainMenu_window_audio)){}
				if(ImGui::MenuItem("Lighting","Ctrl+8", &show_mainMenu_window_light)){}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
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

		if(!ImGui::Begin("Animation", p_open))
		{
			ImGui::Text("ABOUT THIS DEMO:");
			ImGui::End();
			return;
		}
		ImGui::End();
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