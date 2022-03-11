//
// Created by Jason on 07/02/2022.
//

#include <InputManager/InputManager.h>
#include <imgui.h>
#include <stdio.h>
#include <iostream>

#include <GLFW/glfw3.h>

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
		axis.positiveKey = GLFW_KEY_D;
		axis.negativeKey = GLFW_KEY_A;
		axis.type = AxisType::keyboardMouseButton;
		_axes.push_back(axis);
		axis.name="Vertical";
		axis.positiveKey = GLFW_KEY_W;
		axis.negativeKey = GLFW_KEY_S;
		axis.type = AxisType::keyboardMouseButton;
		_axes.push_back(axis);
		axis.name="Submit";
		axis.positiveKey = GLFW_KEY_ENTER;
		axis.negativeKey = GLFW_KEY_UNKNOWN;
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
			float leftX = GetGamepadAxis(0, GLFW_GAMEPAD_AXIS_LEFT_X);
			float leftY = GetGamepadAxis(0, GLFW_GAMEPAD_AXIS_LEFT_Y);
			float rightX = GetGamepadAxis(0, GLFW_GAMEPAD_AXIS_RIGHT_X);
			float rightY = GetGamepadAxis(0, GLFW_GAMEPAD_AXIS_RIGHT_Y);
			float leftTrigger = GetGamepadAxis(0, GLFW_GAMEPAD_AXIS_LEFT_TRIGGER);
			float rightTrigger = GetGamepadAxis(0, GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER);
			ImGui::Text("JoyStick Axis : left x=%f, left y=%f, right x=%f, right y=%f, left trigger=%f, right trigger=%f",
					leftX, leftY, rightX, rightY, leftTrigger, rightTrigger);

			ImVec2 mousePos = GetMousePosition();
			int isConnected = glfwJoystickPresent(0);
			ImGui::Text("JoyStick Connected : %d", isConnected);
			if(isConnected)
			{
				ImGui::Text("JoyStick Name: %s", glfwGetJoystickName(0));
			}
			ImGui::Text("Position: %f, %f", mousePos.x, mousePos.y);
			ImGui::Text("Left Mouse Down: %d", IsMouseDown(0));
			ImGui::Text("Up Arrow Key Down: %d", ImGui::IsKeyDown(GLFW_KEY_UP));
			ImGui::Text("Key Down (ID): %d", keyDown);
			ImGui::Text("Axis: %f", GetAxis("Horizontal"));

			ImVec2 windowPosition = ImGui::GetWindowPos();
			windowPosition.x += GetAxis("Horizontal");// + joystickX;
			windowPosition.y -= GetAxis("Vertical");// - joystickY;
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

	float InputManager::GetGamepadAxis(int gamepadID, int axis)
	{
		GLFWgamepadstate state;
		if (glfwGetGamepadState(gamepadID, &state))
			return state.axes[axis];

//		std::cerr <<
//			"InputManager::GetGamepadAxis has invalid input, gamepadID " <<
//			gamepadID << " is not a gamepad." << std::endl;
		return 0.0f;
	}

	bool InputManager::IsGamepadButtonDown(int gamepadID, int button)
	{
		GLFWgamepadstate state;
		if (glfwGetGamepadState(gamepadID, &state))
			return state.buttons[button] == GLFW_PRESS;

//		std::cerr <<
//				  "InputManager::GetGamepadAxis has invalid input, gamepadID " <<
//				  gamepadID << " is not a gamepad." << std::endl;
		return 0.0f;
	}

	//Returns value of the Axis defined by AxisName
	float InputManager::GetAxis(std::string axisName)
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
					return 0.f;

				case AxisType::gamepadAxis:
					return GetGamepadAxis(axis.gamepadID, axis.gamepadAxis);

				case AxisType::gamepadButton:
					if (IsGamepadButtonDown(axis.gamepadID, axis.gamepadPositiveButton)) return 1.f;
					else if (IsGamepadButtonDown(axis.gamepadID, axis.gamepadNegativeButton)) return -1.f;
					return 0.f;

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

	void InputManager::CreateAxis(std::string axisName)
	{
		Axis newAxis;
		newAxis.name = axisName;
		_axes.push_back(newAxis);
	}
	void InputManager::SetAxisType(std::string axisName, AxisType axisType)
	{
		_axes[GetAxisIndex(axisName)].type = axisType;
	}

	void InputManager::SetAxisPositiveKey(std::string axisName, int posKey)
	{
		_axes[GetAxisIndex(axisName)].positiveKey = posKey;
	}

	void InputManager::SetAxisNegativeKey(std::string axisName, int negKey)
	{
		_axes[GetAxisIndex(axisName)].negativeKey = negKey;
	}

	void InputManager::SetAxisGamepadID(std::string axisName, int id)
	{
		_axes[GetAxisIndex(axisName)].gamepadID = id;
	}

	void InputManager::SetAxisGamepadAxis(std::string axisName, int padAxis)
	{
		_axes[GetAxisIndex(axisName)].gamepadAxis = padAxis;
	}

	void InputManager::SetAxisGamepadPositiveButton(std::string axisName, int padposKey)
	{
		_axes[GetAxisIndex(axisName)].gamepadPositiveButton = padposKey;
	}

	void InputManager::SetAxisGamepadNegativeButton(std::string axisName, int padnegKey)
	{
		_axes[GetAxisIndex(axisName)].gamepadNegativeButton = padnegKey;
	}

	int InputManager::GetAxisIndex(std::string axisName)
	{
		for(int i=0;i<_axes.size();i++)
			if(_axes[i].name==axisName)
				return i;
		return -1;
	}
}