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
			_mouseMoveDelta(0, 0) ,_mouseButtonType(-1),
			_wheelValueDelta(0),_renderTexture(), _renderer(renderer)

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
		if (ImGui::Begin("Scene Editor", outIsOpen))
		{
			//-----------
			// Widgets
			//-----------

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

			//----------------------------------------
			// Child window for Displaying framebuffer
			//----------------------------------------

			// Get the parent window size
			auto targetSize = ImGui::GetContentRegionAvail();

			if(ImGui::BeginChild("##e",ImVec2(targetSize.x, targetSize.y), false,ImGuiWindowFlags_NoMove))
			{

				//--------------------------------
				// update mouse && keyboard input
				//--------------------------------
				_mouseButtonType = _inputManager->GetMouseDown();

				_mouseMoveDelta = _inputManager->GetMouseMoveVector();

				_wheelValueDelta = _inputManager->GetMouseWheelDeltaValue();


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
				_camera.TranslationByKeyBoard(_inputManager->GetAxis("VerticalAxisForEditorCamera"), _inputManager->GetAxis("HorizontalAxisForEditorCamera"));
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

		//---------------------------------------------
		// check if the mouse is selecting game object
		//---------------------------------------------
		if(_inputManager->IsMouseDown(1))
			SelectGameObjectFromScene();

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
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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
			ImGui::Image(_renderTexture.GetColorTexture().GetImGuiHandle(), targetSize);
		}

	}


	void SceneEditor::SelectGameObjectFromScene()
	{
		ImVec2 mouseClickedPosition = _inputManager->GetMousePosition();

		// do ray casting
		if(GameObject* result = DoRayCasting(mouseClickedPosition);
				result != nullptr)
			PLATINUM_INFO(result->name);
	}


	GameObject* SceneEditor::DoRayCasting(ImVec2& clickedPosition)
	{
		//-----------------//
		// Calculate ray   //
		//-----------------//
		PlatinumEngine::Maths::Vec3 ray;

		// check projection type
		if(_camera.isOrthogonal)
		{
			ray = PlatinumEngine::Maths::Vec3(0.f, 0.f, 1.f);
		}
		else
		{
			// turning screen coordinate 2d back to the near panel (of the frustum) coordinate 2d
			float nearPanelCoordinateForClickedPosition_x =(clickedPosition.x *2.f / (float)_framebufferWidth)- 1.f;
			float nearPanelCoordinateForClickedPosition_y =(clickedPosition.y * 2.f / (float)_framebufferHeight)- 1.f;

			// Clipping space
			PlatinumEngine::Maths::Vec4 worldCoordinateForClickedPositionCandidate4D(nearPanelCoordinateForClickedPosition_x,
					nearPanelCoordinateForClickedPosition_y, 1.0f, 1.0f);

			// View space
			worldCoordinateForClickedPositionCandidate4D = PlatinumEngine::Maths::Inverse(_camera.projectionMatrix4) * worldCoordinateForClickedPositionCandidate4D;

			// World space
			worldCoordinateForClickedPositionCandidate4D = PlatinumEngine::Maths::Inverse(_camera.viewMatrix4) * worldCoordinateForClickedPositionCandidate4D;

			PlatinumEngine::Maths::Vec3 worldCoordinateForClickedPosition(
					worldCoordinateForClickedPositionCandidate4D.x/worldCoordinateForClickedPositionCandidate4D.w,
					worldCoordinateForClickedPositionCandidate4D.y/worldCoordinateForClickedPositionCandidate4D.w,
					worldCoordinateForClickedPositionCandidate4D.z/worldCoordinateForClickedPositionCandidate4D.w);


			// Calculate ray
			ray = worldCoordinateForClickedPosition - _camera.GetCameraPosition();
		}


		//----------------------------------------//
		// Loop through Every root game object    //
		//----------------------------------------//

		// variable that store the current selected game object
		GameObject* currentSelectedGameobject = nullptr;
		float closetZValue = (float)-_far;

		// loop through all the root game object from scene class
		unsigned int numberOfRootGameobject = _scene->GetRootGameObjectsCount();

		for(int gameObjectIndex =0; gameObjectIndex < numberOfRootGameobject; gameObjectIndex++)
		{
			// get current game object
			GameObject* currentGameobject = _scene->GetRootGameObject(gameObjectIndex);

			// check if the object enable
			if(currentGameobject->IsEnabledInHierarchy())
			{
				// get result for whether this game object or one of its children is selected
				currentSelectedGameobject = CheckRayTriangleIntersectionForGameobject(currentGameobject, ray,
						_camera.GetCameraPosition(), currentSelectedGameobject, closetZValue);

			}
		}
		return currentSelectedGameobject;
	}

	// for trianlge mesh only
	GameObject* SceneEditor::CheckRayTriangleIntersectionForGameobject(GameObject* currentCheckingGameobject, PlatinumEngine::Maths::Vec3 inRay,
			const PlatinumEngine::Maths::Vec3& inCameraPosition, GameObject* currentSelectedGameObject, float& closestZValueForCrossPoint)
	{

		// check if the game object enable
		if(!currentCheckingGameobject->IsEnabledInHierarchy())
			return currentSelectedGameObject;

		//------------------//
		// Do intersection  //
		//------------------//

		// check if the game object has mesh component
		if( auto renderComponent = currentCheckingGameobject->GetComponent<RenderComponent>(); renderComponent!= nullptr)
		{
			// fetch mesh
			Mesh mesh = renderComponent->GetMesh();

			// loop all the vertices
			for(int count =0; count < mesh.indices.size(); count+=3)
			{
				//----------------------------------//
				// Do ray and triangle intersection //
				//----------------------------------//

				// build the PCS using the triangle primitive
				Maths::Vec3 originForPCS = mesh.vertices[mesh.indices[count + 0]].position;

				Maths::Vec3 axisUForPCS = (mesh.vertices[mesh.indices[count + 1]].position - originForPCS);
				float lengthU = sqrtf(LengthSquared(axisUForPCS));
				axisUForPCS = axisUForPCS/lengthU;

				Maths::Vec3 axisNForPCS = Cross(axisUForPCS, (mesh.vertices[mesh.indices[count + 2]].position - originForPCS));
				float lengthN = sqrtf(LengthSquared(axisNForPCS));
				axisNForPCS = axisNForPCS/lengthN;

				Maths::Vec3 axisWForPCS = Cross(axisUForPCS, axisNForPCS);
				float lengthW = sqrtf(LengthSquared(axisWForPCS));
				axisWForPCS = axisWForPCS/lengthW;

				// find the intersection point for ray and the panel that contains the triangle
				Maths::Vec3 vectorBetweenOriginForPCSandCameraPosition = originForPCS - inCameraPosition;

				float rateForLineApproachingPanel = Maths::Dot(inRay, axisNForPCS);

				// make sure the ray is not contained by the same panel as the triangle primitive
				if(rateForLineApproachingPanel != 0)
					rateForLineApproachingPanel = Maths::Dot(vectorBetweenOriginForPCSandCameraPosition, axisNForPCS) / rateForLineApproachingPanel;
				else
					return currentSelectedGameObject;

				Maths::Vec3 crossPoint = inCameraPosition + inRay * rateForLineApproachingPanel;

				//------------------------------------//
				// Do barycentric interpolation check //
				//------------------------------------//

				// Convert the cross point and the vertices of the triangle into PCS coordinate
				Maths::Mat3 PCSMatrix3x3(std::array<float, 9>({
						axisUForPCS.x, axisWForPCS.x, axisNForPCS.x,
						axisUForPCS.y, axisWForPCS.y, axisNForPCS.y,
						axisUForPCS.z, axisWForPCS.z, axisNForPCS.z
				}).data());
				// 1. cross point
				Maths::Vec3 crossPointPCS = PCSMatrix3x3 * (crossPoint - originForPCS);

				// 2. triangle vertices
				Maths::Vec3 vertex0 = PCSMatrix3x3 * (mesh.vertices[mesh.indices[count + 0]].position - originForPCS);
				Maths::Vec3 vertex1 = PCSMatrix3x3 * (mesh.vertices[mesh.indices[count + 1]].position - originForPCS);
				Maths::Vec3 vertex2 = PCSMatrix3x3 * (mesh.vertices[mesh.indices[count + 2]].position - originForPCS);

				// Do barycentric interpolation to check
				Maths::Vec3 xValueForBarycentricInterpolation((vertex0 - vertex1).x, (vertex0 - vertex2).x, (crossPointPCS - vertex0).x);
				xValueForBarycentricInterpolation = xValueForBarycentricInterpolation / sqrtf(LengthSquared(xValueForBarycentricInterpolation));

				Maths::Vec3 yValueForBarycentricInterpolation((vertex0 - vertex1).y, (vertex0 - vertex2).y, (crossPointPCS - vertex0).y);
				yValueForBarycentricInterpolation = yValueForBarycentricInterpolation/ sqrtf(LengthSquared(yValueForBarycentricInterpolation));

				// Use the features of cross product to calculate the coefficients for interpolation
				Maths::Vec3 coefficientForInterpolation = Maths::Cross(xValueForBarycentricInterpolation , yValueForBarycentricInterpolation);

				// make sure the z is not zero
				if(coefficientForInterpolation.z != 0)
				{
					// check if the final coefficients for interpolation are all valid
					if (1.f - ((coefficientForInterpolation.x + coefficientForInterpolation.y) /coefficientForInterpolation.z) > 0.f
						&& coefficientForInterpolation.x / coefficientForInterpolation.z > 0.f
						&& coefficientForInterpolation.y / coefficientForInterpolation.z > 0.f)
					{

						//----------------------------------------------------//
						// Check whether to update the selected game object   //
						//----------------------------------------------------//

						// transform the object coordinate into view coordinate
						PlatinumEngine::Maths::Vec4 crossPointViewCoordinate =
								_camera.viewMatrix4 * PlatinumEngine::Maths::Vec4(crossPoint.x, crossPoint.y, crossPoint.z, 1.0f);

						//crossPointViewCoordinate = crossPointViewCoordinate/crossPointViewCoordinate.w;

						// This part check
						// check if the current cross point has the closest z value.
						// check if the current cross point is within clipping space.
						if( closestZValueForCrossPoint < crossPointViewCoordinate.z && crossPointViewCoordinate.z >= (float)-_far && crossPointViewCoordinate.z <= (float)-_near)
						{
							// update the selected game object
							currentSelectedGameObject = currentCheckingGameobject;

							// update the closest z value for cross point
							closestZValueForCrossPoint = crossPointViewCoordinate.z;
						}
					}
				}
			}
		}

		//-----------------//
		// Check children  //
		//-----------------//

		// Keep checking the children of currentChecking Gameobject
		// if children exist, call this function for every child
		for(int counter = 0; counter < currentCheckingGameobject->GetChildrenCount()>0; counter++)
		{
			currentSelectedGameObject = CheckRayTriangleIntersectionForGameobject(currentCheckingGameobject->GetChild(counter), inRay,
					inCameraPosition, currentSelectedGameObject, closestZValueForCrossPoint);
		}

		// return the selected game object
		return currentSelectedGameObject;
	}

}



