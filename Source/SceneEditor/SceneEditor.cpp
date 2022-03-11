//
// Created by Shihua on 02/03/2022.
//

#include "SceneEditor/SceneEditor.h"
#include <imgui.h>
#include <stdio.h>


namespace PlatinumEngine{



	// ___CONSTRUCTOR___

	SceneEditor::SceneEditor(InputManagerExtend* inputManager)://, Scene* scene):
			_ifCameraSettingWindowOpen(false),
			_camera(), _fov(0), _near(0.4), _far(10000),_inputManager(inputManager),//, _scene(scene)
			_mouseMoveDelta(0, 0) ,_mouseButtonType(InputManagerExtend::MouseButtonType::none),
			_wheelValueDelta(0)

	{
		_inputManager->CreateAxis(std::string ("Horizontal"), GLFW_KEY_RIGHT, GLFW_KEY_LEFT, InputManagerExtend::AxisType::keyboardMouseButton);
		_inputManager->CreateAxis(std::string ("Vertical"), GLFW_KEY_UP, GLFW_KEY_DOWN, InputManagerExtend::AxisType::keyboardMouseButton);

		// use a random number instead
		//	_renderTexture.create(1.f,1.f,_contextSettings);

	}





	// ___FUNCTIONS___

	void SceneEditor::ShowGUIWindow(bool* outIsOpen, Renderer& rasterRenderer)
	{
		//-------------------------------------------
		// begin the ImGui Scene Editor Main Window
		//-------------------------------------------


		if (ImGui::Begin("Scene Editor", outIsOpen))
		{

			// Get window size
			auto targetSize = ImGui::GetContentRegionAvail();


			if(ImGui::BeginChild("##e",ImVec2(targetSize.x, targetSize.y), 0,ImGuiWindowFlags_NoMove))
			{
				//-----------
				// Widgets
				//-----------

				_inputManager->GetMouseMoveVector(_mouseMoveDelta,_mouseButtonType);

				_wheelValueDelta = _inputManager->GetMouseWheelDeltaValue();


				ImGui::Text("Wheel shift value: %.1f", _wheelValueDelta);
				ImGui::Text("Detect Mouse Movement: Button %d is pressed, delta: (%.1f, %.1f)", _mouseButtonType, _mouseMoveDelta.x, _mouseMoveDelta.y);
				ImGui::Text("Projection Matrix: (\n %.1f, %.1f, %.1f, %.1f \n %.1f, %.1f, %.1f, %.1f \n %.1f, %.1f, %.1f, %.1f \n %.1f, %.1f, %.1f, %.1f \n)",
						_camera.projectionMatrix4[0][0],_camera.projectionMatrix4[0][1], _camera.projectionMatrix4[0][2],_camera.projectionMatrix4[0][3],
						_camera.projectionMatrix4[1][0],_camera.projectionMatrix4[1][1], _camera.projectionMatrix4[1][2],_camera.projectionMatrix4[1][3],
						_camera.projectionMatrix4[2][0],_camera.projectionMatrix4[2][1], _camera.projectionMatrix4[2][2],_camera.projectionMatrix4[2][3],
						_camera.projectionMatrix4[3][0],_camera.projectionMatrix4[3][1], _camera.projectionMatrix4[3][2],_camera.projectionMatrix4[3][3]
				);


				if (ImGui::Button("Camera Setting"))
				{

					_ifCameraSettingWindowOpen = _ifCameraSettingWindowOpen ^ true;

				}


				//------------
				// Rendering
				//------------

				// get window size
				//auto targetSize = ImGui::GetContentRegionAvail();

				// get rendering result from Renderer
				// rasterRenderer.RenderObjects(_renderTexture, targetSize);

				// put target into GUI window
				// ImGui::Image(_renderTexture, targetSize);



				//-------------
				// Sub window
				//-------------


				// Camera setting window

				if (_ifCameraSettingWindowOpen == true)
				{

					if (ImGui::Begin("Camera Setting", &_ifCameraSettingWindowOpen))
					{

						// ___Widgets in this window___

						// Button

						if (_camera.isOrthogonal)
						{
							if (ImGui::Button("Orthogonal"))
							{
								_camera.isOrthogonal = _camera.isOrthogonal ^ 1;
							}

							// Text boxes

							ImGui::InputInt("Near", &_near, 16);

							ImGui::InputInt("Far", &_far, 16);
						}
						else
						{
							if (ImGui::Button("Perspective"))
							{
								_camera.isOrthogonal = _camera.isOrthogonal ^ 1;
							}

							// Slider

							ImGui::SliderInt("Field of View", &_fov, 4, 40);


							// Text boxes

							ImGui::InputInt("Near", &_near, 16);

							ImGui::InputInt("Far", &_far, 16);

						}




						// ___Window Interaction Features___

						if (ImGui::IsWindowFocused() == false)
							_ifCameraSettingWindowOpen = false;




					}
					ImGui::End();
				}


				//------------------
				// Update Data
				//------------------

				Update();

			}
			ImGui::EndChild();
		}
		ImGui::End();

	}



	void SceneEditor::Update()
	{
		///__get viewport size__

		auto targetSize = ImGui::GetContentRegionAvail();




		///__update view matrix__

		// check mouse click to do rotation and translation

		// rotation
		if(_mouseButtonType == InputManagerExtend::MouseButtonType::left)
		{

			_camera.RotationByMouse(Maths::Vec2(_mouseMoveDelta.x, _mouseMoveDelta.y));

		}
		// rotation
		else if(_mouseButtonType == InputManagerExtend::MouseButtonType::right)
		{

			_camera.RotationByMouse(Maths::Vec2(_mouseMoveDelta.x, _mouseMoveDelta.y));

		}
		// translation (up down left right)
		else if(_mouseButtonType == InputManagerExtend::MouseButtonType::middle)
		{

			_camera.TranslationByMouse(Maths::Vec2(_mouseMoveDelta.x, _mouseMoveDelta.y));

		}

		// check this is for moving camera closer/further
		if(_wheelValueDelta != 0)
		{

			_camera.TranslationByMouse(_wheelValueDelta);

		}

		// check if there is any keyboard input
		if(_inputManager->IsKeyPressed(GLFW_KEY_UP) || _inputManager->IsKeyPressed(GLFW_KEY_DOWN) || _inputManager->IsKeyPressed(GLFW_KEY_LEFT) || _inputManager->IsKeyPressed(GLFW_KEY_RIGHT))
			_camera.TranslationByKeyBoard(_inputManager->GetAxis("Vertical"), _inputManager->GetAxis("Horizontal"));




		///__Update projection matrix__

		// check camera type
		if(_camera.isOrthogonal)
		{
			// update as orthogonal projection matrix
			_camera.SetOrthogonalMatrix(-targetSize.x/2,targetSize.x/2,
					-targetSize.y/2,targetSize.y/2,(float)_near,(float)_far);

		}
		else
		{
			// update as perspective projection matrix
			_camera.SetPerspectiveMatrix((float)_fov, targetSize.x / targetSize.y, (float)_near, (float)_far);
		}
	}

}



