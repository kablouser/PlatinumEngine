//
// Created by Shihua on 02/03/2022.
//

// imgui library
#include <imgui.h>

// Platinum library
#include "ComponentComposition/Objects.h"
#include <SceneEditor/SceneEditor.h>
#include <ComponentComposition/Transform.h>
#include <ComponentComposition/MeshRender.h>


namespace PlatinumEngine{

	// ___CONSTRUCTOR___


	SceneEditor::SceneEditor(InputManager* inputManager, Scene* scene, Renderer* renderer,AssetHelper* assetHelper, Time* time, Physics* physics):
			_ifCameraSettingWindowOpen(false),
			_camera(), _fov(60), _near(0.1), _far(10000),

			_inputManager(inputManager),
			_scene(scene),
			_renderer(renderer),
			_time(time),
			_physics(physics),

			_renderTexture(),

			_selectedGameobject(nullptr),

			_mouseMoveDelta(0, 0) ,
			_mouseButtonType(-1),
			_wheelValueDelta(0),

			_useSnap(false),
			_boundSizing(false),
			_boundSizingSnap(false),
			_onObjectGizmo(true),

			_currentGizmoMode(ImGuizmo::LOCAL),
			_currentGizmoOperation(ImGuizmo::TRANSLATE),

			_snap{ 1.f, 1.f, 1.f },
			_bounds{-0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f},
			_boundsSnap{ 0.1f, 0.1f, 0.1f },

			_skyboxTexture(),
			_skyBoxShaderInput(),
			_transparency(1.0),

			_enableGrid(true),
			_enableSkyBox(true),
			_xGrid(false), _yGrid(true), _zGrid(false),

			_assetHelper(assetHelper)
	{

		// Setup skybox texture

		_skyboxTexture.CreateCubeMap({"./Assets/Texture/Left_X.png",
									   "./Assets/Texture/Right_X.png",
									   "./Assets/Texture/Up_Y.png",
									   "./Assets/Texture/Bottom_Y.png",
									   "./Assets/Texture/Front_Z.png",
									   "./Assets/Texture/Back_Z.png"});
		// Setup skybox texture shader input
		CreateSkyBoxShaderInput();


		// Setup Grid shader input
		CreateGridShaderInput();

		// Setup input manager
		_inputManager->CreateAxis(std::string ("HorizontalAxisForEditorCamera"), GLFW_KEY_RIGHT, GLFW_KEY_LEFT, InputManager::AxisType::keyboardMouseButton);
		_inputManager->CreateAxis(std::string ("VerticalAxisForEditorCamera"), GLFW_KEY_UP, GLFW_KEY_DOWN, InputManager::AxisType::keyboardMouseButton);

		// Setup frame buffer
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
		if (ImGui::Begin(ICON_FA_IMAGE " Scene Editor", outIsOpen))
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

			if (ImGui::Button(ICON_FA_VIDEO "##Camera Setting"))
			{
				_ifCameraSettingWindowOpen = !_ifCameraSettingWindowOpen;
			}
			if(ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("Scene Camera");
			}
			//translate, rotate, scale button
			ImGui::SameLine();
			if (_currentGizmoOperation != ImGuizmo::SCALE)
			{
				if (ImGui::Button(_currentGizmoMode == ImGuizmo::LOCAL ? ICON_FA_CIRCLE_NODES "##Local###ViewMode" : ICON_FA_EARTH_ASIA "##Global###ViewMode"))
				{
					if (_currentGizmoMode == ImGuizmo::LOCAL)
					{
						_currentGizmoMode = ImGuizmo::WORLD;
					}
					else if (_currentGizmoMode == ImGuizmo::WORLD)
					{
						_currentGizmoMode = ImGuizmo::LOCAL;
					}
				}
				if (ImGui::IsItemHovered())
				{
					if(_currentGizmoMode == ImGuizmo::LOCAL)
						ImGui::SetTooltip("Local gizmo");
					if(_currentGizmoMode == ImGuizmo::WORLD)
						ImGui::SetTooltip("Global gizmo");
				}

			}
			else if(_currentGizmoOperation == ImGuizmo::SCALE)
			{
				ImGui::Button( ICON_FA_CIRCLE_NODES "##Local###ViewMode");
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip("Local Only Under Scale");
				}
			}

			ImGui::SameLine();
			if (ImGui::Button(ICON_FA_ARROWS_UP_DOWN_LEFT_RIGHT "##Translate"))
				_currentGizmoOperation = ImGuizmo::TRANSLATE;
			if(ImGui::IsItemHovered())
				ImGui::SetTooltip("Translate");

			ImGui::SameLine();
			if (ImGui::Button(ICON_FA_ROTATE "##Rotate"))
				_currentGizmoOperation = ImGuizmo::ROTATE;
			if(ImGui::IsItemHovered())
				ImGui::SetTooltip("Rotate");

			ImGui::SameLine();
			if (ImGui::Button(ICON_FA_MAXIMIZE "##Scale"))
				_currentGizmoOperation = ImGuizmo::SCALE;
			if(ImGui::IsItemHovered())
				ImGui::SetTooltip("Scale");

			ImGui::SameLine();
			if (ImGui::Button( ICON_FA_SLIDERS "##Universal"))
				_currentGizmoOperation = ImGuizmo::UNIVERSAL;
			if(ImGui::IsItemHovered())
				ImGui::SetTooltip("translate, rotate and scale");
			ImGui::SameLine();

			if(ImGui::Checkbox("##turn_on_off_gizmo", &_onObjectGizmo)) {}
			if(ImGui::IsItemHovered())
				ImGui::SetTooltip("turn on or off the gizmo");
			ImGui::SameLine();
			// grid and skybox
			if (ImGui::BeginPopupContextItem("grid"))
			{
				if (ImGui::Selectable("X Grid", &_xGrid))
				{
					_yGrid = false;
					_zGrid = false;
				}
				if (ImGui::Selectable("Y Grid", &_yGrid))
				{
					_xGrid = false;
					_zGrid = false;
				}
				if (ImGui::Selectable("Z Grid", &_zGrid))
				{
					_xGrid = false;
					_yGrid = false;
				}
				ImGui::Text("Opacity");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(70.f);
				ImGui::SliderFloat("##Opacity", &_transparency, 0.0f, 1.0f, "%.2f");
				ImGui::EndPopup();
			}

			if(ImGui::Button(_transparency ? ICON_FA_BORDER_ALL "##enable###gridTransparency" : ICON_FA_BORDER_NONE "##disable###gridTransparency"))
			{
				_enableGrid = !_enableGrid;
			}
			if(ImGui::IsItemHovered())
				ImGui::SetTooltip("turn on or off the grid");

			ImGui::SameLine(260.f);
			if(ImGui::Button(ICON_FA_CARET_DOWN "##GridDetail"))
			{
				ImGui::OpenPopup("grid");
			}

			ImGui::SameLine();
			if(ImGui::Button(ICON_FA_CLOUD "##skybox"))
			{
				_enableSkyBox = !_enableSkyBox;
			}
			if(ImGui::IsItemHovered())
				ImGui::SetTooltip("turn on or off the skybox");
			ImGui::Checkbox("Bound Sizing", &_boundSizing);
			//-------------
			// Sub window
			//-------------

			// Boolean to check if the projection matrix is update
			bool isProjectionUpdated = false;

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

							isProjectionUpdated = true;
						}

						// widgets
						if(ImGui::InputFloat("Near", &_near, 16) || ImGui::InputFloat("Far", &_far, 16))
						{
							isProjectionUpdated = true;
						}
					}
					else
					{
						if (ImGui::Button("Perspective"))
						{
							_camera.isOrthogonal = !_camera.isOrthogonal;

							isProjectionUpdated = true;
						}

						// widgets
						if(ImGui::SliderInt("Field of View", &_fov, 4, 179) || ImGui::InputFloat("Near", &_near, 16)||ImGui::InputFloat("Far", &_far, 16))
						{
							isProjectionUpdated = true;
						}

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

			if(ImGui::BeginChild("##renderingWindow",ImVec2(targetSize.x, targetSize.y), false,ImGuiWindowFlags_NoMove))
			{
				//------------------
				// Render Data
				//------------------
				Render(targetSize, isProjectionUpdated, _currentGizmoMode, _currentGizmoOperation);
			}
			ImGui::EndChild();
			if(ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RegularFilePathPayload"))
				{
					char* payloadPointer = (char*)payload->Data;
					int size = payload->DataSize;
					std::string filePath = "";
					for(int i=0;i<size;i++)
						filePath+=*(payloadPointer+i);
					std::filesystem::path payloadPath = std::filesystem::path(filePath);
					if(payloadPath.extension()==".obj")
					{
						GameObject* go = _scene->AddGameObject(payloadPath.stem().string());
						_scene->AddComponent<Transform>(go);
						_scene->AddComponent<MeshRender>(go);
						//Now we set the mesh
						auto asset_Helper = _assetHelper->GetMeshAsset(payloadPath.string());
						if (std::get<0>(asset_Helper))
							go->GetComponent<MeshRender>()->SetMesh(std::get<1>(asset_Helper));
						_selectedGameobject = go;
					}
				}
				ImGui::EndDragDropTarget();
			}
		}


		ImGui::End();

	}



	void SceneEditor::Render(ImVec2 targetSize, bool IsProjectionUpdated, ImGuizmo::MODE currentGizmoMode, ImGuizmo::OPERATION currentGizmoOperation)
	{

		//--------------------------------
		// update mouse && keyboard input
		//--------------------------------
		_mouseButtonType = _inputManager->GetMouseDown();
		_mouseMoveDelta  = _inputManager->GetMouseMoveVector();
		_wheelValueDelta = _inputManager->GetMouseWheelDeltaValue();

		//--------------------------------------
		// check if mouse is inside the screen
		//--------------------------------------

		if (   _inputManager->GetMousePosition().x <= ImGui::GetWindowContentRegionMax().x
			&& _inputManager->GetMousePosition().x >= ImGui::GetWindowContentRegionMin().x
			&& _inputManager->GetMousePosition().y <= ImGui::GetWindowContentRegionMax().y
			&& _inputManager->GetMousePosition().y >= ImGui::GetWindowContentRegionMin().y
			&& ImGui::IsWindowFocused()
			&& !ImGuizmo::IsUsing())
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

		// check if there is any keyboard input
		if (_inputManager->IsKeyPressed(GLFW_KEY_UP) || _inputManager->IsKeyPressed(GLFW_KEY_DOWN) ||
			_inputManager->IsKeyPressed(GLFW_KEY_LEFT) || _inputManager->IsKeyPressed(GLFW_KEY_RIGHT))
			_camera.TranslationByKeyBoard(_inputManager->GetAxis("VerticalAxisForEditorCamera"), _inputManager->GetAxis("HorizontalAxisForEditorCamera"));



		//---------------------------
		// Render projection matrix
		//---------------------------

		// check camera type
		if(!_camera.CheckIfProjectionMatrixUsed() || IsProjectionUpdated || _framebufferHeight != targetSize.y || _framebufferWidth != targetSize.x)
		{
			if (_camera.isOrthogonal)
			{
				// update as orthogonal projection matrix
				_camera.SetOrthogonalMatrix(
						-targetSize.x / (2.f * _camera.viewPortSize),
						targetSize.x / (2.f * _camera.viewPortSize),
						-targetSize.y / (2.f * _camera.viewPortSize),
						targetSize.y / (2.f* _camera.viewPortSize),
						(float)_near, (float)_far);
			}
			else
			{
				// update as perspective projection matrix
				_camera.SetPerspectiveMatrix((float)_fov,
						targetSize.x / targetSize.y,
						(float)_near, (float)_far);
			}
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

			// bind framebuffer
			_renderTexture.Bind();

			// initiate setting before rendering
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glViewport(0, 0, _framebufferWidth, _framebufferHeight);
			glClearColor(0.2784f, 0.2784f, 0.2784f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


			// ---------------- Render SKY BOX ---------------- //
			glDisable(GL_DEPTH_TEST);
			// discard the depth of the skybox
			if(_enableSkyBox)
			{
				glDepthMask(false);
				_renderer->BeginSkyBoxShader();

				// matrix for rescaling the skybox based on the near panel distance
				Maths::Mat4 scaleMatrix;
				scaleMatrix.SetScaleMatrix(
						Maths::Vec3(((float)_near * 2.f), ((float)_near * 2.f), ((float)_near * 2.f)));
				// set matrix uniform
				_renderer->SetViewMatrixSkyBox(Maths::Inverse(_camera.GetRotationOnlyViewMatrix()) * scaleMatrix);
				_renderer->SetProjectionMatrixSkyBox(_camera.projectionMatrix4);
				_skyboxTexture.BindCubeMap();
				_skyBoxShaderInput.Draw();
				_renderer->EndSkyBoxShader();

				// enable depth test for the later rendering
				glDepthMask(true);
			}

			// ------------- Render Game Objects ------------- //
			// Start rendering (bind a shader)
			_renderer->AnimationBegin();

			// Update rendering information to renderer
			_renderer->SetModelMatrixAnimation();

			// check if the view matrix is passed to shader
			_renderer->SetViewMatrixAnimation(_camera.viewMatrix4);
			_renderer->SetProjectionMatrixAnimation(_camera.projectionMatrix4);
			_renderer->SetLightPropertiesAnimation();

			// Render game objects
			_scene->Render(*_renderer);

			// End rendering (unbind a shader)
			_renderer->AnimationEnd();

			// -------------------- Render GRID ------------------ //
			if(_enableGrid)
			{
				_renderer->BeginGrid();
				_renderer->SetViewMatrixForGridShader(_camera.viewMatrix4);
				_renderer->SetProjectionMatrixForGridShader(_camera.projectionMatrix4);
				_renderer->SetFarValueForGridShader((float)_far);
				_renderer->SetNearValueForGridShader((float)_near);
				_renderer->SetTransparencyForGridShader(_transparency);

				if (_xGrid)
				{
					_renderer->SetGridAxisForGridShader(0);
				}
				else if (_zGrid)
				{
					_renderer->SetGridAxisForGridShader(2);
				}
				else
				{
					_renderer->SetGridAxisForGridShader(1);
				}

				_gridShaderInput.Draw();
				_renderer->EndGrid();
			}
			glEnable(GL_DEPTH_TEST);
			// ------------- Render Game Objects ------------- //
			// Start rendering (bind a shader)
			_renderer->Begin();

			// Bind skybox for objects to sample from, use an unused texture
			glActiveTexture(GL_TEXTURE7);
			_skyboxTexture.BindCubeMap();
			glActiveTexture(GL_TEXTURE0);

			// Update rendering information to renderer
			_renderer->SetModelMatrix();

			// check if the view matrix is passed to shader

			// TODO: Commneted these lines of code out to fix some bug, need to check how to do properly
			// if(!_camera.CheckIfViewMatrixUsed())
			{
				_renderer->SetViewMatrix(_camera.viewMatrix4);
				_camera.MarkViewMatrixAsUsed();
			}
			// if(!_camera.CheckIfProjectionMatrixUsed())
			{
				_renderer->SetProjectionMatrix(_camera.projectionMatrix4);
				_camera.MarkProjectionMatrixAsUsed();
			}
      
			_renderer->SetLightProperties();
			_renderer->SetCameraPos(_camera.GetCameraPosition());

			// Render game objects
			_scene->Render(*_renderer);

			// End rendering (unbind a shader)
			_renderer->End();

			// unbind framebuffer
			_renderTexture.Unbind();

			// reset setting after rendering
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_BLEND);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);


			// display updated framebuffer
			ImGui::Image(_renderTexture.GetColorTexture().GetImGuiHandle(), targetSize, ImVec2(0, 1), ImVec2(1, 0));


			// display gizmos
			UseGizmo(_camera.viewMatrix4.matrix, _camera.projectionMatrix4.matrix, currentGizmoMode, currentGizmoOperation, _onObjectGizmo);

			// update the camera quaternion, because it was rotated in the UseGizmo function
			_camera.UpdateCameraQuaternion();
		}

	}

	///--------------------------------------------
	/// scene Hierarchy and scene Editor linking helper function
	///--------------------------------------------
	void SceneEditor::SelectGameObjectFromScene()
	{
		// get mouse click position
		ImVec2 mouseClickedPosition = _inputManager->GetMousePosition();

		// since the display window use coordinate system which is inverse vertically,
		// we have to inverse the y value
		mouseClickedPosition.y = (float)_framebufferHeight - mouseClickedPosition.y;

		// check which object is clicked
		if(GameObject* result = FindClickedObject(mouseClickedPosition);
				result != nullptr)
		{
			_selectedGameobject = result;
		}
	}


	GameObject* SceneEditor::FindClickedObject(ImVec2& clickedPosition)
	{
		//---------------------------------------//
		// Calculate ray (only for perspective)  //
		//---------------------------------------//
		Maths::Vec3 ray(0.f, 0.f, 0.f);

		// check projection type
		if(!_camera.isOrthogonal)
		{

			PlatinumEngine::Maths::Vec3 worldCoordinateForClickedPosition = ConvertScreenCoordinateToWorldCoordinate(
					{ clickedPosition.x, clickedPosition.y });

			// Calculate ray
			ray = worldCoordinateForClickedPosition - _camera.GetCameraPosition();
		}


		//----------------------------------------//
		// Loop through Every root game object    //
		//----------------------------------------//

		// variable that store the current selected game object
		GameObject* currentSelectedGameobject = nullptr;

		// value that store the closest z value
		float closestZValue = (float)_far;

		// loop through all the root game object from scene class
		unsigned int numberOfRootGameObject = _scene->GetRootGameObjectsCount();

		for(int gameObjectIndex =0; gameObjectIndex < numberOfRootGameObject; gameObjectIndex++)
		{
			// get current game object
			GameObject* currentGameobject = _scene->GetRootGameObject(gameObjectIndex);

			// check if the object enable
			if(currentGameobject->IsEnabledInHierarchy())
			{
				// check whether this game object or one of its children is selected
				if(_camera.isOrthogonal!=true)
					currentSelectedGameobject = UpdateSelectedGameObject(currentGameobject, ray,
							_camera.GetCameraPosition(), currentSelectedGameobject, closestZValue);
				else
					currentSelectedGameobject = UpdateSelectedGameObject(currentGameobject, currentSelectedGameobject,
							Maths::Vec2{ clickedPosition.x, clickedPosition.y }, closestZValue);
			}
		}

		// return the final selected game object
		return currentSelectedGameobject;
	}

	// for triangle mesh only
	GameObject* SceneEditor::UpdateSelectedGameObject(GameObject* currentCheckingGameobject, PlatinumEngine::Maths::Vec3 inRay,
			const PlatinumEngine::Maths::Vec3& inCameraPosition, GameObject* currentSelectedGameObject, float& closestZValueForCrossPoint)
	{

		// check if the game object enable
		if(!currentCheckingGameobject->IsEnabledInHierarchy())
			return currentSelectedGameObject;

		//------------------//
		// Do intersection  //
		//------------------//

		// check if the game object has mesh component
		if( auto renderComponent = currentCheckingGameobject->GetComponent<MeshRender>(); renderComponent != nullptr)
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
					// Get the world coordinate of the object (after transformation)  //
					//----------------------------------------------------------------//

					Maths::Vec3 vertex0, vertex1, vertex2;

					if(mesh->animation.isAnimationOn)
					{
						vertex0 = mesh->animation.animationVertex[mesh->indices[count + 0]].position;
						vertex1 = mesh->animation.animationVertex[mesh->indices[count + 1]].position;
						vertex2 = mesh->animation.animationVertex[mesh->indices[count + 2]].position;

					}
					else
					{
						vertex0 = mesh->vertices[mesh->indices[count + 0]].position;
						vertex1 = mesh->vertices[mesh->indices[count + 1]].position;
						vertex2 = mesh->vertices[mesh->indices[count + 2]].position;
					}

					// check if the game object has transformation components
					if (auto transformComponent = currentCheckingGameobject->GetComponent<TransformComponent>();
							transformComponent != nullptr)
					{
						Maths::Mat4 modelMatrix = transformComponent->GetLocalToWorldMatrix();
						Maths::Vec4 temporaryVertex;

						// get the world coordinate of vertex 0
						temporaryVertex = modelMatrix * Maths::Vec4(vertex0.x,vertex0.y,vertex0.z, 1.0f);
						temporaryVertex = temporaryVertex / temporaryVertex.w;
						vertex0 = PlatinumEngine::Maths::Vec3(temporaryVertex.x, temporaryVertex.y, temporaryVertex.z);

						// get the world coordinate of vertex 1
						temporaryVertex = modelMatrix * Maths::Vec4(vertex1.x,vertex1.y,vertex1.z, 1.0f);
						temporaryVertex = temporaryVertex / temporaryVertex.w;
						vertex1 = PlatinumEngine::Maths::Vec3(temporaryVertex.x, temporaryVertex.y, temporaryVertex.z);

						// get the world coordinate of vertex 2
						temporaryVertex = modelMatrix * Maths::Vec4(vertex2.x,vertex2.y,vertex2.z, 1.0f);
						temporaryVertex = temporaryVertex / temporaryVertex.w;
						vertex2 = PlatinumEngine::Maths::Vec3(temporaryVertex.x, temporaryVertex.y, temporaryVertex.z);
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

					float x, y, z;

					if(DoInterpolationCheck(
							{
									Maths::Vec2{ vertex0PCS.x, vertex0PCS.y} ,
									Maths::Vec2{ vertex1PCS.x, vertex1PCS.y},
								 	Maths::Vec2{ vertex2PCS.x, vertex2PCS.y}
									 },
							{ crossPointPCS.x, crossPointPCS.y }, x, y, z))
					{
						// transform the object coordinate into view coordinate
						PlatinumEngine::Maths::Vec4 crossPointViewCoordinate =
								_camera.viewMatrix4 *
								PlatinumEngine::Maths::Vec4(crossPoint.x, crossPoint.y, crossPoint.z, 1.0f);

						// This part check:
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

		//-----------------//
		// Check children  //
		//-----------------//

		// Keep checking the children of currentChecking Gameobject
		// if children exist, call this function for every child
		for(int counter = 0; counter < currentCheckingGameobject->GetChildrenCount(); counter++)
		{
			currentSelectedGameObject = UpdateSelectedGameObject(currentCheckingGameobject->GetChild(counter), inRay,
					inCameraPosition, currentSelectedGameObject, closestZValueForCrossPoint);
		}

		// return the selected game object
		return currentSelectedGameObject;
	}

	bool SceneEditor::DoInterpolationCheck(std::array<Maths::Vec2,3> triangle, Maths::Vec2 point, float& x, float& y, float& z)
	{
		// Do barycentric interpolation to check
		Maths::Vec3 xValueOfTriangle((triangle[0] - triangle[1]).x, (triangle[0] - triangle[2]).x,
				(point - triangle[0]).x);
		float lengthOfxValueOfTriangle = Maths::Length(xValueOfTriangle);

		Maths::Vec3 yValueOfTriangle((triangle[0] - triangle[1]).y, (triangle[0] - triangle[2]).y,
				(point - triangle[0]).y);
		float lengthOfyValueOfTriangle = Maths::Length(yValueOfTriangle);

		// if the length of the axis is 0, the primitive is not a valid triangle primitive
		if (lengthOfxValueOfTriangle == 0 || lengthOfyValueOfTriangle == 0)
		{
			//PLATINUM_ERROR("You are clicking on an object with invalid triangle primitive.");
			return false;
		}

		// normalize the vector
		xValueOfTriangle = xValueOfTriangle / lengthOfxValueOfTriangle;
		yValueOfTriangle = yValueOfTriangle / lengthOfyValueOfTriangle;

		// Use the features of cross product to calculate the coefficients for interpolation
		Maths::Vec3 coefficientForInterpolation = Maths::Cross(xValueOfTriangle, yValueOfTriangle);

		// make sure the z is not zero
		if (coefficientForInterpolation.z != 0)
		{
			x = 1.f - ((coefficientForInterpolation.x + coefficientForInterpolation.y) / coefficientForInterpolation.z);

			y = coefficientForInterpolation.x / coefficientForInterpolation.z;

			z = coefficientForInterpolation.y / coefficientForInterpolation.z;

			// check if the final coefficients for interpolation are all valid
			if (x > 0.f && y > 0.f && z> 0.f)
			{
				return true;
			}
		}

		return false;
	}

	Maths::Vec3 SceneEditor::ConvertWorldCoordinateToScreenCoordinate(const Maths::Vec3& coordinate)
	{
		Maths::Vec4 coordinateV4(coordinate.x, coordinate.y, coordinate.z, 1.0f);

		coordinateV4 = _camera.viewMatrix4 * coordinateV4;

		coordinateV4 = _camera.projectionMatrix4 * coordinateV4;

		coordinateV4 = coordinateV4/coordinateV4.w;

		return {(coordinateV4.x + 1)/2.0f * (float)_framebufferWidth, (coordinateV4.y + 1)/2.0f * (float)_framebufferHeight, coordinateV4.z};
	}

	Maths::Vec3 SceneEditor::ConvertScreenCoordinateToWorldCoordinate(const Maths::Vec2& coordinate)
	{
		// turning screen coordinate 2d back to the near panel (of the frustum) coordinate 2d
		float nearPanelCoordinateForClickedPosition_x =(coordinate.x *2.f / (float)_framebufferWidth)- 1.f;
		float nearPanelCoordinateForClickedPosition_y =(coordinate.y * 2.f / (float)_framebufferHeight)- 1.f;

		// Clipping space
		PlatinumEngine::Maths::Vec4 coordinateV4(nearPanelCoordinateForClickedPosition_x,
				nearPanelCoordinateForClickedPosition_y, 1.0f, 1.0f);

		// View space
		coordinateV4 = PlatinumEngine::Maths::Inverse(_camera.projectionMatrix4) * coordinateV4;

		// World space
		coordinateV4 = PlatinumEngine::Maths::Inverse(_camera.viewMatrix4) * coordinateV4;

		return {coordinateV4.x/coordinateV4.w,
				coordinateV4.y/coordinateV4.w,
				coordinateV4.z/coordinateV4.w};
	}

	GameObject* SceneEditor::UpdateSelectedGameObject(GameObject* currentCheckingGameobject, GameObject* currentSelectedGameObject, Maths::Vec2 clickedPosition, float& closestZValueForCrossPoint)
	{
		// check if the game object enable
		if(!currentCheckingGameobject->IsEnabledInHierarchy())
			return currentSelectedGameObject;

		//------------------//
		// Do intersection  //
		//------------------//

		// check if the game object has mesh component
		if( auto renderComponent = currentCheckingGameobject->GetComponent<MeshRender>(); renderComponent != nullptr)
		{
			// fetch mesh
			Mesh* mesh = renderComponent->GetMesh();

			// Make sure there is mesh
			if (mesh != nullptr)
			{
				// loop all the vertices
				for (int count = 0; count < mesh->indices.size(); count += 3)
				{
					//----------------------------------------------------------------//
					// Get the world coordinate of the object (after transformation)  //
					//----------------------------------------------------------------//

					// vertices
					Maths::Vec3 vertex0, vertex1, vertex2;

					// check if the game object has transformation components
					if (auto transformComponent = currentCheckingGameobject->GetComponent<Transform>();
							transformComponent != nullptr)
					{
						Maths::Mat4 modelMatrix = transformComponent->GetLocalToWorldMatrix();


						Maths::Vec4 temporaryMatrix;

						// get the world coordinate of vertex 0
						temporaryMatrix = modelMatrix *
										  Maths::Vec4(mesh->vertices[mesh->indices[count + 0]].position.x,
												  mesh->vertices[mesh->indices[count + 0]].position.y,
												  mesh->vertices[mesh->indices[count + 0]].position.z, 1.0f);
						temporaryMatrix = temporaryMatrix / temporaryMatrix.w;
						vertex0 = PlatinumEngine::Maths::Vec3(temporaryMatrix.x, temporaryMatrix.y, temporaryMatrix.z);

						// get the world coordinate of vertex 1
						temporaryMatrix = modelMatrix *
										  Maths::Vec4(mesh->vertices[mesh->indices[count + 1]].position.x,
												  mesh->vertices[mesh->indices[count + 1]].position.y,
												  mesh->vertices[mesh->indices[count + 1]].position.z, 1.0f);
						temporaryMatrix = temporaryMatrix / temporaryMatrix.w;
						vertex1 = PlatinumEngine::Maths::Vec3(temporaryMatrix.x, temporaryMatrix.y, temporaryMatrix.z);

						// get the world coordinate of vertex 2
						temporaryMatrix = modelMatrix *
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

					//------------------------------------------------------//
					// Convert the world coordinate into screen coordinate  //
					//------------------------------------------------------//
					vertex0 = ConvertWorldCoordinateToScreenCoordinate(vertex0);
					vertex1 = ConvertWorldCoordinateToScreenCoordinate(vertex1);
					vertex2 = ConvertWorldCoordinateToScreenCoordinate(vertex2);

					//----------------------------------------------------------------------------//
					// Do interpolation to check if the clicked position is inside the triangle   //
					//----------------------------------------------------------------------------//
					float x, y, z;
					if(DoInterpolationCheck({Maths::Vec2{vertex0.x, vertex0.y},
											 Maths::Vec2{ vertex1.x, vertex1.y },
											 Maths::Vec2{ vertex2.x, vertex2.y }}, clickedPosition, x, y, z))
					{
						float zValueForClickedPoint = x * vertex0.z + y * vertex1.z + z * vertex2.z;

						// This part check:
						// check if the current cross point has the closest z value.
						// check if the current cross point is within clipping space.
						if (closestZValueForCrossPoint > zValueForClickedPoint)
						{
							// update the selected game object
							currentSelectedGameObject = currentCheckingGameobject;

							// update the closest z value for cross point
							closestZValueForCrossPoint = zValueForClickedPoint;
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
			currentSelectedGameObject = UpdateSelectedGameObject(currentCheckingGameobject->GetChild(counter),
					currentSelectedGameObject, clickedPosition, closestZValueForCrossPoint);
		}

		return currentSelectedGameObject;
	}

	void SceneEditor::SetSelectedGameobject(GameObject* inGameObject)
	{
		_selectedGameobject = inGameObject;
	}

	void SceneEditor::DeleteSelectedGameObject()
	{
		_scene->RemoveGameObject(*_selectedGameobject);
		_selectedGameobject = nullptr;
	}

	GameObject* SceneEditor::GetSelectedGameobject()
	{
		return _selectedGameobject;
	}


	///--------------------------------------------
	/// Gizmo
	///--------------------------------------------
	void SceneEditor::UseGizmo(float* cameraView, float* cameraProjection, ImGuizmo::MODE currentGizmoMode, ImGuizmo::OPERATION currentGizmoOperation, bool onGizmo)
	{
		Maths::Mat4 identityMatrix(1);

		ImGuizmo::SetDrawlist();
		auto windowWidth = (float)ImGui::GetWindowWidth();
		auto windowHeight = (float)ImGui::GetWindowHeight();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
		float viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
		float viewManipulateTop = ImGui::GetWindowPos().y;

		if(onGizmo)
		{
			if (_selectedGameobject != nullptr && _selectedGameobject->GetComponent<Transform>() != nullptr)
			{
				auto transform = _selectedGameobject->GetComponent<Transform>();
				Maths::Mat4 transform_matrix = transform->GetLocalToWorldMatrix();

				ImGuizmo::Manipulate(
						cameraView, cameraProjection, currentGizmoOperation, currentGizmoMode,
						transform_matrix.matrix, NULL, _useSnap ? &_snap[0] : NULL,
						_boundSizing ? _bounds : NULL, _boundSizingSnap ? _boundsSnap : NULL);

				transform->SetLocalToWorldMatrix(transform_matrix);
			}
		}
		// view manipulate gizmo
		ImGuizmo::ViewManipulate(cameraView, 0.001, ImVec2(viewManipulateRight - 100, viewManipulateTop),
				ImVec2(100, 100), 0x10101010);

	}


	///--------------------------------------------
	/// SkyBox
	///--------------------------------------------
	void SceneEditor::CreateSkyBoxShaderInput()
	{

		std::vector<Vertex> skyboxVertices = {
				{Maths::Vec3(-1.0f,  1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(-1.0f, -1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f, -1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f, -1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f,  1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(-1.0f,  1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},

				{Maths::Vec3(-1.0f, -1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(-1.0f, -1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(-1.0f,  1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(-1.0f,  1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(-1.0f,  1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(-1.0f, -1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},

				{Maths::Vec3(1.0f, -1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f, -1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f,  1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f,  1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f,  1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f, -1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},

				{Maths::Vec3(-1.0f, -1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(-1.0f,  1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f,  1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f,  1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f, -1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(-1.0f, -1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},

				{Maths::Vec3(-1.0f,  1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f,  1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f,  1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f,  1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(-1.0f,  1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(-1.0f,  1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},

				{Maths::Vec3(-1.0f, -1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(-1.0f, -1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f, -1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f, -1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(-1.0f, -1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f, -1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
		};

		std::vector<GLuint> indices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
									   16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
									   30, 31, 32, 33, 34, 35};


		_skyBoxShaderInput.Set(skyboxVertices, indices);
	}

	void SceneEditor::CreateGridShaderInput()
	{
		std::vector<Vertex> vertices = {Vertex{{1, 1, 0}, {0.0, 0.0, 0.0}, {0.0, 0.0 } },
										Vertex{{-1, -1, 0}, {0.0, 0.0, 0.0}, {0.0, 0.0 } },
										Vertex{{-1, 1, 0}, {0.0, 0.0, 0.0}, {0.0, 0.0 } },
										Vertex{{1, -1, 0}, {0.0, 0.0, 0.0}, {0.0, 0.0 } }};
		std::vector<GLuint> indices = {0, 1, 2, 1, 0, 3};

		_gridShaderInput.Set(vertices, indices);
	}
}



