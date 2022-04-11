//
// Created by Shihua on 02/03/2022.
//

#include <SceneEditor/SceneEditor.h>
#include <imgui.h>
#include <imgui_internal.h>
static ImGuizmo::OPERATION currentGizmoOperation(ImGuizmo::TRANSLATE);

namespace PlatinumEngine{
	// ___CONSTRUCTOR___

	SceneEditor::SceneEditor(InputManager* inputManager, Scene* scene, Renderer* renderer):
			_ifCameraSettingWindowOpen(false),
			_camera(), _fov(60), _near(4), _far(10000),

			_inputManager(inputManager),
			_scene(scene),
			_renderer(renderer),
			_renderTexture(),


			_mouseMoveDelta(0, 0) ,
			_mouseButtonType(-1),
			_wheelValueDelta(0),

			_useSnap(false),
			_boundSizing(false),
			_boundSizingSnap(false),

			_currentGizmoMode(ImGuizmo::LOCAL),

			_snap{ 1.f, 1.f, 1.f },
			_bounds{-0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f},
			_boundsSnap{ 0.1f, 0.1f, 0.1f },
			_fakeVeiwMatrix{},

			_currentClickedZone()
	{
		_inputManager->CreateAxis(std::string ("HorizontalAxisForEditorCamera"), GLFW_KEY_RIGHT, GLFW_KEY_LEFT, InputManager::AxisType::keyboardMouseButton);
		_inputManager->CreateAxis(std::string ("VerticalAxisForEditorCamera"), GLFW_KEY_UP, GLFW_KEY_DOWN, InputManager::AxisType::keyboardMouseButton);


		_framebufferWidth = 1;
		_framebufferHeight = 1;
		if (!_renderTexture.Create(_framebufferWidth, _framebufferHeight))
			return;

	}





	// ___FUNCTIONS___

	void SceneEditor::ShowGUIWindow(bool* outIsOpen)
	{
		//-------------------------------------------
		// begin the ImGui Scene Editor Main Window
		//-------------------------------------------
		if (ImGui::Begin(ICON_KI_MOVIE " Scene Editor", outIsOpen))
		{
			if(ImGui::IsKeyPressed(GLFW_KEY_Q))
				currentGizmoOperation = ImGuizmo::TRANSLATE;
			if(ImGui::IsKeyPressed(GLFW_KEY_W))
				currentGizmoOperation = ImGuizmo::ROTATE;
			if(ImGui::IsKeyPressed(GLFW_KEY_E))
				currentGizmoOperation = ImGuizmo::SCALE;
			if(ImGui::IsKeyPressed(GLFW_KEY_R))
				currentGizmoOperation = ImGuizmo::UNIVERSAL;
			//-----------
			// Widgets
			//-----------

			if (ImGui::Button("Camera Setting"))
			{

				_ifCameraSettingWindowOpen = !_ifCameraSettingWindowOpen;

			}
			//ImGuizmo
			ImGui::SameLine();
			if (ImGui::RadioButton("Translate", currentGizmoOperation == ImGuizmo::TRANSLATE))
				currentGizmoOperation = ImGuizmo::TRANSLATE;
			ImGui::SameLine();
			if (ImGui::RadioButton("Rotate", currentGizmoOperation == ImGuizmo::ROTATE))
				currentGizmoOperation = ImGuizmo::ROTATE;
			ImGui::SameLine();
			if (ImGui::RadioButton("Scale", currentGizmoOperation == ImGuizmo::SCALE))
				currentGizmoOperation = ImGuizmo::SCALE;
			ImGui::SameLine();
			if (ImGui::RadioButton("Universal", currentGizmoOperation == ImGuizmo::UNIVERSAL))
				currentGizmoOperation = ImGuizmo::UNIVERSAL;

			ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x - 130.f);

			if (currentGizmoOperation != ImGuizmo::SCALE)
			{
				if (ImGui::RadioButton("Local", _currentGizmoMode == ImGuizmo::LOCAL))
					_currentGizmoMode = ImGuizmo::LOCAL;
				ImGui::SameLine();
				if (ImGui::RadioButton("World", _currentGizmoMode == ImGuizmo::WORLD))
					_currentGizmoMode = ImGuizmo::WORLD;
			}

			ImGui::Checkbox("Bound Sizing", &_boundSizing);
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

			//----------------------------------------
			// Child window for Displaying framebuffer
			//----------------------------------------

			// Get the parent window size
			auto targetSize = ImGui::GetContentRegionAvail();

			if(ImGui::BeginChild("##e",ImVec2(targetSize.x, targetSize.y), false,ImGuiWindowFlags_NoMove))
			{
				//ImGuizmo

				//--------------------------------
				// update mouse && keyboard input
				//--------------------------------
				_mouseButtonType = _inputManager->GetMouseDown();

				_mouseMoveDelta = _inputManager->GetMouseMoveVector();

				_wheelValueDelta = _inputManager->GetMouseWheelDeltaValue();


				//------------------
				// Update Data
				//------------------
				Update(targetSize, _currentGizmoMode);
			}
			ImGui::EndChild();
		}


		ImGui::End();

	}



	void SceneEditor::Update(ImVec2 targetSize, ImGuizmo::MODE currentGizmoMode)
	{
		//--------------------------------------
		// check if mouse is inside the screen
		//--------------------------------------

		if(_inputManager->GetMousePosition().x <= targetSize.x
				&& _inputManager->GetMousePosition().x >= 0.f
				&& _inputManager->GetMousePosition().y <= targetSize.y
				&& _inputManager->GetMousePosition().y >= 0.f)
		{
			//---------------------
			// update view matrix
			//---------------------

			// check mouse click to do rotation and translation
			if (_mouseButtonType == 0 ) // for rotation
			{

				_camera.RotationByMouse(Maths::Vec2(_mouseMoveDelta.x, _mouseMoveDelta.y));

			}
			else if (_mouseButtonType == 1)// translation (up down left right)
			{

				_camera.TranslationByMouse(Maths::Vec2(_mouseMoveDelta.x, _mouseMoveDelta.y));

			}

			// check this is for moving camera closer/further
			if (_wheelValueDelta != 0)
			{
				_camera.TranslationByMouse(_wheelValueDelta);
			}

		}

		// check if there is any keyboard input
		if (_inputManager->IsKeyPressed(GLFW_KEY_UP) || _inputManager->IsKeyPressed(GLFW_KEY_DOWN) ||
			_inputManager->IsKeyPressed(GLFW_KEY_LEFT) || _inputManager->IsKeyPressed(GLFW_KEY_RIGHT))
			_camera.TranslationByKeyBoard(_inputManager->GetAxis("VerticalAxisForEditorCamera"), _inputManager->GetAxis("HorizontalAxisForEditorCamera"));



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



		//--------------------
		// Render Objects
		//--------------------

		if(1.0f < targetSize.x && 1.0f < targetSize.y)
		{
			// resize framebuffer if necessary
			if (_framebufferWidth != (int)targetSize.x || _framebufferHeight != (int)targetSize.y)
			{
				_framebufferWidth = (int)targetSize.x;
				_framebufferHeight = (int)targetSize.y;
				_renderTexture.Create(_framebufferWidth, _framebufferHeight);
			}

			_renderTexture.Bind();
			glEnable(GL_DEPTH_TEST);
			glViewport(0, 0, _framebufferWidth, _framebufferHeight);
			glClearColor(0.35f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


			_renderer->Begin();
			// Update rendering information to renderer
			_scene->Render(*_renderer);
			_renderer->SetModelMatrix();
			_renderer->SetViewMatrix(_camera.viewMatrix4);
			_renderer->SetProjectionMatrix(_camera.projectionMatrix4);
			_renderer->SetLightProperties();
			_renderer->End();

			_renderTexture.Unbind();
			glDisable(GL_DEPTH_TEST);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);


			// display updated framebuffer
			ImGui::Image(_renderTexture.GetColorTexture().GetImGuiHandle(), targetSize, ImVec2(0, 1), ImVec2(1, 0));

			// display gizmos
			UseGizmo(_camera.viewMatrix4.matrix, _camera.projectionMatrix4.matrix, currentGizmoMode);
		}

	}

	void SceneEditor::UseGizmo(float* cameraView, float* cameraProjection, ImGuizmo::MODE currentGizmoMode)
	{
		Maths::Mat4 identityMatrix(1);

		ImGuizmo::SetDrawlist();
		auto windowWidth = (float)ImGui::GetWindowWidth();
		auto windowHeight = (float)ImGui::GetWindowHeight();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
		float viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
		float viewManipulateTop = ImGui::GetWindowPos().y;


		ImGuizmo::Manipulate(
				cameraView, cameraProjection, currentGizmoOperation, currentGizmoMode,
				identityMatrix.matrix, NULL, _useSnap ? &_snap[0] : NULL,
				_boundSizing ? _bounds : NULL, _boundSizingSnap ? _boundsSnap : NULL);


		// don't want the view manipulate gizmo to change the view matrix, so use this to avoid that.
		memcpy(_fakeVeiwMatrix,cameraView,sizeof(float) * 16);

		// view manipulate gizmo
		ImGuizmo::ViewManipulate(_fakeVeiwMatrix, sqrt(LengthSquared(_camera.GetForwardDirection())), ImVec2(viewManipulateRight - 100, viewManipulateTop),
				ImVec2(100, 100), 0x10101010);

		// grid
		ImGuizmo::DrawGrid(cameraView, cameraProjection, identityMatrix.matrix, 100.f);
	}
}



