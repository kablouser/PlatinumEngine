//
// Created by shawn on 2022/2/7.
//

#pragma once
#include <WindowManager/Filedialog.h>
#include <SceneManager/Scene.h>

namespace PlatinumEngine
{
	class WindowManager
	{
	public:
		WindowManager() = default;

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
        void LoadFile();
        void SaveFile();

        //shortcuts for main menu bars
//		void DoShortCut(sf::Event event);
		///-----------------------------------
		///sub menu item functions to open corresponding window
		///-----------------------------------
		static void ShowWindowGame(bool* outIsOpen);
		static void ShowWindowHierarchy(bool* outIsOpen);
		static void ShowWindowInspector(bool* outIsOpen);
		static void ShowWindowScene(bool* outIsOpen);
		static void ShowWindowProject(bool* outIsOpen);
		static void ShowWindowAnimation(bool* outIsOpen);
		static void ShowWindowLight(bool* outIsOpen);
		static void ShowWindowAudio(bool* outIsOpen);

	public:
		///-----------------------------------------------------------------------
		///bools in main menu window list
		///-----------------------------------------------------------------------
		bool _showWindowGame                 = false;
		bool _showWindowScene                = false;
		bool _showWindowInspector            = false;
		bool _showWindowHierarchy            = false;
		bool _showWindowProject              = false;
		bool _showWindowAnimation            = false;
		bool _showWindowAudio                = false;
		bool _showWindowLight                = false;

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
	};
}
