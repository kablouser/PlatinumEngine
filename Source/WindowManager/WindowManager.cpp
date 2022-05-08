
//
// Created by shawn on 2022/2/7.
//

#include <WindowManager/WindowManager.h>

#include <imgui.h>
#include <iostream>
#include <ImGuiFileDialog.h>
#include <Application.h>
#include <AssetDatabase/AssetHelper.h>
#include <SceneManager/SceneWithTemplates.h>

namespace PlatinumEngine
{
	WindowManager::WindowManager() = default;

	///--------------------------------------------------------------------------
	/// this function will create a basic window when you open the Platinum Engine
	///--------------------------------------------------------------------------
	void WindowManager::ShowGUI()
	{
		///-----------------------------------------------------------------------
		///ifs in main menu window list to call the function inside
		///-----------------------------------------------------------------------
		//window section
		Application::Instance->gameWindow.ShowGUIWindow(&_showWindowGame);
		if (_showWindowScene) 			ShowWindowScene(&_showWindowScene);
		if (_showWindowHierarchy) 		ShowWindowHierarchy(&_showWindowHierarchy);
		if (_showWindowInspector) 		ShowWindowInspector(&_showWindowInspector);
		if (_showWindowProject) 		ShowWindowProject(&_showWindowProject);
		if (_showWindowAnimation) 		ShowWindowAnimation(&_showWindowAnimation);
		if (_showWindowLight) 			ShowWindowLight(&_showWindowLight);
		if (_showWindowLogger)   		ShowWindowLogger(&_showWindowLogger);
		if(_showWindowProfiler) 		ShowWindowProfiler(&_showWindowProfiler);
		if (_showFileLoad) 				LoadFile(&_showFileLoad);
		if (_showFileSave) 				SaveFile(&_showFileSave);

		///-------------------------------------------------------------------
		/// set up the main menu bar
		///-------------------------------------------------------------------
		SetUpMainMenu();
	}

	///--------------------------------------------------------------------------
	/// Set up the main menu for basic Window
	///--------------------------------------------------------------------------
	void WindowManager::SetUpMainMenu()
	{
		if (ImGui::BeginMainMenuBar())
		{
			///---------------------------------------------------------------
			/// File component
			///---------------------------------------------------------------
			if (ImGui::BeginMenu("File"))
			{
				ShowMenuFile();
				ImGui::EndMenu();
			}
			///---------------------------------------------------------------
			/// File component
			///---------------------------------------------------------------
			if (ImGui::BeginMenu("Edit"))
			{
				ShowMenuEdit();
				ImGui::EndMenu();
			}
			///---------------------------------------------------------------
			/// GameObject component
			///---------------------------------------------------------------
			if (ImGui::BeginMenu("GameObject"))
			{
				ShowMenuGameObject();
				ImGui::EndMenu();
			}
			///---------------------------------------------------------------
			/// Window component
			///---------------------------------------------------------------
			if (ImGui::BeginMenu("Window"))
			{
				ShowMenuWindow();
				ImGui::EndMenu();
			}
			///---------------------------------------------------------------
			/// Asset Database control
			///---------------------------------------------------------------
			if (ImGui::Button("Reload Asset Database"))
			{
				Application::Instance->assetDatabase.Update(Application::Instance->idSystem, Application::Instance->scene);
			}

			ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x - 130.f);


			///------------------------------------------------------------------
			/// This section is for main menu bar to control the game view play/pause/step
			///------------------------------------------------------------------

			if (Application::Instance->gameWindow.GetIsStarted())
			{
				if (ImGui::Button(ICON_FA_STOP "##StopGame"))
				{
					auto& physicsObjects = Application::Instance->physics.GetRigidBodies();
					for(auto& object: physicsObjects)
					{
						object.DeRef()->GetComponent<Transform>().DeRef()->localRotation = object.DeRef()->GetComponent<Transform>().DeRef()->initialRotation;
						object.DeRef()->GetComponent<Transform>().DeRef()->localPosition = object.DeRef()->GetComponent<Transform>().DeRef()->initialPosition;
					}
					Application::Instance->gameWindow.SetIsStarted(false);
					// isPaused is reset to false when game is stopped
					Application::Instance->gameWindow.isPaused = false;
				}
			}
			else
			{
				if (ImGui::Button(ICON_FA_PLAY "##PlayGame"))
				{
					Application::Instance->gameWindow.SetIsStarted(true);
					auto& physicsObjects = Application::Instance->physics.GetRigidBodies();
					for(auto& object: physicsObjects)
					{
						object.DeRef()->GetComponent<Transform>().DeRef()->initialRotation = object.DeRef()->GetComponent<Transform>().DeRef()->localRotation;
						object.DeRef()->GetComponent<Transform>().DeRef()->initialPosition = object.DeRef()->GetComponent<Transform>().DeRef()->localPosition;
						object.DeRef()->Reposition(object.DeRef()->GetComponent<Transform>().DeRef()->initialPosition,
								object.DeRef()->GetComponent<Transform>().DeRef()->initialRotation);
					}
				}
			}

			// Pause can be useful before the game starts for debugging
			if (Application::Instance->gameWindow.isPaused)
			{
				// When paused, cause pause button to look "hovered"
				ImGui::PushStyleColor(ImGuiCol_Button,ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
				if (ImGui::Button(ICON_FA_PAUSE "##Pause"))
					Application::Instance->gameWindow.isPaused = false;
				ImGui::PopStyleColor(1);
			}
			else
			{
				if (ImGui::Button(ICON_FA_PAUSE "##Pause"))
					Application::Instance->gameWindow.isPaused = true;
			}

			// Step does nothing when game is stopped. So disable it.
			ImGui::BeginDisabled(!Application::Instance->gameWindow.GetIsStarted());
			if (ImGui::Button(ICON_FA_FORWARD_STEP "##Step"))
				Application::Instance->gameWindow.Step();
			ImGui::EndDisabled();

			ImGui::EndMainMenuBar();
		}
	}

	///--------------------------------------------------------------------------
	/// This section is for main menu bar "file" part file dialog showing
	///--------------------------------------------------------------------------
	void WindowManager::LoadFile(bool* outIsOpen)
	{
		auto [success, path] = PlatinumEngine::FileDialog::OpenDialog(
				outIsOpen,
				"LoadFile",
				"Load File",
				".scene",
				"Assets",
				"");
		if (success)
			Application::Instance->scene.LoadFile(path);
	}
	void WindowManager::SaveFile(bool* outIsOpen)
	{
		auto [success, path] = PlatinumEngine::FileDialog::OpenDialog(
				outIsOpen,
				"SaveFile",
				"Save File",
				".scene",
				"Assets",
				"Scene");
		if (success)
			Application::Instance->scene.SaveFile(path);
	}

	///--------------------------------------------------------------------------
	/// this function helps to create a list of
	/// operations of "File" in the menu Bar
	///--------------------------------------------------------------------------
	void WindowManager::ShowMenuFile()
	{
		ImGui::MenuItem("Load", "", &_showFileLoad);
		ImGui::MenuItem("Save", "Ctrl+S", &_showFileSave);

		ImGui::Separator();

		if(ImGui::MenuItem("Quit", "Ctrl+Q"))
		{
			glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
		}
	}

	///--------------------------------------------------------------------------
	/// this function helps to create a list of
	/// operations of "GameObject" in the menu Bar
	///--------------------------------------------------------------------------
	void WindowManager::ShowMenuGameObject()
	{
		if (ImGui::MenuItem("Create Empty"))
		{
			Application::Instance->scene.AddGameObject();
		}
		if (ImGui::BeginMenu("Create Game Object"))
		{
			if (ImGui::MenuItem(ICON_FA_CAMERA " Camera"))
			{
				auto obj = Application::Instance->scene.AddGameObject("Camera");
				Application::Instance->scene.AddComponent<Camera>(obj);
				Application::Instance->scene.AddComponent<Transform>(obj);
				Application::Instance->sceneEditor.SetSelectedGameobject(obj);
			}
			if (ImGui::MenuItem(ICON_FA_LIGHTBULB " Light"))
			{}
			if (ImGui::BeginMenu("Particle Effects"))
			{
				if (ImGui::MenuItem(ICON_FA_FIRE " Basic Effect"))
				{
					auto obj = Application::Instance->scene.AddGameObject("Particle Effect");
					Application::Instance->scene.AddComponent<ParticleEffect>(obj);
					Application::Instance->scene.AddComponent<Transform>(obj);
					Application::Instance->sceneEditor.SetSelectedGameobject(obj);
				}
				if (ImGui::MenuItem(ICON_FA_BURST " Single Point Effect"))
				{
					auto obj = Application::Instance->scene.AddGameObject("Particle Effect");
					auto component = Application::Instance->scene.AddComponent<ParticleEffect>(obj);
					component.DeRef()->particleEmitter.useRandomInitVelocityX = false;
					component.DeRef()->particleEmitter.useRandomInitVelocityZ = false;
					component.DeRef()->particleEmitter.initVelocity = {0.0f, 0.0f, 0.0f};
					component.DeRef()->particleEmitter.actingForce = {0.0f, 0.0f, 0.0f};
					Application::Instance->scene.AddComponent<Transform>(obj);
					Application::Instance->sceneEditor.SetSelectedGameobject(obj);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("3D Object"))
		{
			if (ImGui::MenuItem(ICON_FA_CUBE " Cube"))
			{
				std::filesystem::path cubePath = "./Assets/Meshes/Cube.obj";
				auto [success, asset] = AssetHelper::GetAsset<Mesh>(cubePath.string());
				auto cube = Application::Instance->scene.AddGameObject("Cube");
				Application::Instance->scene.AddComponent<Transform>(cube);
				if(success)
				{
					Application::Instance->scene.AddComponent<MeshRender>(cube);
					cube.DeRef()->GetComponent<MeshRender>().DeRef()->SetMesh(asset);
				}
				Application::Instance->scene.AddComponent<BoxCollider>(cube);
			}

			if (ImGui::MenuItem(ICON_FA_CIRCLE " Sphere"))
			{
				std::filesystem::path spherePath = "./Assets/Meshes/Sphere4.obj";
				auto [success, asset] = AssetHelper::GetAsset<Mesh>(spherePath.string());
				auto sphere = Application::Instance->scene.AddGameObject("Sphere");
				Application::Instance->scene.AddComponent<Transform>(sphere);
				if(success)
				{
					Application::Instance->scene.AddComponent<MeshRender>(sphere);
					sphere.DeRef()->GetComponent<MeshRender>().DeRef()->SetMesh(asset);
				}
				Application::Instance->scene.AddComponent<SphereCollider>(sphere);
			}

			if (ImGui::MenuItem(ICON_FA_CAPSULES " Capsule"))
			{
				std::filesystem::path capsulePath = "./Assets/Meshes/Capsule.obj";
				auto [success, asset] = AssetHelper::GetAsset<Mesh>(capsulePath.string());
				auto capsule = Application::Instance->scene.AddGameObject("Capsule");
				Application::Instance->scene.AddComponent<Transform>(capsule);
				if(success)
				{
					Application::Instance->scene.AddComponent<MeshRender>(capsule);
					capsule.DeRef()->GetComponent<MeshRender>().DeRef()->SetMesh(asset);
				}
				Application::Instance->scene.AddComponent<CapsuleCollider>(capsule);
			}

			if (ImGui::MenuItem(ICON_FA_SQUARE " Plane"))
			{
				std::filesystem::path planePath = "./Assets/Meshes/Quad.obj";
				auto [success, asset] = AssetHelper::GetAsset<Mesh>(planePath.string());
				auto plane = Application::Instance->scene.AddGameObject("Plane");
				Application::Instance->scene.AddComponent<Transform>(plane);
				if(success)
				{
					Application::Instance->scene.AddComponent<MeshRender>(plane);
					plane.DeRef()->GetComponent<MeshRender>().DeRef()->SetMesh(asset);
				}

				Application::Instance->scene.AddComponent<BoxCollider>(plane);
			}
			ImGui::EndMenu();
		}
	}

	///--------------------------------------------------------------------------
	/// this function helps to create a list of
	/// operations of "Edit" in the menu Bar
	///--------------------------------------------------------------------------
	void WindowManager::ShowMenuEdit()
	{
		if (ImGui::MenuItem("Undo", "Ctrl+Z"))
		{}
		if (ImGui::MenuItem("Redo", "Shift+Ctrl+Z"))
		{}
		if (ImGui::MenuItem("Cut", "Ctrl+X"))
		{}
		if (ImGui::MenuItem("Copy", "Ctrl+C"))
		{}
		if (ImGui::MenuItem("Paste", "Ctrl+V"))
		{}
	}

	void WindowManager::ShowMenuWindow()
	{
		if(ImGui::BeginMenu("Analysis"))
		{
			if(ImGui::MenuItem(ICON_FA_FILE_CIRCLE_EXCLAMATION " Logger", "", &_showWindowLogger))
			{
				ShowWindowLogger(&_showWindowLogger);
			}
			if(ImGui::MenuItem(ICON_FA_CHART_AREA " Profiler", "", &_showWindowProfiler))
			{
				ShowWindowProfiler(&_showWindowProfiler);
			}
			ImGui::EndMenu();
		}
		ImGui::Separator();

		// Do NOT call ShowWindowXYZ here. Do NOT call OnRenderGUI here.
		// They are called once already in the ShowGUI.
		ImGui::MenuItem("Animation", "", &_showWindowAnimation);
		ImGui::MenuItem(ICON_FA_GAMEPAD " Game", "", &_showWindowGame);
		ImGui::MenuItem(ICON_FA_BARS_STAGGERED " Hierarchy", "", &_showWindowHierarchy);
		ImGui::MenuItem(ICON_FA_CIRCLE_INFO " Inspector", "", &_showWindowInspector);
		ImGui::MenuItem("Lighting", "", &_showWindowLight);
		ImGui::MenuItem(ICON_FA_FOLDER " Project", "", &_showWindowProject);
		ImGui::MenuItem(ICON_FA_IMAGE " Scene", "", &_showWindowScene);
	}

	///--------------------------------------------------------------------------
	///   ---                                                               ---
	///   | Section: Please implement GUI in the corresponding function below |
	///   ---                                                               ---
	///--------------------------------------------------------------------------

	//Please implement Animation Window below
	void WindowManager::ShowWindowAnimation(bool* outIsOpen)
	{
		if (!ImGui::Begin("Animation", outIsOpen))
		{
			ImGui::Text("ABOUT THIS DEMO:");
			ImGui::End();
			return;
		}
		ImGui::End();
	}

	//Please implement Project Window below
	void WindowManager::ShowWindowProject(bool* outIsOpen)
	{
		Application::Instance->projectWindow.ShowGUIWindow(outIsOpen);
	}

	//Please implement Light Window below
	void WindowManager::ShowWindowLight(bool* outIsOpen)
	{
		//TODO:
	}

	//Please implement Scene Window below
	void WindowManager::ShowWindowScene(bool* outIsOpen)
	{
		Application::Instance->sceneEditor.ShowGUIWindow(outIsOpen);
	}

	//Please implement Inspector Window below
	void WindowManager::ShowWindowInspector(bool* outIsOpen)
	{
		Application::Instance->inspectorWindow.ShowGUIWindow(outIsOpen);
	}

	//Please implement Hierarchy Window below
	void WindowManager::ShowWindowHierarchy(bool* outIsOpen)
	{
		Application::Instance->hierarchyWindow.ShowGUIWindow(outIsOpen);
	}

	void WindowManager::ShowWindowLogger(bool* outIsOpen)
	{
		Application::Instance->logger.ShowGUIWindow(outIsOpen);
	}

	void WindowManager::ShowWindowProfiler(bool* outIsOpen)
	{
		Application::Instance->profiler.ShowGUIWindow(outIsOpen);
	}


	///--------------------------------------------------------------------------
	/// This section implements the short cuts for main menu bar
	///--------------------------------------------------------------------------
	// sfml event trigger for shortcuts
//	void WindowManager::DoShortCut(GLFWwindow* window)
//	{
//		switch (event.type)
//		{
//		case sf::Event::KeyPressed:
//            if(event.key.control && event.key.code == sf::Keyboard::S)
//            {
//				showFileSave = true;
//                break;
//            }
//            else if(event.key.control && event.key.code == sf::Keyboard::Num1)
//            {
//				_showWindowGame = true;
//                break;
//            }
//            else if(event.key.control && event.key.code == sf::Keyboard::Num2)
//            {
//				_showWindowHierarchy = true;
//                break;
//            }
//            else if(event.key.control && event.key.code == sf::Keyboard::Num3)
//            {
//				_showWindowInspector = true;
//                break;
//            }
//            else if(event.key.control && event.key.code == sf::Keyboard::Num4)
//            {
//				_showWindowProject = true;
//                break;
//            }
//            else if(event.key.control && event.key.code == sf::Keyboard::Num5)
//            {
//				_showWindowScene = true;
//                break;
//            }
//            else if(event.key.control && event.key.code == sf::Keyboard::Num6)
//            {
//				_showWindowAnimation = true;
//            break;
//            }
//            else if(event.key.control && event.key.code == sf::Keyboard::Num7)
//            {
//				_showWindowAudio = true;
//                break;
//            }
//            else if(event.key.control && event.key.code == sf::Keyboard::Num8)
//            {
//				_showWindowLight = true;
//                break;
//            }
//		}
//	}
}