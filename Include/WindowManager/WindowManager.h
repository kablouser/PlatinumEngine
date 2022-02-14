//
// Created by shawn on 2022/2/7.
//

#ifndef PLATINUMENGINE_WINDOWMANAGER_H
#define PLATINUMENGINE_WINDOWMANAGER_H

namespace PlatinumEngine
{
	class WindowManager
	{
	public:
		///-----------------------------------
		///Main menu bar functions
		///-----------------------------------
		//window section
		void ShowBasicScene();
		void ShowMenuFile();
		void ShowMenuGameObject();
        void ShowMenuEdit();
		void SetUpMainMenu();

        //file section
        void DrawSaveScene(bool* p_open);
        void DrawOpenScene(bool* p_open);

        //shortcuts for main menu bars
		void DoShortCut(sf::Event e);
		///-----------------------------------
		///sub menu item functions to open corresponding window
		///-----------------------------------
		static void ShowWindowGame(bool* p_open);
		static void ShowWindowHierarchy(bool* p_open);
		static void ShowWindowInspector(bool* p_open);
		static void ShowWindowScene(bool* p_open);
		static void ShowWindowProject(bool* p_open);
		static void ShowWindowAnimation(bool* p_open);
		static void ShowWindowLight(bool* p_open);
		static void ShowWindowAudio(bool* p_open);

	public:
		///-----------------------------------------------------------------------
		///bools in main menu window list
		///-----------------------------------------------------------------------
		bool show_mainMenu_window_game                 = false;
		bool show_mainMenu_window_scene                = false;
		bool show_mainMenu_window_inspector            = false;
		bool show_mainMenu_window_hierarchy            = false;
		bool show_mainMenu_window_project              = false;
		bool show_mainMenu_window_animation            = false;
		bool show_mainMenu_window_audio                = false;
		bool show_mainMenu_window_light                = false;

		///-----------------------------------------------------------------------
		///bools in main menu GameObject list
		///-----------------------------------------------------------------------
		bool show_mainMenu_gameObject_empty            = false;
		bool show_mainMenu_gameObject_emptyChild       = false;
		bool show_mainMenu_gameObject_emptyParent      = false;
		bool show_mainMenu_gameObject_object_cube      = false;
		bool show_mainMenu_gameObject_object_sphere    = false;
		bool show_mainMenu_gameObject_object_plane     = false;
		bool show_mainMenu_gameObject_object_capsule   = false;
		bool show_mainMenu_gameObject_effect_particle  = false;
		bool show_mainMenu_gameObject_camera           = false;
		bool show_mainMenu_gameObject_light            = false;

		///-----------------------------------------------------------------------
		///bools in main menu file list
		///-----------------------------------------------------------------------
	    bool show_mainMenu_file_newScene               = false;
	    bool show_mainMenu_file_openScene              = false;
	    bool show_mainMenu_file_save                   = false;
	    bool show_mainMenu_file_saveAs                 = false;

	};

	/*
	class Window
	{
	public:
		virtual void showBasicScene();
	};

	class MainWindow: Window
	{
	public:
		virtual void showBasicScene() override;
		static void showMenuFile();
		static void showMenuGameObject();
		static void showPanel();
	};
	 */
}
#endif //PLATINUMENGINE_WINDOWMANAGER_H
