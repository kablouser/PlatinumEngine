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



	InputManager::InputManager()
	{
		///--------------------------------------------------------------------------
		/// Test Axes for Inputs (WILL NEED TO REMOVE THIS LATER)
		///--------------------------------------------------------------------------
		//May need a better way to properly allow user defined axes???
		Axis axis;
		axis.name="Horizontal";
		axis.positiveKey = sf::Keyboard::D;
		axis.negativeKey = sf::Keyboard::A;
		axis.type = AxisType::keyboardMouseButton;
		_axes.push_back(axis);
		axis.name="Vertical";
		axis.positiveKey = sf::Keyboard::W;
		axis.negativeKey = sf::Keyboard::S;
		axis.type = AxisType::keyboardMouseButton;
		_axes.push_back(axis);
		axis.name="Submit";
		axis.positiveKey = sf::Keyboard::Enter;
		axis.negativeKey = sf::Keyboard::Unknown;
		axis.type = AxisType::keyboardMouseButton;
		_axes.push_back(axis);


	}

	void InputManager::ShowGUIWindow(bool* outIsOpen)
	{
		///--------------------------------------------------------------------------
		/// Test Window for Inputs (WILL NEED TO REMOVE THIS LATER)
		///--------------------------------------------------------------------------
		if(ImGui::Begin("Input Test", outIsOpen))
		{
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

			//TESTING HERE
			int keyDown = -1;
			for (int key = 0; key < 512; key++) if (ImGui::IsKeyDown(key)) keyDown = key;
			float joystickX = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X);
			float joystickY = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y);
			float joystickZ = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Z);
			float joystickR = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::R);
			float joystickU = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::U);
			float joystickV = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::V);
			float joystickPovX = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovX);
			float joystickPovY = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY);
			ImGui::Text("JoyStick Axis : x=%f, y=%f, z=%f, r=%f, u=%f, v=%f, povX=%f, povY=%f",
					joystickX, joystickY, joystickZ, joystickR, joystickU, joystickV, joystickPovX, joystickPovY);

			ImVec2 mousePos = GetMousePosition();
			sf::Joystick::Identification id = sf::Joystick::getIdentification(0);
			ImGui::Text("JoyStick Connected : %d", sf::Joystick::isConnected(0));
			ImGui::Text("JoyStick: %d, %d", id.productId, id.vendorId);
			ImGui::Text("JoyStick: %s", id.name.toAnsiString().c_str());
			ImGui::Text("Position: %f, %f", mousePos.x, mousePos.y);
			ImGui::Text("Left Mouse Down: %d", IsMouseDown(0));
			ImGui::Text("Up Arrow Key Down: %d", ImGui::IsKeyDown(sf::Keyboard::Up));
			ImGui::Text("Key Down (ID): %d", keyDown);
			ImGui::Text("Axis: %f", GetAxis("Horizontal"));

			sf::Vector2i windowPosition = ImGui::GetWindowPos();
			windowPosition.x += GetAxis("Horizontal") + joystickX;
			windowPosition.y -= GetAxis("Vertical") - joystickY;
			ImGui::SetWindowPos(windowPosition);
		}
		ImGui::End();
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
	float InputManager::GetAxis(std::string axisName, int joystickID)
	{
		for (Axis axis: _axes)
		{
			if (axis.name == axisName)
			{
				switch (axis.type)
				{
				case AxisType::keyboardMouseButton:
					if (IsKeyDown(axis.positiveKey)) return 1.f;
					else if (IsKeyDown(axis.negativeKey)) return -1.f;
					break;

				case AxisType::joystickAxis:
					if (axis.joystickID == joystickID)
					{
						return sf::Joystick::getAxisPosition(joystickID, axis.joystickAxis);
					}
					break;

				default:
					// produce warning/error here
					return 0.0f;
				}
			}
		}

		// produce warning/error here
		return 0.0f;
		//NEED MORE REFINEMENT AND IMPROVEMENT
	}
}