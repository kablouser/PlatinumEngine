//
// Created by Shihua on 02/03/2022.
//

#include <SceneEditor/SceneEditor.h>
#include <imgui.h>



namespace PlatinumEngine{



	// ___CONSTRUCTOR___

	SceneEditor::SceneEditor(InputManager* inputManager, Scene* scene, Renderer* renderer):
			_ifCameraSettingWindowOpen(false),
			_camera(), _fov(60), _near(4), _far(10000),_inputManager(inputManager), _scene(scene),
			_mouseMoveDelta(0, 0) ,_mouseButtonType(InputManagerExtend::MouseButtonType::none),
			_wheelValueDelta(0),_renderTexture(), _renderer(renderer)

	{
		_inputManager->CreateAxis(std::string ("Horizontal"), GLFW_KEY_RIGHT, GLFW_KEY_LEFT, InputManagerExtend::AxisType::keyboardMouseButton);
		_inputManager->CreateAxis(std::string ("Vertical"), GLFW_KEY_UP, GLFW_KEY_DOWN, InputManagerExtend::AxisType::keyboardMouseButton);

		// use a random number instead
		_renderTexture.Create(1.f,1.f);

	}





	// ___FUNCTIONS___

	void SceneEditor::ShowGUIWindow(bool* outIsOpen)
	{
		//-------------------------------------------
		// begin the ImGui Scene Editor Main Window
		//-------------------------------------------
		if (ImGui::Begin("Scene Editor", outIsOpen))
		{


			// Get window size
			auto targetSize = ImGui::GetContentRegionAvail();


			if(ImGui::BeginChild("##e",ImVec2(targetSize.x, targetSize.y), false,ImGuiWindowFlags_NoMove))
			{
				//-----------
				// Widgets
				//-----------

				_mouseButtonType = _inputManager->GetMouseDown();

				_mouseMoveDelta = _inputManager->GetMouseMoveVector();

				_wheelValueDelta = _inputManager->GetMouseWheelDeltaValue();


				if (ImGui::Button("Camera Setting"))
				{

					_ifCameraSettingWindowOpen = !_ifCameraSettingWindowOpen;

				}

				//-------------
				// Sub window
				//-------------


				// Camera setting window

				if (_ifCameraSettingWindowOpen)
				{

					if (ImGui::Begin("Camera Setting", &_ifCameraSettingWindowOpen))
					{

						// ___Widgets in this window___

						// Button

						if (_camera.isOrthogonal)
						{
							if (ImGui::Button("Orthogonal"))
							{
								_camera.isOrthogonal = !_camera.isOrthogonal;
							}

							// Text boxes

							ImGui::InputInt("Near", &_near, 16);

							ImGui::InputInt("Far", &_far, 16);
						}
						else
						{
							if (ImGui::Button("Perspective"))
							{
								_camera.isOrthogonal = !_camera.isOrthogonal;
							}

							// Slider

							ImGui::SliderInt("Field of View", &_fov, 4, 160);


							// Text boxes

							ImGui::InputInt("Near", &_near, 16);

							ImGui::InputInt("Far", &_far, 16);

						}




						// ___Window Interaction Features___

						if (!ImGui::IsWindowFocused())
							_ifCameraSettingWindowOpen = false;




					}


					ImGui::End();
				}


				//------------------
				// Update Data
				//------------------
				Update(targetSize);


			}
			ImGui::EndChild();
		}


		ImGui::End();

	}



	void SceneEditor::Update(ImVec2 targetSize)
	{

		//--------------------------------------
		// check if mouse is inside the screen
		//--------------------------------------
		if(_inputManager->GetMousePosition().x <= targetSize.x
				&& _inputManager->GetMousePosition().x >= 0.f
				&& _inputManager->GetMousePosition().y <= targetSize.y
				&& _inputManager->GetMousePosition().y >= 0.f
				&& ImGui::IsWindowFocused())
		{
			//---------------------
			// update view matrix
			//---------------------

			// check mouse click to do rotation and translation
			if (_mouseButtonType == 0) // for rotation
			{

				_camera.RotationByMouse(Maths::Vec2(_mouseMoveDelta.x, _mouseMoveDelta.y));

			}

			else if (_mouseButtonType == 1)// for rotation
			{

				_camera.RotationByMouse(Maths::Vec2(_mouseMoveDelta.x, _mouseMoveDelta.y));

			}
			else if (_mouseButtonType == 2)// translation (up down left right)
			{

				_camera.TranslationByMouse(Maths::Vec2(_mouseMoveDelta.x, _mouseMoveDelta.y));

			}

			// check this is for moving camera closer/further
			if (_wheelValueDelta != 0)
			{

				_camera.TranslationByMouse(_wheelValueDelta);

			}

			// check if there is any keyboard input
			if (_inputManager->IsKeyPressed(GLFW_KEY_UP) || _inputManager->IsKeyPressed(GLFW_KEY_DOWN) ||
				_inputManager->IsKeyPressed(GLFW_KEY_LEFT) || _inputManager->IsKeyPressed(GLFW_KEY_RIGHT))
				_camera.TranslationByKeyBoard(_inputManager->GetAxis("Vertical"), _inputManager->GetAxis("Horizontal"));




		}

		//---------------------------
		// Update projection matrix
		//---------------------------

		// check camera type
		if (_camera.isOrthogonal)
		{

			// update as orthogonal projection matrix
			_camera.SetOrthogonalMatrix(-targetSize.x / 20.f, targetSize.x / 20.f,
					-targetSize.y / 20.f, targetSize.y / 20.f, (float)_near, (float)_far);

		}
		else
		{


			// update as perspective projection matrix
			_camera.SetPerspectiveMatrix(3.14159265f * (float)_fov / 180.f,
					targetSize.x / targetSize.y,
					(float)_near, (float)_far);

		}

		//------------------------------------------
		// Update rendering information to renderer
		//------------------------------------------
		_scene->Render(_renderer);

		//--------------------
		// Render Objects
		//--------------------

		// pass framebuffer to renderer
		_renderer->SetFramebuffer(&_renderTexture);

		if(1.0f < targetSize.x && 1.0f < targetSize.y)
		{
			// resize framebuffer if necessary
			if (_renderer->_framebufferWidth != (int)targetSize.x || _renderer->_framebufferHeight != (int)targetSize.y)
			{
				_renderer->ResizeFrameBuffer(_renderer->_framebuffer, targetSize);
			}

			_renderer->Begin();
			Mesh mesh(_renderer->vertices, _renderer->indices);
			_renderer->LoadMesh(mesh);
			_renderer->SetModelMatrix();
			_renderer->SetViewMatrix(_camera.viewMatrix4);
			_renderer->SetProjectionMatrix(_camera.projectionMatrix4);
			_renderer->SetLightProperties();
			_renderer->Render();

			_renderer->LoadLight();
			_renderer->_lightShaderInput.Draw();
			_renderer->_lightShader.Unbind();
			_renderer->End();


			// display updated framebuffer
			ImGui::Image(_renderTexture.GetColorTexture().GetImGuiHandle(), targetSize);
		}

	}
}



