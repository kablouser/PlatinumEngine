//
// Created by shawn on 2022/2/7.
//

#include <WindowManager/WindowManager.h>
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
								Profiler *profiler
								):
								_gameWindow(gameWindow),
								_sceneEditor(sceneEditor),
								_hierarchy(hierarchy),
								_logger(logger),
								_inspector(inspector),
								_profiler(profiler)
	{

	}
	/// this is a bool parameter used for disable or enable the pause and step button
	static bool enablePauseButton = true;
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
		if (_showWindowAudio) 			ShowWindowAudio(&_showWindowAudio);
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
			if (ImGui::Button(ICON_KI_CARET_RIGHT "##Play"))
			{
				_gameWindow->_onUpdate = !_gameWindow->_onUpdate;
				if(_gameWindow->_onUpdate)
				{
					enablePauseButton = !enablePauseButton;
				}
				else if(!_gameWindow->_onUpdate)
				{
					enablePauseButton = true;
				}
			}

  			// activate or inactive pause and step button
			ImGui::BeginDisabled(enablePauseButton);
			if (ImGui::Button(ICON_KI_PAUSE "##Pause"))
			{
				_pause = !_pause;
				_gameWindow->Pause(_pause);
			}

			if (ImGui::Button(ICON_KI_STEP_FORWARD "##Step"))
			{
				if(!_pause)
					_pause = true;
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
		if (ImGui::BeginMenu("Effect"))
		{
			if (ImGui::MenuItem("Particle System"))
			{}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Camera"))
		{}
		if (ImGui::MenuItem("Light"))
		{}
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
		if (ImGui::MenuItem("Game", "Ctrl+1", &_showWindowGame))
		{
			ShowWindowGame(&_showWindowGame);
		}
		if (ImGui::MenuItem("Hierarchy", "Ctrl+2", &_showWindowHierarchy))
		{
			ShowWindowHierarchy(&_showWindowHierarchy, scene);
		}
		if (ImGui::MenuItem("Inspector", "Ctrl+3", &_showWindowInspector))
		{
			ShowWindowInspector(&_showWindowInspector, scene);
		}
		if (ImGui::MenuItem("Project", "Ctrl+4", &_showWindowProject))
		{}
		if (ImGui::MenuItem("Scene", "Ctrl+5", &_showWindowScene))
		{
			ShowWindowScene(&_showWindowScene);
		}
		if (ImGui::MenuItem("Animation", "Ctrl+6", &_showWindowAnimation))
		{}
		if (ImGui::MenuItem("Audio", "Ctrl+7", &_showWindowAudio))
		{}
		if (ImGui::MenuItem("Lighting", "Ctrl+8", &_showWindowLight))
		{}
		ImGui::Separator();
		if(ImGui::BeginMenu("Analysis"))
		{
			if(ImGui::MenuItem("Profiler", "", &_showWindowProfiler))
			{
				ShowWindowProfiler(&_showWindowProfiler);
			}
			if(ImGui::MenuItem("Logger", "", &_showWindowLogger))
			{
				ShowWindowLogger(&_showWindowLogger);
			}
			ImGui::EndMenu();
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
		//TODO:
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
		_inspector->SetActiveGameObject(_sceneEditor->GetSelectedGameobject());
		_inspector->ShowGUIWindow(outIsOpen, scene);
	}

	//Please implement Audio Window below
	void WindowManager::ShowWindowAudio(bool* outIsOpen)
	{
		//TODO:
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