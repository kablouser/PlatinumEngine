//
// Created by shawn on 2022/2/7.
//

#pragma once

#include <ComponentComposition/Camera.h>
#include <WindowManager/Filedialog.h>
#include <SceneManager/Scene.h>
#include <GameWindow/GameWindow.h>
#include <SceneEditor/SceneEditor.h>
#include <Inspector/InspectorWindow.h>
#include <Project/ProjectWindow.h>
#include <SceneManager/HierarchyWindow.h>
#include <Profiler/Profiler.h>
#include <Logger/Logger.h>
#include <TypeDatabase/TypeDatabase.h>
#include <AssetDatabase/AssetDatabase.h>

namespace PlatinumEngine
{
	class WindowManager
	{
	public:
		WindowManager();

		///-----------------------------------
		///Main menu bar functions
		///-----------------------------------
		//window section
		void ShowGUI();
		void ShowMenuFile();
		void ShowMenuGameObject();
        void ShowMenuEdit();
		void ShowMenuWindow();
		void SetUpMainMenu();

        //file section
        void LoadFile(bool* outIsOpen);
		void SaveFile(bool* outIsOpen);

        //shortcuts for main menu bars
		///-----------------------------------
		///sub menu item functions to open corresponding window
		///-----------------------------------
		void ShowWindowHierarchy(bool* outIsOpen);
		void ShowWindowInspector(bool* outIsOpen);
		void ShowWindowScene(bool* outIsOpen);
		void ShowWindowProject(bool* outIsOpen);
		void ShowWindowLogger(bool* outIsOpen);
		void ShowWindowProfiler(bool* outIsOpen);

		void ShowQuitMenu();

		// First call will popup a window confirming quit, second call will close glfw window
		void SuggestQuit();
	private:
		///-----------------------------------------------------------------------
		///bools in main menu window list
		///-----------------------------------------------------------------------
		bool _showWindowGame                 = true;
		bool _showWindowScene                = true;
		bool _showWindowInspector            = true;
		bool _showWindowHierarchy            = true;
		bool _showWindowProject              = true;
		bool _showWindowLogger				 = false;
		bool _showWindowProfiler			 = false;

		///-----------------------------------------------------------------------
		///bools in main menu file list
		///-----------------------------------------------------------------------
	    bool _showFileLoad          		 = false;
	    bool _showFileSave                   = false;

		bool _isQuitting = false;
	};
}
