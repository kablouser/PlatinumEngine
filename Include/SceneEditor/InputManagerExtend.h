//
// Created by Shihua on 10/03/2022.
//

#pragma once

#include "Maths/Vectors.h"

#include "InputManager/InputManager.h"


namespace PlatinumEngine
{


	class InputManagerExtend: public InputManager
	{
	public:
		///__ENUM__
		enum MouseButtonType{none, left, right, middle};

		///__FUNCTIONS__

		/**
		 * This function can help to get which mouse button is down
		 * and how the mouse move between every update when it is down.
		 * @param delta : reference of delta, which will ge a updated delta value (direction + magnitude of the mouse movement).
		 * @param pressedMouseButtonType : reference of the Mouse Button Type. it record which button is down.
		 */
		void GetMouseMoveVector(ImVec2& delta, MouseButtonType& pressedMouseButtonType);


		/**
		 * This function can help to check which button is pressed in the current update loop.
		 * @return : return the type of pressed mouse button.
		 */
		MouseButtonType GetPressedMouseButtonType();


		float GetMouseWheelDeltaValue();


		void CreateAxis(std::string axisName, int positiveKey, int negativeKey, AxisType inputType);

		float GetAxis(std::string axisName);

		///__CONSTRUCTORS__
		InputManagerExtend();


	private:
		///__PARAMETERS__
		ImVec2 _previousMousePosition;
		MouseButtonType _clickedMouseButton;
		std::vector<Axis> _axes;



	};


}