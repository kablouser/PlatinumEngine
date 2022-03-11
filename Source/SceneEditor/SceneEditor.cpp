//
// Created by Shihua on 02/03/2022.
//

#include "SceneEditor/SceneEditor.h"
#include <imgui.h>
#include <stdio.h>


namespace PlatinumEngine{



	// ___CONSTRUCTOR___

	SceneEditor::SceneEditor( unsigned int depthBits,
								unsigned int stencilBits,
								unsigned int antiAliasingLevel):
			_ifCameraSettingWindowOpen(false),
			_camera(), _fov(0), _near(0.4), _far(10000),_inputManager()

	{

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
				ImVec2 value_raw = _inputManager.MouseMoveVector();
				ImGui::Text("Detect Mouse Movement between update loop: (%.1f, %.1f)", value_raw.x, value_raw.y);


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
		// get viewport size
		auto targetSize = ImGui::GetContentRegionAvail();


		// update projection matrix
		if(_camera.isOrthogonal)
		{
			_camera.SetOrthogonalMatrix(-targetSize.x/2,targetSize.x/2,
					-targetSize.y/2,targetSize.y/2,(float)_near,(float)_far);


		}
		else
		{
			_camera.SetPerspectiveMatrix((float)_fov, targetSize.x / targetSize.y, (float)_near, (float)_far);
		}
	}

}



