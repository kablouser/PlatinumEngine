//
// Created by Jason on 07/02/2022.
//

#include "imgui.h"
#include "stdio.h"
#include "iostream"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"
#include "SFML/Window/Joystick.hpp"
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
		a.name="Horizontal";
		a.positiveKey = sf::Keyboard::D;
		a.negativeKey = sf::Keyboard::A;
		a.type = TYPE_KEYBOARD_MOUSE_BUTTON;
		Axes.push_back(a);
		a.name="Vertical";
		a.positiveKey = sf::Keyboard::W;
		a.negativeKey = sf::Keyboard::S;
		a.type = TYPE_KEYBOARD_MOUSE_BUTTON;
		Axes.push_back(a);
		a.name="Submit";
		a.positiveKey = sf::Keyboard::Enter;
		a.negativeKey = sf::Keyboard::Unknown;
		a.type = TYPE_KEYBOARD_MOUSE_BUTTON;
		Axes.push_back(a);

		sf::RenderWindow window(sf::VideoMode(1080, 960), "Platinum Engine");
		window.setFramerateLimit(60);
		ImGui::SFML::Init(window);

		sf::Clock deltaClock;

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

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
			float jx = sf::Joystick::getAxisPosition(0,sf::Joystick::Axis::X);
			float jy = sf::Joystick::getAxisPosition(0,sf::Joystick::Axis::Y);
			float jz = sf::Joystick::getAxisPosition(0,sf::Joystick::Axis::Z);
			float jr = sf::Joystick::getAxisPosition(0,sf::Joystick::Axis::R);
			float ju = sf::Joystick::getAxisPosition(0,sf::Joystick::Axis::U);
			float jv = sf::Joystick::getAxisPosition(0,sf::Joystick::Axis::V);
			float jpx = sf::Joystick::getAxisPosition(0,sf::Joystick::Axis::PovX);
			float jpy = sf::Joystick::getAxisPosition(0,sf::Joystick::Axis::PovY);
			ImGui::Text("JoyStick Axis : x=%f, y=%f, z=%f, r=%f, u=%f, v=%f, px=%f, py=%f", jx,jy,jz,jr,ju,jv,jpx,jpy);

			ImVec2 mousePos = GetMousePosition();
			sf::Joystick::Identification id = sf::Joystick::getIdentification(0);
			ImGui::Text("JoyStick Connected : %d", sf::Joystick::isConnected(0));
			ImGui::Text("JoyStick: %d, %d",id.productId, id.vendorId);
			ImGui::Text("JoyStick: %s",id.name.toAnsiString().c_str());
			ImGui::Text("Position: %f, %f", mousePos.x, mousePos.y);
			ImGui::Text("Left Mouse Down: %d", IsMouseDown(0));
			ImGui::Text("Up Arrow Key Down: %d", ImGui::IsKeyDown(sf::Keyboard::Up));
			ImGui::Text("Key Down (ID): %d", kd);
			ImGui::Text("Axis: %f", GetAxis("Horizontal"));

			sf::Vector2i winpos = window.getPosition();
			winpos.x += GetAxis("Horizontal") + jx;
			winpos.y -= GetAxis("Vertical") - jy;
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
	float InputManager::GetAxis(std::string AxisName, int joyid)
	{
		for(Axis axis: Axes)
		{
			if(axis.name.compare(AxisName) == 0)
			{
				if(axis.type == TYPE_KEYBOARD_MOUSE_BUTTON)
				{
					if (IsKeyDown(axis.positiveKey)) return 1.f;
					else if (IsKeyDown(axis.negativeKey)) return -1.f;
				}
				else if(axis.type == TYPE_JOYSTICK_AXIS)
				{
					if(axis.joyID == joyid)
					{
						return sf::Joystick::getAxisPosition(joyid,axis.axis);
					}
				}
			}
		}
		return 0;
		//NEED MORE REFINEMENT AND IMPROVEMENT
	}
}