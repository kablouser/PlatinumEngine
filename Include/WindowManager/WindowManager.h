//
// Created by shawn on 2022/2/7.
//

#pragma once
#include <WindowManager/Filedialog.h>
#include <SceneManager/Scene.h>
#include <GameWindow/GameWindow.h>
#include <SceneEditor/SceneEditor.h>
#include <Inspector/InspectorWindow.h>
#include <SceneManager/HierarchyWindow.h>
#include <Logger/Logger.h>


namespace PlatinumEngine
{
	class WindowManager
	{
	public:
		WindowManager(GameWindow* gameWindow,
				      SceneEditor *_sceneEditor,
					  HierarchyWindow *_hierarchy,
					  Logger *_logger,
					  InspectorWindow *_inspector,
					  Scene& scene);

		///-----------------------------------
		///Main menu bar functions
		///-----------------------------------
		//window section
		void ShowGUI(Scene &scene);
		void ShowMenuFile();
		void ShowMenuGameObject(Scene &scene);
        void ShowMenuEdit();
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
		void ShowWindowInspector(bool* outIsOpen);
		void ShowWindowScene(bool* outIsOpen);
		void ShowWindowProject(bool* outIsOpen);
		void ShowWindowAnimation(bool* outIsOpen);
		void ShowWindowLight(bool* outIsOpen);
		void ShowWindowAudio(bool* outIsOpen);
		void ShowLogger(bool* outIsOpen);

	private:
		///-----------------------------------------------------------------------
		///bools in main menu window list
		///-----------------------------------------------------------------------
		bool _showWindowGame                 = true;
		bool _showWindowScene                = true;
		bool _showWindowInspector            = true;
		bool _showWindowHierarchy            = true;
		bool _showWindowProject              = false;
		bool _showWindowAnimation            = false;
		bool _showWindowAudio                = false;
		bool _showWindowLight                = false;
		bool _showLogger					 = true;

		///-----------------------------------------------------------------------
		///bools in main menu GameObject list
		///-----------------------------------------------------------------------
		bool _showGameObjectEmpty            = false;
		bool _showGameObjectEmptyChild       = false;
		bool _showGameObjectEmptyParent      = false;
		bool _showGameObjectCube      		 = false;
		bool _showGameObjectSphere  		 = false;
		bool _showGameObjectPlane   		 = false;
		bool _showGameObjectCapsule   		 = false;
		bool _showGameObjectParticleEffect   = false;
		bool _showGameObjectCamera           = false;
		bool _showGameObjectLight            = false;

		///-----------------------------------------------------------------------
		///bools in main menu file list
		///-----------------------------------------------------------------------
	    bool _showFileLoad          		 = false;
	    bool _showFileSave                   = false;

        ///-----------------------------------------------------------------------
		///bools in main menu that controls the play/pause/step function of GameWindow
		///-----------------------------------------------------------------------
		bool _pause							 = true;
		bool _step							 = false;


	private:
		///-----------------------------------------------------------------------
		///pointers to the constructor of each window need to be shown
		///-----------------------------------------------------------------------
		GameWindow *_gameWindow;
		SceneEditor *_sceneEditor;
		HierarchyWindow *_hierarchy;
		Logger *_logger;
		InspectorWindow *_inspector;
		Scene& _scene;

	};
}
