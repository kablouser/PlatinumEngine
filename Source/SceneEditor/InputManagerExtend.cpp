//
// Created by Shihua on 10/03/2022.
//

#include "SceneEditor/InputManagerExtend.h"
#include <GLFW/glfw3.h>

namespace PlatinumEngine
{

	///__CONSTRUCTOR__
	InputManagerExtend::InputManagerExtend()
			:_previousMousePosition(0,0) ,
			_clickedMouseButton(none)
	{}



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

	void InputManagerExtend::GetMouseMoveVector(ImVec2& delta, MouseButtonType& pressedMouseButtonType)
	{

		// Check pressed mouse button type
		pressedMouseButtonType = GetPressedMouseButtonType();

		// Initialize delta.
		delta = ImVec2(0.f, 0.f);

		// if the key is pressed for the first time
		if(_clickedMouseButton != pressedMouseButtonType)
		{
			// update previous position
			_previousMousePosition = GetMousePosition();
		}
		// if the key is pressed before
		else if(pressedMouseButtonType != none)
		{

			// get current position
			ImVec2 currentPosition = GetMousePosition();


			// calculate delta (distance and direction)
			delta.x = currentPosition.x - _previousMousePosition.x;
			delta.y = currentPosition.y - _previousMousePosition.y;


			// update previous position
			_previousMousePosition = GetMousePosition();

		}

		_clickedMouseButton = pressedMouseButtonType;

	}

	float InputManagerExtend::GetMouseWheelDeltaValue()
	{

		float currentWheelValue = ImGui::GetIO().MouseWheel;

		float delta = currentWheelValue - 0;


		return delta;

	}

	void InputManagerExtend::CreateAxis(std::string axisName, int positiveKey,
			int negativeKey, AxisType inputType)
	{
		Axis axis;
		axis.name=axisName;
		axis.positiveKey = positiveKey;
		axis.negativeKey = negativeKey;
		axis.type = inputType;
		_axes.push_back(axis);
	}

	//Returns value of the Axis defined by AxisName
	float InputManagerExtend::GetAxis(std::string axisName)
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

}