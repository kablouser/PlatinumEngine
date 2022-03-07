//
// Created by Jason on 07/02/2022.
//
#pragma once

#include <imgui.h>

#include <string>
#include <vector>

namespace PlatinumEngine
{
	class InputManager
	{
	public:

		enum class AxisType
		{
			keyboardMouseButton,
			mouseMovement,
			gamepadAxis,
			gamepadButton
		};

		//Defining Axis structure to theoretically handle mouse/keyboard/joystick inputs
		//Also will need to allow support for alternate inputs???
		struct Axis
		{
			std::string name;
			AxisType type;
			int	positiveKey;
			int	negativeKey;

			int gamepadID; // same as joystick ID
			int gamepadAxis;
			int gamepadPositiveButton;
			int gamepadNegativeButton;
		};

		InputManager();

		void ShowGUIWindow(bool* outIsOpen);

		//General functions to handle Mouse or Keyboard inputs
		bool IsMouseClicked(int button);
		bool IsMouseDown(int button);
		bool IsMouseReleased(int button);
		ImVec2 GetMousePosition();
		bool IsKeyPressed(int key);
		bool IsKeyDown(int key);
		bool IsKeyReleased(int key);

		float GetGamepadAxis(int gamepadID, int axis);
		bool IsGamepadButtonDown(int gamepadID, int button);

		//Specific function that will handle Axis related inputs
		float GetAxis(std::string axisName);

		// Todo
		// Handle button events and modulation keys

	private:

		std::vector<Axis> _axes;
	};
}
