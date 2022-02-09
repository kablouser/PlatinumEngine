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
		static void ShowBasicScene();
		static void ShowMenuFile();
		static void ShowMenuGameObject();
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
