//
// Created by Jason on 07/02/2022.
//
#pragma once

namespace PlatinumEngine
{
	class InputManager
	{
	public:

		enum class AxisType
		{
			keyboardMouseButton,
			mouseMovement,
			joystickAxis
		};

		//Defining Axis structure to theoretically handle mouse/keyboard/joystick inputs
		//Also will need to allow support for alternate inputs???
		struct Axis
		{
			std::string name;
			AxisType type;
			int
					positiveKey,
					negativeKey,
					joystickID;
			sf::Joystick::Axis joystickAxis;
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

		//Specific function that will handle Axis related inputs
		float GetAxis(std::string axisName, int joystickID = 0);

	private:
		std::vector<Axis> _axes;
	};
}
