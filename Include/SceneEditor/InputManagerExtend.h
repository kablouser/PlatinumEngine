//
// Created by Shihua on 10/03/2022.
//

#pragma once


#include "InputManager/InputManager.h"

namespace PlatinumEngine{


	class InputManagerExtend: public InputManager
	{
	public:
		///__ENUM__
		enum MouseButtonType{none, left, right, middle};

		///__FUNCTIONS__
		ImVec2 MouseMoveVector();
		MouseButtonType GetPressedMouseButtonType();

		///__CONSTRUCTORS__
		InputManagerExtend();


	private:
		///__PARAMETERS__
		ImVec2 _previousMousePosition;
		MouseButtonType _clickedMouseButton;
		bool _isFirstMouseWheelPress;

	};


}