//
// Created by Jason on 07/02/2022.
//

#include "imgui.h"
#include "stdio.h"
#include "iostream"
#include <imgui-SFML.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include "InputManager/InputManager.h"

namespace PlatinumEngine
{
	///--------------------------------------------------------------------------
	/// Test Scene for Inputs (WILL NEED TO REMOVE THIS LATER)
	///--------------------------------------------------------------------------
	void InputManager::InputScene()
	{
		//May need a better way to properly allow user defined axes???
		Axis a;
		a.Name="Horizontal";
		a.positiveKey = KeyCode::D;
		a.negativeKey = KeyCode::A;
		Axes.push_back(a);

		sf::RenderWindow window(sf::VideoMode(1080, 960), "Platinum Engine");
		window.setFramerateLimit(60);
		ImGui::SFML::Init(window);

		sf::Clock deltaClock;

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

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

			//TESTING HERE
			int kd = -1;
			for(int k=0;k<512;k++) if(ImGui::IsKeyDown(k)) kd=k;
			ImVec2 mousePos = GetMousePosition();
			ImGui::Text("Position: %f, %f", mousePos.x, mousePos.y);
			ImGui::Text("Left Mouse Down: %d", IsMouseDown(0));
			ImGui::Text("Up Arrow Key Down: %d", ImGui::IsKeyDown(KeyCode::UpArrow));
			ImGui::Text("Key Down (ID): %d", kd);
			ImGui::Text("Axis: %f", GetAxis("Horizontal"));

			sf::Vector2i winpos = window.getPosition();
			winpos.x += GetAxis("Horizontal");
			window.setPosition(winpos);

			//ImGui::ShowDemoWindow();
			window.clear();
			ImGui::SFML::Render(window);
			window.display();
		}
		ImGui::SFML::Shutdown();
	}

	//Checks for Mouse Click Action
	bool InputManager::IsMouseClicked(int button)
	{
		switch (button)
		{
			case 0: return ImGui::IsMouseClicked(ImGuiMouseButton_Left);
			case 1: return ImGui::IsMouseClicked(ImGuiMouseButton_Right);
			case 2: return ImGui::IsMouseClicked(ImGuiMouseButton_Middle);
		}
		return false;
	}

	//Checks for Mouse Hold Action
	bool InputManager::IsMouseDown(int button)
	{
		switch (button)
		{
			case 0: return ImGui::IsMouseDown(ImGuiMouseButton_Left);
			case 1: return ImGui::IsMouseDown(ImGuiMouseButton_Right);
			case 2: return ImGui::IsMouseDown(ImGuiMouseButton_Middle);
		}
		return false;
	}

	//Checks for Mouse Release Action
	bool InputManager::IsMouseReleased(int button)
	{
		switch (button)
		{
			case 0: return ImGui::IsMouseReleased(ImGuiMouseButton_Left);
			case 1: return ImGui::IsMouseReleased(ImGuiMouseButton_Right);
			case 2: return ImGui::IsMouseReleased(ImGuiMouseButton_Middle);
		}
		return false;
	}

	//Returns (Relative) Mouse Position
	ImVec2 InputManager::GetMousePosition()
	{
		ImVec2 mousePositionAbsolute = ImGui::GetMousePos();
		ImVec2 screenPositionAbsolute = ImGui::GetItemRectMin();
		ImVec2 mousePositionRelative = ImVec2(mousePositionAbsolute.x - screenPositionAbsolute.x, mousePositionAbsolute.y - screenPositionAbsolute.y);
		return mousePositionRelative;
		//Relative to the current screen (might need to change this)
	}

	//Checks for Key Press Action
	bool InputManager::IsKeyPressed(int key)
	{
		return ImGui::IsKeyPressed(key);
	}

	//Checks for Key Hold Action
	bool InputManager::IsKeyDown(int key)
	{
		return ImGui::IsKeyDown(key);
	}

	//Checks for Key Release Action
	bool InputManager::IsKeyReleased(int key)
	{
		return ImGui::IsKeyReleased(key);
	}

	//Returns value of the Axis defined by AxisName
	float InputManager::GetAxis(std::string AxisName)
	{
		for(Axis axis: Axes)
		{
			if(axis.Name.compare(AxisName) == 0)
			{
				if(IsKeyDown(axis.positiveKey)) axis.SetValue(1.f);
				else if(IsKeyDown(axis.negativeKey)) axis.SetValue(-1.f);
				return axis.GetValue();
			}
		}
		return 0;
		//NEED MORE REFINEMENT AND IMPROVEMENT
	}
}