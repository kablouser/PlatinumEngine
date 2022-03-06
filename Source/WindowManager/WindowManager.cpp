//
// Created by shawn on 2022/2/7.
//

#include <WindowManager/WindowManager.h>
#include <imgui.h>
#include <iostream>
#include <ImGuiFileDialog.h>

namespace PlatinumEngine
{
	///--------------------------------------------------------------------------
	/// this function will create a basic window when you open the Platinum Engine
	///--------------------------------------------------------------------------
	void WindowManager::ShowGUI()
	{
		///-----------------------------------------------------------------------
		///ifs in main menu window list to call the function inside
		///-----------------------------------------------------------------------
		//window section
		if (_showWindowGame) ShowWindowGame(&_showWindowGame);
		if (_showWindowScene) ShowWindowScene(&_showWindowScene);
		if (_showWindowHierarchy) ShowWindowHierarchy(&_showWindowHierarchy);
		if (_showWindowInspector) ShowWindowInspector(&_showWindowInspector);
		if (_showWindowProject) ShowWindowProject(&_showWindowProject);
		if (_showWindowAnimation) ShowWindowAnimation(&_showWindowAnimation);
		if (_showWindowAudio) ShowWindowAudio(&_showWindowAudio);
		if (_showWindowLight) ShowWindowLight(&_showWindowLight);

		if (showFileOpenScene) DrawOpenScene(&showFileOpenScene);
		if (showFileSave) DrawSaveScene(&showFileSave);

		///-------------------------------------------------------------------
		/// set up the main menu bar
		///-------------------------------------------------------------------
		SetUpMainMenu();
	}

	///--------------------------------------------------------------------------
	/// This section is for main menu bar "file" part file dialog showing
	///--------------------------------------------------------------------------
	void WindowManager::DrawSaveScene(bool* outIsOpen)
	{
		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Save Scene", ".scene", ".");

		// display
		if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
		{
			// action if OK
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
				std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
				// action
			}

			// close
			ImGuiFileDialog::Instance()->Close();
		}
	}

	void WindowManager::DrawOpenScene(bool* outIsOpen)
	{
		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Open Scene", ".scene", ".");

		// display
		if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
		{
			// action if OK
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
				std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
				// action
			}

			// close
			ImGuiFileDialog::Instance()->Close();
		}
	}

	///--------------------------------------------------------------------------
	/// this function helps to create a list of
	/// operations of "File" in the menu Bar
	///--------------------------------------------------------------------------
	void WindowManager::ShowMenuFile()
	{
		if (ImGui::MenuItem("New Scene"))
		{}
		if (ImGui::MenuItem("Open Scene", "", &showFileOpenScene))
		{}
		if (ImGui::MenuItem("Save", "Ctrl+S", &showFileSave))
		{}
		if (ImGui::MenuItem("Save as"))
		{}
	}

	///--------------------------------------------------------------------------
	/// this function helps to create a list of
	/// operations of "GameObject" in the menu Bar
	///--------------------------------------------------------------------------
	void WindowManager::ShowMenuGameObject()
	{
		if (ImGui::MenuItem("Create Empty"))
		{}
		if (ImGui::MenuItem("Create Empty Child"))
		{}
		if (ImGui::MenuItem("Create Empty Parent"))
		{}
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
				if (ImGui::MenuItem("Game", "Ctrl+1", &_showWindowGame))
				{}
				if (ImGui::MenuItem("Hierarchy", "Ctrl+2", &_showWindowHierarchy))
				{}
				if (ImGui::MenuItem("Inspector", "Ctrl+3", &_showWindowInspector))
				{}
				if (ImGui::MenuItem("Project", "Ctrl+4", &_showWindowProject))
				{}
				if (ImGui::MenuItem("Scene", "Ctrl+5", &_showWindowScene))
				{}
				if (ImGui::MenuItem("Animation", "Ctrl+6", &_showWindowAnimation))
				{}
				if (ImGui::MenuItem("Audio", "Ctrl+7", &_showWindowAudio))
				{}
				if (ImGui::MenuItem("Lighting", "Ctrl+8", &_showWindowLight))
				{}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
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
		//TODO:
	}

	//Please implement Scene Window below
	void WindowManager::ShowWindowScene(bool* outIsOpen)
	{
		//TODO:
	}

	//Please implement Inspector Window below
	void WindowManager::ShowWindowInspector(bool* outIsOpen)
	{
		//TODO:
	}

	//Please implement Audio Window below
	void WindowManager::ShowWindowAudio(bool* outIsOpen)
	{
		//TODO:
	}

	//Please implement Hierarchy Window below
	void WindowManager::ShowWindowHierarchy(bool* outIsOpen)
	{
		//TODO:
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