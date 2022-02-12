//
// Created by Jason on 07/02/2022.
//

#ifndef PLATINUMENGINE_INPUTMANAGER_H
#define PLATINUMENGINE_INPUTMANAGER_H

namespace PlatinumEngine
{
	//Defining Axis structure to theoretically handle mouse/keyboard/joystick inputs
	//Also will need to allow support for alternate inputs???
	struct Axis
	{
	public:
		std::string name;
		int positiveKey, negativeKey, type, joyID;
		sf::Joystick::Axis axis;
	};
	class InputManager
	{
	public:
		void InputScene();
		
		//General functions to handle Mouse or Keyboard inputs
		bool IsMouseClicked(int button);
		bool IsMouseDown(int button);
		bool IsMouseReleased(int button);
		ImVec2 GetMousePosition();
		bool IsKeyPressed(int key);
		bool IsKeyDown(int key);
		bool IsKeyReleased(int key);

		//Specific function that will handle Axis related inputs
		float GetAxis(std::string Axis, int joyid = 0);

	public:
		//Constants for Input types
		int TYPE_KEYBOARD_MOUSE_BUTTON=0;
		int TYPE_MOUSE_MOVEMENT=1;
		int TYPE_JOYSTICK_AXIS=2;
	private:
		std::vector<Axis> Axes;
	};
}

#endif