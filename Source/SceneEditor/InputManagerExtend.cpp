//
// Created by Shihua on 10/03/2022.
//

#include "SceneEditor/InputManagerExtend.h"


namespace PlatinumEngine
{

	///__CONSTRUCTOR__
	InputManagerExtend::InputManagerExtend()
			:InputManager(),_previousMousePosition(0,0) ,
			_clickedMouseButton(none) ,_isFirstMouseWheelPress(false){}



	///__FUNCTION__

	InputManagerExtend::MouseButtonType InputManagerExtend::GetPressedMouseButtonType()
	{

		if(IsMouseDown(0))
		{
			return left;

		}
		else if(IsMouseDown(1))
		{
			return right;
		}
		else if(IsMouseDown(2))
		{
			return middle;

		}

		return none;

	}

	ImVec2 InputManagerExtend::MouseMoveVector()
	{

		// Check pressed mouse button type
		MouseButtonType mouseButtonType = GetPressedMouseButtonType();

		// Initialize delta.
		ImVec2 delta(0.f, 0.f);

		// if the key is pressed for the first time
		if(_clickedMouseButton != mouseButtonType)
		{
			// update previous position
			_previousMousePosition = GetMousePosition();
		}
		// if the key is pressed before
		else if(mouseButtonType != none){

			// get current position
			ImVec2 currentPosition = GetMousePosition();


			// calculate delta (distance and direction)
			delta.x = currentPosition.x - _previousMousePosition.x;
			delta.y = currentPosition.y - _previousMousePosition.y;


			// update previous position
			_previousMousePosition = GetMousePosition();

		}

		_clickedMouseButton = mouseButtonType;

		return delta;


	}

}