//
// Created by shawn on 2022/2/7.
//

#pragma once

#include <ComponentComposition/CameraComponent.h>
#include <WindowManager/Filedialog.h>
#include <SceneManager/Scene.h>
#include <GameWindow/GameWindow.h>
#include <SceneEditor/SceneEditor.h>
#include <Inspector/InspectorWindow.h>
#include <Project/ProjectWindow.h>
#include <SceneManager/HierarchyWindow.h>
#include <Profiler/Profiler.h>
#include <Logger/Logger.h>

namespace PlatinumEngine
{
	class WindowManager
	{
	public:
		WindowManager(GameWindow *gameWindow,
				      SceneEditor *sceneEditor,
					  HierarchyWindow *hierarchy,
					  Logger *logger,
					  InspectorWindow *inspector,
					  Profiler *profiler,
					  ProjectWindow *projectWindow,
					  Scene *scene
					  );

		///-----------------------------------
		///Main menu bar functions
		///-----------------------------------
		//window section
		void ShowGUI(Scene &scene);
		void ShowMenuFile();
		void ShowMenuGameObject(Scene &scene);
        void ShowMenuEdit();
		void ShowMenuWindow(Scene &scene);
		void SetUpMainMenu(Scene &scene);

        //file section
        static void LoadFile();
        static void SaveFile();

        //shortcuts for main menu bars
		///-----------------------------------
		///sub menu item functions to open corresponding window
		///-----------------------------------
		void ShowWindowGame(bool* outIsOpen);
		void ShowWindowHierarchy(bool* outIsOpen, Scene &scene);
		void ShowWindowInspector(bool* outIsOpen, Scene &scene);
		void ShowWindowScene(bool* outIsOpen);
		void ShowWindowProject(bool* outIsOpen);
		void ShowWindowAnimation(bool* outIsOpen);
		void ShowWindowLight(bool* outIsOpen);
		void ShowWindowLogger(bool* outIsOpen);
		void ShowWindowProfiler(bool* outIsOpen);
	private:
		///-----------------------------------------------------------------------
		///bools in main menu window list
		///-----------------------------------------------------------------------
		bool _showWindowGame                 = true;
		bool _showWindowScene                = true;
		bool _showWindowInspector            = true;
		bool _showWindowHierarchy            = true;
		bool _showWindowProject              = true;
		bool _showWindowAnimation            = false;
		bool _showWindowLight                = false;
		bool _showWindowLogger				 = false;
		bool _showWindowProfiler			 = false;

		///-----------------------------------------------------------------------
		///bools in main menu file list
		///-----------------------------------------------------------------------
	    bool _showFileLoad          		 = false;
	    bool _showFileSave                   = false;

	private:
		///-----------------------------------------------------------------------
		///pointers to the constructor of each window need to be shown
		///-----------------------------------------------------------------------
		GameWindow *_gameWindow;
		SceneEditor *_sceneEditor;
		HierarchyWindow *_hierarchy;
		Logger *_logger;
		InspectorWindow *_inspector;
		Profiler *_profiler;
		ProjectWindow *_projectWindow;
		Scene *_scene;
	};
}
