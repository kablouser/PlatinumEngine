//
// Created by Jason on 07/02/2022.
//

#ifndef PLATINUMENGINE_INPUTMANAGER_H
#define PLATINUMENGINE_INPUTMANAGER_H

namespace PlatinumEngine
{
//	int TYPE_KEYBOARD_MOUSE_BUTTON=0;
//	int TYPE_MOUSE_MOVEMENT=1;
//	int TYPE_JOYSTICK_AXIS=2;

	//Defining Axis structure to theoretically handle mouse/keyboard/joystick inputs
	//Also will need to allow support for alternate inputs???
	struct Axis
	{
	public:
		std::string Name;
		int positiveKey, negativeKey, Type;

	public:
		float GetValue(){return Value;}
		void SetValue(float value){Value = value;}

	private:
		float Value;
	};
	class InputManager
	{
	public:
		void InputScene();
		bool IsMouseClicked(int button);
		bool IsMouseDown(int button);
		bool IsMouseReleased(int button);
		ImVec2 GetMousePosition();
		bool IsKeyPressed(int key);
		bool IsKeyDown(int key);
		bool IsKeyReleased(int key);
		float GetAxis(std::string Axis);

	private:
		std::vector<Axis> Axes;

	public:
		//May need to change how keys are handled
		enum KeyCode{A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,Alpha0,Alpha1,Alpha2,Alpha3,Alpha4,Alpha5,Alpha6,Alpha7,Alpha8,Alpha9,LeftCtrl=37,LeftShift,LeftAlt,Win,RightCtrl,RightShift,RightAlt,LeftArrow=71,RightArrow,UpArrow,DownArrow};
	};
}

#endif