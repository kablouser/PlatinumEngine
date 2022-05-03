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
								Scene *scene
								):
								_gameWindow(gameWindow),
								_sceneEditor(sceneEditor),
								_hierarchy(hierarchy),
								_logger(logger),
								_inspector(inspector),
								_profiler(profiler),
								_projectWindow(projectWindow),
								_scene(scene)
	{
	}

	/// this is a bool parameter used for disable or enable the pause and step button
	static bool enablePauseButton = true;
	static bool enableStepButton = false;
	///--------------------------------------------------------------------------
	/// this function will create a basic window when you open the Platinum Engine
	///--------------------------------------------------------------------------
	void WindowManager::ShowGUI(Scene &scene)
	{
		///-----------------------------------------------------------------------
		///ifs in main menu window list to call the function inside
		///-----------------------------------------------------------------------
		//window section
		if (_showWindowGame) 			ShowWindowGame(&_showWindowGame);
		if (_showWindowScene) 			ShowWindowScene(&_showWindowScene);
		if (_showWindowHierarchy) 		ShowWindowHierarchy(&_showWindowHierarchy, scene);
		if (_showWindowInspector) 		ShowWindowInspector(&_showWindowInspector, scene);
		if (_showWindowProject) 		ShowWindowProject(&_showWindowProject);
		if (_showWindowAnimation) 		ShowWindowAnimation(&_showWindowAnimation);
		if (_showWindowLight) 			ShowWindowLight(&_showWindowLight);
		if (_showWindowLogger)   		ShowWindowLogger(&_showWindowLogger);
		if(_showWindowProfiler) 		ShowWindowProfiler(&_showWindowProfiler);
		if (_showFileLoad) 				LoadFile();
		if (_showFileSave) 				SaveFile();

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

			ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x - 130.f);


			///------------------------------------------------------------------
			/// This section is for main menu bar to control the game view play/pause/step
			///------------------------------------------------------------------

			if (ImGui::Button(_start ? ICON_FA_STOP "##Play###startButton" : ICON_FA_PLAY "##Stop###startButton"))
			{
				if(!_start)
				{
					_start = true;
					_gameWindow->isPaused = false;
					_scene->Start();
				}
				else if(_start)
				{
					_start = false;
					_gameWindow->isPaused = true;
					_scene->End();
				}

				enablePauseButton = !enablePauseButton;
			}

  			// activate or inactive pause and step button
			ImGui::BeginDisabled(enablePauseButton);
			if (ImGui::Button(ICON_FA_PAUSE "##Pause"))
			{
				_gameWindow->isPaused = !_gameWindow->isPaused;
			}

			if (ImGui::Button(ICON_FA_FORWARD_STEP "##Step"))
			{
				_gameWindow->Step();
			}

			ImGui::EndDisabled();

			ImGui::EndMainMenuBar();
		}
	}

	///--------------------------------------------------------------------------
	/// This section is for main menu bar "file" part file dialog showing
	///--------------------------------------------------------------------------
	void WindowManager::LoadFile()
	{
		std::string x;
		x = PlatinumEngine::FileDialog::LoadFile();
		std::cout << x << std::endl;
	}
	void WindowManager::SaveFile()
	{
		std::string x;
		x = PlatinumEngine::FileDialog::SaveFile();
		std::cout << x << std::endl;
	}

	///--------------------------------------------------------------------------
	/// this function helps to create a list of
	/// operations of "File" in the menu Bar
	///--------------------------------------------------------------------------
	void WindowManager::ShowMenuFile()
	{

		if (ImGui::MenuItem("Load", "", &_showFileLoad))
		{
			ImGuiFileDialog::Instance()->OpenDialog("LoadFileKey","Load File",".*",".");
		}
		if (ImGui::MenuItem("Save", "Ctrl+S", &_showFileSave))
		{
			ImGuiFileDialog::Instance()->OpenDialog("SaveFileKey","Save File",".*",".");
		}

		ImGui::Separator();

		if(ImGui::MenuItem("Quit", "Ctrl+Q"))
		{
			std::exit(0);
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
			{}
			if (ImGui::MenuItem("Sphere"))
			{}
			if (ImGui::MenuItem("Capsule"))
			{}
			if (ImGui::MenuItem("Plane"))
			{}
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

		if (ImGui::MenuItem("Animation", "", &_showWindowAnimation))
		{}
		if (ImGui::MenuItem(ICON_FA_GAMEPAD " Game", "", &_showWindowGame))
		{
			ShowWindowGame(&_showWindowGame);
		}
		if (ImGui::MenuItem(ICON_FA_BARS_STAGGERED " Hierarchy", "", &_showWindowHierarchy))
		{
			ShowWindowHierarchy(&_showWindowHierarchy, scene);
		}
		if (ImGui::MenuItem(ICON_FA_CIRCLE_INFO " Inspector", "", &_showWindowInspector))
		{
			ShowWindowInspector(&_showWindowInspector, scene);
		}
		if (ImGui::MenuItem("Lighting", "", &_showWindowLight))
		{}
		if (ImGui::MenuItem(ICON_FA_FOLDER " Project", "", &_showWindowProject))
		{
			ShowWindowProject(&_showWindowProject);
		}
		if (ImGui::MenuItem(ICON_FA_IMAGE " Scene", "", &_showWindowScene))
		{
			ShowWindowScene(&_showWindowScene);
		}

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

	//Please implement Game Window below
	void WindowManager::ShowWindowGame(bool* outIsOpen)
	{
		_gameWindow->ShowGuiWindow(outIsOpen);
	}

	//Please implement Scene Window below
	void WindowManager::ShowWindowScene(bool* outIsOpen)
	{
		//TODO:
		_sceneEditor->ShowGUIWindow(outIsOpen);
	}

	//Please implement Inspector Window below
	void WindowManager::ShowWindowInspector(bool* outIsOpen, Scene &scene)
	{
		//TODO:
		_inspector->ShowGUIWindow(outIsOpen, scene);
	}

	//Please implement Hierarchy Window below
	void WindowManager::ShowWindowHierarchy(bool* outIsOpen, Scene &scene)
	{
		//TODO:
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