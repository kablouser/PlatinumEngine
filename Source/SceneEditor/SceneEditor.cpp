//
// Created by Shihua on 02/03/2022.
//

// imgui library
#include <imgui.h>

// Platinum library
#include <SceneEditor/SceneEditor.h>
#include <ComponentComposition/TransformComponent.h>
#include <ComponentComposition/RenderComponent.h>


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
			_currentGizmoOperation(ImGuizmo::TRANSLATE),

			_snap{ 1.f, 1.f, 1.f },
			_bounds{-0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f},
			_boundsSnap{ 0.1f, 0.1f, 0.1f },

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
				_currentGizmoOperation = ImGuizmo::TRANSLATE;
			if(ImGui::IsKeyPressed(GLFW_KEY_W))
				_currentGizmoOperation = ImGuizmo::ROTATE;
			if(ImGui::IsKeyPressed(GLFW_KEY_E))
				_currentGizmoOperation = ImGuizmo::SCALE;
			if(ImGui::IsKeyPressed(GLFW_KEY_R))
				_currentGizmoOperation = ImGuizmo::UNIVERSAL;
			//-----------
			// Widgets
			//-----------

			if (ImGui::Button("Camera Setting"))
			{

				_ifCameraSettingWindowOpen = !_ifCameraSettingWindowOpen;

			}
			//ImGuizmo
			ImGui::SameLine();
			if (ImGui::RadioButton("Translate", _currentGizmoOperation == ImGuizmo::TRANSLATE))
				_currentGizmoOperation = ImGuizmo::TRANSLATE;
			ImGui::SameLine();
			if (ImGui::RadioButton("Rotate", _currentGizmoOperation == ImGuizmo::ROTATE))
				_currentGizmoOperation = ImGuizmo::ROTATE;
			ImGui::SameLine();
			if (ImGui::RadioButton("Scale", _currentGizmoOperation == ImGuizmo::SCALE))
				_currentGizmoOperation = ImGuizmo::SCALE;
			ImGui::SameLine();
			if (ImGui::RadioButton("Universal", _currentGizmoOperation == ImGuizmo::UNIVERSAL))
				_currentGizmoOperation = ImGuizmo::UNIVERSAL;

			ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x - 130.f);

			if (_currentGizmoOperation != ImGuizmo::SCALE)
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
				Update(targetSize, _currentGizmoMode, _currentGizmoOperation);
			}
			ImGui::EndChild();
		}


		ImGui::End();

	}



	void SceneEditor::Update(ImVec2 targetSize, ImGuizmo::MODE currentGizmoMode, ImGuizmo::OPERATION currentGizmoOperation)
	{
		//--------------------------------------
		// check if mouse is inside the screen
		//--------------------------------------
	if(!ImGuizmo::IsUsing())
	{
		if (_inputManager->GetMousePosition().x <= targetSize.x
			&& _inputManager->GetMousePosition().x >= ImGui::GetWindowPos().x
			&& _inputManager->GetMousePosition().y <= targetSize.y
			&& _inputManager->GetMousePosition().y >= ImGui::GetWindowPos().y)
		{

			//---------------------
			// update view matrix
			//---------------------

			// check mouse click to do rotation and translation
			if (_mouseButtonType == 0)// translation (up down left right)
			{

				_camera.TranslationByMouse(Maths::Vec2(_mouseMoveDelta.x, _mouseMoveDelta.y));

			}
			else if (_mouseButtonType == 1) // for rotation
			{
				_camera.RotationByMouse(Maths::Vec2(_mouseMoveDelta.x, _mouseMoveDelta.y));
			}
			// check this is for moving camera closer/further
			if (_wheelValueDelta != 0)
			{
				_camera.TranslationByMouse(_wheelValueDelta);
			}

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

		//---------------------------------------------
		// check if the mouse is selecting game object
		//---------------------------------------------
		if(_inputManager->IsMouseDown(0))
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
			glClearColor(0.35f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			// Start rendering (bind a shader)
			_renderer->Begin();

			// Update rendering information to renderer
			_renderer->SetModelMatrix();
			_renderer->SetViewMatrix(_camera.viewMatrix4);
			_renderer->SetProjectionMatrix(_camera.projectionMatrix4);
			_renderer->SetLightProperties();

			// Render game objects
			_scene->Render(*_renderer);

			// End rendering (unbind a shader)
			_renderer->End();

			_renderTexture.Unbind();
			glDisable(GL_DEPTH_TEST);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);


			// display updated framebuffer
			ImGui::Image(_renderTexture.GetColorTexture().GetImGuiHandle(), targetSize, ImVec2(0, 1), ImVec2(1, 0));

			// display gizmos
			UseGizmo(_camera.viewMatrix4.matrix, _camera.projectionMatrix4.matrix, currentGizmoMode, currentGizmoOperation);
		}

	}


	void SceneEditor::SelectGameObjectFromScene()
	{
		ImVec2 mouseClickedPosition = _inputManager->GetMousePosition();

		// do ray casting
		if(GameObject* result = DoRayCasting(mouseClickedPosition);
				result != nullptr)
		{
			_selectedGameobject = result;
		}
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
		float closestZValue = (float)_far;

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
						_camera.GetCameraPosition(), currentSelectedGameobject, closestZValue);
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
			Mesh* mesh = renderComponent->GetMesh();

			// Make sure there is mesh
			if(mesh != nullptr)
			{
				// loop all the vertices
				for (int count = 0; count < mesh->indices.size(); count += 3)
				{
					//----------------------------------------------------------------//
					// Find the world coordinate of the object (after transformation) //
					//----------------------------------------------------------------//

					Maths::Vec3 vertex0, vertex1, vertex2;

					// check if the game object has transformation components
					if (auto transformComponent = currentCheckingGameobject->GetComponent<TransformComponent>();
							transformComponent != nullptr)
					{
						Maths::Mat4 translationMatrix, rotationMatrix, scaleMatrix;
						translationMatrix.SetTranslationMatrix(transformComponent->position);
						rotationMatrix.SetRotationMatrix(transformComponent->rotation.EulerAngles());
						scaleMatrix.SetScaleMatrix(transformComponent->scale);

						Maths::Vec4 temporaryMatrix;

						// get the world coordinate of vertex 0
						temporaryMatrix = rotationMatrix * translationMatrix * scaleMatrix *
										  Maths::Vec4(mesh->vertices[mesh->indices[count + 0]].position.x,
												  mesh->vertices[mesh->indices[count + 0]].position.y,
												  mesh->vertices[mesh->indices[count + 0]].position.z, 1.0f);
						temporaryMatrix = temporaryMatrix / temporaryMatrix.w;
						vertex0 = PlatinumEngine::Maths::Vec3(temporaryMatrix.x, temporaryMatrix.y, temporaryMatrix.z);

						// get the world coordinate of vertex 1
						temporaryMatrix = rotationMatrix * translationMatrix * scaleMatrix *
										  Maths::Vec4(mesh->vertices[mesh->indices[count + 1]].position.x,
												  mesh->vertices[mesh->indices[count + 1]].position.y,
												  mesh->vertices[mesh->indices[count + 1]].position.z, 1.0f);
						temporaryMatrix = temporaryMatrix / temporaryMatrix.w;
						vertex1 = PlatinumEngine::Maths::Vec3(temporaryMatrix.x, temporaryMatrix.y, temporaryMatrix.z);

						// get the world coordinate of vertex 2
						temporaryMatrix = rotationMatrix * translationMatrix * scaleMatrix *
										  Maths::Vec4(mesh->vertices[mesh->indices[count + 2]].position.x,
												  mesh->vertices[mesh->indices[count + 2]].position.y,
												  mesh->vertices[mesh->indices[count + 2]].position.z, 1.0f);
						temporaryMatrix = temporaryMatrix / temporaryMatrix.w;
						vertex2 = PlatinumEngine::Maths::Vec3(temporaryMatrix.x, temporaryMatrix.y, temporaryMatrix.z);
					}
					else
					{
						vertex0 = mesh->vertices[mesh->indices[count + 0]].position;
						vertex1 = mesh->vertices[mesh->indices[count + 1]].position;
						vertex2 = mesh->vertices[mesh->indices[count + 2]].position;
					}

					//----------------------------------//
					// Do ray and triangle intersection //
					//----------------------------------//

					// PCS: Planer Coordinate System
					// build the PCS using the triangle primitive as a panel that contains two axes (U and V).
					// and the other axis (N) is the normal of the panel.

					// origin of PCS
					Maths::Vec3 originForPCS = vertex0;

					// not normalized axis of PCS
					Maths::Vec3 axisUForPCS = vertex1 - originForPCS;
					Maths::Vec3 axisNForPCS = Cross(axisUForPCS, (vertex2 - originForPCS));
					Maths::Vec3 axisWForPCS = Cross(axisUForPCS, axisNForPCS);

					// length of axis
					float lengthU = Maths::Length(axisUForPCS);
					float lengthN = Maths::Length(axisNForPCS);
					float lengthW = Maths::Length(axisWForPCS);

					// if the length of the axis is 0, the primitive is not a valid triangle primitive
					if (lengthU == 0 || lengthN == 0 || lengthW == 0)
					{
						//PLATINUM_ERROR("You are clicking on an object with invalid triangle primitive.");
						break;
					}

					// if the length of the three axes are not 0, normalize them
					axisUForPCS = axisUForPCS / lengthU;
					axisNForPCS = axisNForPCS / lengthN;
					axisWForPCS = axisWForPCS / lengthW;


					// find cross point

					// 1. find the ratio between the ray (regard it as a limited line with a start point which is camera position) projection length
					// and line (a line that form by the origin of Planer Coordinate System and camera position) projection length
					float ratioForRayAndPointsDistance = 0;

					// project the Ray on the axis N of the Planer Coordinate System
					float lengthOfRayProjectionOnAxisN = Maths::Dot(inRay, axisNForPCS);

					// make sure the ray is not contained by the same panel (the situation when the dot product will be zero)
					if (lengthOfRayProjectionOnAxisN != 0)
					{
						// project the line between origin of Planer coordinate system and the camera position (the start point of the ray)
						// on the axis N of the Planer Coordinate System
						Maths::Vec3 vectorBetweenPCSOriginAndCameraPosition = originForPCS - inCameraPosition; // use this is because putting `originForPCS - inCameraPosition` directly into Maths::Dots() cause error
						float lengthOfPointsDistanceProjectionOnAxisN = Maths::Dot(vectorBetweenPCSOriginAndCameraPosition, axisNForPCS);

						// calculate the ratio between the length of the two projections
						ratioForRayAndPointsDistance = lengthOfPointsDistanceProjectionOnAxisN / lengthOfRayProjectionOnAxisN;

					}
					else
					{
						break;
					}

					// calculate the cross point by adding vector with the right ratio to the camera position (which is the start point of the Ray)
					Maths::Vec3 crossPoint = inCameraPosition + inRay * ratioForRayAndPointsDistance;

					//------------------------------------//
					// Do barycentric interpolation check //
					//------------------------------------//

					// Convert the cross point and the vertices of the triangle into PCS coordinate
					Maths::Mat3 PCSMatrix3x3({
							axisUForPCS.x, axisWForPCS.x, axisNForPCS.x,
							axisUForPCS.y, axisWForPCS.y, axisNForPCS.y,
							axisUForPCS.z, axisWForPCS.z, axisNForPCS.z
					});
					// 1. cross point
					Maths::Vec3 crossPointPCS = PCSMatrix3x3 * (crossPoint - originForPCS);

					// 2. triangle vertices
					Maths::Vec3 vertex0PCS = PCSMatrix3x3 * (vertex0 - originForPCS);
					Maths::Vec3 vertex1PCS = PCSMatrix3x3 * (vertex1 - originForPCS);
					Maths::Vec3 vertex2PCS = PCSMatrix3x3 * (vertex2 - originForPCS);

					// Do barycentric interpolation to check
					Maths::Vec3 xValueOfVerticesPCS((vertex0PCS - vertex1PCS).x, (vertex0PCS - vertex2PCS).x,
							(crossPointPCS - vertex0PCS).x);
					float lengthOfXValueOfVerticesPCS = Maths::Length(xValueOfVerticesPCS);

					Maths::Vec3 yValueOfVerticesPCS((vertex0PCS - vertex1PCS).y, (vertex0PCS - vertex2PCS).y,
							(crossPointPCS - vertex0PCS).y);
					float lengthOfYValueOfVerticesPCS = Maths::Length(yValueOfVerticesPCS);

					// if the length of the axis is 0, the primitive is not a valid triangle primitive
					if (lengthOfXValueOfVerticesPCS == 0 || lengthOfYValueOfVerticesPCS == 0)
					{
						//PLATINUM_ERROR("You are clicking on an object with invalid triangle primitive.");
						break;
					}

					xValueOfVerticesPCS = xValueOfVerticesPCS / lengthOfXValueOfVerticesPCS;
					yValueOfVerticesPCS = yValueOfVerticesPCS / lengthOfYValueOfVerticesPCS;

					// Use the features of cross product to calculate the coefficients for interpolation
					Maths::Vec3 coefficientForInterpolation = Maths::Cross(xValueOfVerticesPCS, yValueOfVerticesPCS);

					// make sure the z is not zero
					if (coefficientForInterpolation.z != 0)
					{
						// check if the final coefficients for interpolation are all valid
						if (1.f - ((coefficientForInterpolation.x + coefficientForInterpolation.y) /
								   coefficientForInterpolation.z) > 0.f
							&& coefficientForInterpolation.x / coefficientForInterpolation.z > 0.f
							&& coefficientForInterpolation.y / coefficientForInterpolation.z > 0.f)
						{

							//----------------------------------------------------//
							// Check whether to update the selected game object   //
							//----------------------------------------------------//

							// transform the object coordinate into view coordinate
							PlatinumEngine::Maths::Vec4 crossPointViewCoordinate =
									_camera.viewMatrix4 *
									PlatinumEngine::Maths::Vec4(crossPoint.x, crossPoint.y, crossPoint.z, 1.0f);

							//crossPointViewCoordinate = crossPointViewCoordinate/crossPointViewCoordinate.w;

							// This part check
							// check if the current cross point has the closest z value.
							// check if the current cross point is within clipping space.
							if (closestZValueForCrossPoint > crossPointViewCoordinate.z &&
								crossPointViewCoordinate.z <= (float)_far &&
								crossPointViewCoordinate.z >= (float)_near)
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
		}

		//-----------------//
		// Check children  //
		//-----------------//

		// Keep checking the children of currentChecking Gameobject
		// if children exist, call this function for every child
		for(int counter = 0; counter < currentCheckingGameobject->GetChildrenCount(); counter++)
		{
			currentSelectedGameObject = CheckRayTriangleIntersectionForGameobject(currentCheckingGameobject->GetChild(counter), inRay,
					inCameraPosition, currentSelectedGameObject, closestZValueForCrossPoint);
		}

		// return the selected game object
		return currentSelectedGameObject;
	}

	void SceneEditor::SetSelectedGameobject(GameObject* inGameObject)
	{
		_selectedGameobject = inGameObject;
	}

	GameObject* SceneEditor::GetSelectedGameobject()
	{
		return _selectedGameobject;
	}
}



