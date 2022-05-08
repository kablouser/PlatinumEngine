
//
// Created by shawn on 2022/2/7.
//

#include <WindowManager/WindowManager.h>
#include <Physics/Physics.h>

#include <imgui.h>
#include <iostream>
#include <ImGuiFileDialog.h>

namespace PlatinumEngine
{
	WindowManager::WindowManager(GameWindow *gameWindow,
								SceneEditor *sceneEditor,
								HierarchyWindow *hierarchy,
								Logger *logger,
								InspectorWindow *inspector,
								Profiler *profiler,
								ProjectWindow *projectWindow,
								IDSystem& idSystem,
								TypeDatabase& typeDatabase,
								AssetDatabase& assetDatabase,
								Scene& scene,
								AssetHelper *assetHelper
								):
								_gameWindow(gameWindow),
								_sceneEditor(sceneEditor),
								_hierarchy(hierarchy),
								_logger(logger),
								_inspector(inspector),
								_profiler(profiler),
								_projectWindow(projectWindow),
								_idSystem(idSystem),
								_typeDatabase(typeDatabase),
								_assetDatabase(assetDatabase),
								_scene(scene),
								_assetHelper(assetHelper)
	{
	}

	///--------------------------------------------------------------------------
	/// this function will create a basic window when you open the Platinum Engine
	///--------------------------------------------------------------------------
	void WindowManager::ShowGUI(Scene &scene)
	{
		///-----------------------------------------------------------------------
		///ifs in main menu window list to call the function inside
		///-----------------------------------------------------------------------
		//window section
		_gameWindow->ShowGUIWindow(&_showWindowGame);
		if (_showWindowScene) 			ShowWindowScene(&_showWindowScene);
		if (_showWindowHierarchy) 		ShowWindowHierarchy(&_showWindowHierarchy, scene);
		if (_showWindowInspector) 		ShowWindowInspector(&_showWindowInspector, scene);
		if (_showWindowProject) 		ShowWindowProject(&_showWindowProject);
		if (_showWindowAnimation) 		ShowWindowAnimation(&_showWindowAnimation);
		if (_showWindowLight) 			ShowWindowLight(&_showWindowLight);
		if (_showWindowLogger)   		ShowWindowLogger(&_showWindowLogger);
		if(_showWindowProfiler) 		ShowWindowProfiler(&_showWindowProfiler);
		if (_showFileLoad) 				LoadFile(scene, &_showFileLoad);
		if (_showFileSave) 				SaveFile(scene, &_showFileSave);

		///-------------------------------------------------------------------
		/// set up the main menu bar
		///-------------------------------------------------------------------
		SetUpMainMenu(scene);
	}

	///--------------------------------------------------------------------------
	/// Set up the main menu for basic Window
	///--------------------------------------------------------------------------
	void WindowManager::SetUpMainMenu(Scene &scene)
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
				ShowMenuGameObject(scene);
				ImGui::EndMenu();
			}
			///---------------------------------------------------------------
			/// Window component
			///---------------------------------------------------------------
			if (ImGui::BeginMenu("Window"))
			{
				ShowMenuWindow(scene);
				ImGui::EndMenu();
			}
			///---------------------------------------------------------------
			/// Asset Database control
			///---------------------------------------------------------------
			if (ImGui::Button("Reload Asset Database"))
			{
				_assetDatabase.Update(_idSystem, _scene);
			}

			ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x - 130.f);


			///------------------------------------------------------------------
			/// This section is for main menu bar to control the game view play/pause/step
			///------------------------------------------------------------------

			if (_gameWindow->GetIsStarted())
			{
				if (ImGui::Button(ICON_FA_STOP "##StopGame"))
				{
					auto& physicsObjects = scene.physics.GetRigidBodies();
					for(auto& object: physicsObjects)
					{
						object.DeRef()->GetComponent<Transform>().DeRef()->localRotation = object.DeRef()->GetComponent<Transform>().DeRef()->initialRotation;
						object.DeRef()->GetComponent<Transform>().DeRef()->localPosition = object.DeRef()->GetComponent<Transform>().DeRef()->initialPosition;
					}
					_gameWindow->SetIsStarted(false);
					// isPaused is reset to false when game is stopped
					_gameWindow->isPaused = false;
				}
			}
			else
			{
				if (ImGui::Button(ICON_FA_PLAY "##PlayGame"))
				{
					_gameWindow->SetIsStarted(true);
					auto& physicsObjects = scene.physics.GetRigidBodies();
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
			if (_gameWindow->isPaused)
			{
				// When paused, cause pause button to look "hovered"
				ImGui::PushStyleColor(ImGuiCol_Button,ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
				if (ImGui::Button(ICON_FA_PAUSE "##Pause"))
					_gameWindow->isPaused = false;
				ImGui::PopStyleColor(1);
			}
			else
			{
				if (ImGui::Button(ICON_FA_PAUSE "##Pause"))
					_gameWindow->isPaused = true;
			}

			// Step does nothing when game is stopped. So disable it.
			ImGui::BeginDisabled(!_gameWindow->GetIsStarted());
			if (ImGui::Button(ICON_FA_FORWARD_STEP "##Step"))
				_gameWindow->Step();
			ImGui::EndDisabled();

			ImGui::EndMainMenuBar();
		}
	}

	///--------------------------------------------------------------------------
	/// This section is for main menu bar "file" part file dialog showing
	///--------------------------------------------------------------------------
	void WindowManager::LoadFile(Scene& scene, bool* outIsOpen)
	{
		auto [success, path] = PlatinumEngine::FileDialog::OpenDialog(
				outIsOpen,
				"LoadFile",
				"Load File",
				".scene",
				"Assets",
				"");
		if (success)
			scene.LoadFile(path);
	}
	void WindowManager::SaveFile(Scene& scene, bool* outIsOpen)
	{
		auto [success, path] = PlatinumEngine::FileDialog::OpenDialog(
				outIsOpen,
				"SaveFile",
				"Save File",
				".scene",
				"Assets",
				"Scene");
		if (success)
			scene.SaveFile(path);
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
	void WindowManager::ShowMenuGameObject(Scene &scene)
	{
		if (ImGui::MenuItem("Create Empty"))
		{
			scene.AddGameObject();
		}
		if (ImGui::BeginMenu("Create Game Object"))
		{
			if (ImGui::MenuItem(ICON_FA_CAMERA " Camera"))
			{
				auto obj = scene.AddGameObject("Camera");
				scene.AddComponent<Camera>(obj);
				scene.AddComponent<Transform>(obj);
				_sceneEditor->SetSelectedGameobject(obj);
			}
			if (ImGui::MenuItem("Light"))
			{}
			if (ImGui::MenuItem(ICON_FA_FIRE " Particle Effect"))
			{
				auto obj = scene.AddGameObject("Particle Effect");
				scene.AddComponent<ParticleEffect>(obj);
				scene.AddComponent<Transform>(obj);
				_sceneEditor->SetSelectedGameobject(obj);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("3D Object"))
		{
			if (ImGui::MenuItem("Cube"))
			{
				std::filesystem::path cubePath = "./Assets/Meshes/Cube.obj";
				auto [success, asset] = _assetHelper->GetAsset<Mesh>(cubePath.string());
				auto cube = scene.AddGameObject("Cube");
				scene.AddComponent<Transform>(cube);
				if(success)
				{
					scene.AddComponent<MeshRender>(cube);
					cube.DeRef()->GetComponent<MeshRender>().DeRef()->SetMesh(asset);
				}
				scene.AddComponent<BoxCollider>(cube);
			}

			if (ImGui::MenuItem("Sphere"))
			{
				std::filesystem::path spherePath = "./Assets/Meshes/Sphere4.obj";
				auto [success, asset] = _assetHelper->GetAsset<Mesh>(spherePath.string());
				auto sphere = scene.AddGameObject("Sphere");
				scene.AddComponent<Transform>(sphere);
				if(success)
				{
					scene.AddComponent<MeshRender>(sphere);
					sphere.DeRef()->GetComponent<MeshRender>().DeRef()->SetMesh(asset);
				}
				scene.AddComponent<SphereCollider>(sphere);
			}

			if (ImGui::MenuItem("Capsule"))
			{
				std::filesystem::path capsulePath = "./Assets/Meshes/Capsule.obj";
				auto [success, asset] = _assetHelper->GetAsset<Mesh>(capsulePath.string());
				auto capsule = scene.AddGameObject("Capsule");
				scene.AddComponent<Transform>(capsule);
				if(success)
				{
					scene.AddComponent<MeshRender>(capsule);
					capsule.DeRef()->GetComponent<MeshRender>().DeRef()->SetMesh(asset);
				}
				scene.AddComponent<CapsuleCollider>(capsule);
			}

			if (ImGui::MenuItem("Plane"))
			{
				std::filesystem::path planePath = "./Assets/Meshes/Quad.obj";
				auto [success, asset] = _assetHelper->GetAsset<Mesh>(planePath.string());
				auto plane = scene.AddGameObject("Plane");
				scene.AddComponent<Transform>(plane);
				if(success)
				{
					scene.AddComponent<MeshRender>(plane);
					plane.DeRef()->GetComponent<MeshRender>().DeRef()->SetMesh(asset);
				}

				scene.AddComponent<BoxCollider>(plane);
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

	void WindowManager::ShowMenuWindow(Scene &scene)
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
		_projectWindow->ShowGUIWindow(outIsOpen);
	}

	//Please implement Light Window below
	void WindowManager::ShowWindowLight(bool* outIsOpen)
	{
		//TODO:
	}

	//Please implement Scene Window below
	void WindowManager::ShowWindowScene(bool* outIsOpen)
	{
		_sceneEditor->ShowGUIWindow(outIsOpen);
	}

	//Please implement Inspector Window below
	void WindowManager::ShowWindowInspector(bool* outIsOpen, Scene &scene)
	{
		_inspector->ShowGUIWindow(outIsOpen, scene);
	}

	//Please implement Hierarchy Window below
	void WindowManager::ShowWindowHierarchy(bool* outIsOpen, Scene &scene)
	{
		_hierarchy->ShowGUIWindow(outIsOpen, scene);
	}

	void WindowManager::ShowWindowLogger(bool* outIsOpen)
	{
		_logger->ShowGUIWindow(outIsOpen);
	}

	void WindowManager::ShowWindowProfiler(bool* outIsOpen)
	{
		_profiler->ShowGUIWindow(outIsOpen);
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