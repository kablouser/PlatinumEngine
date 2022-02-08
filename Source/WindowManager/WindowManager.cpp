//
// Created by shawn on 2022/2/7.
//

#include "imgui.h"
#include "stdio.h"
#include <imgui-SFML.h>
#include <D:\GithubDesktop\PlatinumEngine\cmake-build-debug\_deps\imgui-src\imgui_demo.cpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include "WindowManager/WindowManager.h"

namespace PlatinumEngine
{
	///--------------------------------------------------------------------------
	/// this function will create a basic window when you open the Platinum Engine
	///---------------------------------------------------------------------------
	void WindowManager::basicScene()
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
					showMenuFile();
					ImGui::EndMenu();
				}

				///---------------------------------------------------------------
				/// GameObject component
				///---------------------------------------------------------------
				if(ImGui::BeginMenu("GameObject"))
				{
					showMenuGameObject();
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

			window.clear();
			ImGui::SFML::Render(window);
			window.display();
		}
		ImGui::SFML::Shutdown();
	}

	///--------------------------------------------------------------------------
	/// this function helps to create a list of
	/// operations of "File" in the menu Bar
	///---------------------------------------------------------------------------
	void WindowManager::showMenuFile()
	{
		if(ImGui::MenuItem("New Scene"))
		{
			//TODO:
		}

		if(ImGui::MenuItem("Open Scene"))
		{
			//TODO:
		}

		if(ImGui::MenuItem("Save"))
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
	///---------------------------------------------------------------------------
	void WindowManager::showMenuGameObject()
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

		if(ImGui::MenuItem("Camera"))
		{
			//TODO:
		}
	}
}