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
		void Create();
		void Delete();
		void Listen();
	};

	class Window
	{
	public:
		virtual void ShowBasicScene();
	};

	class MainWindow: Window
	{
	public:
		virtual void ShowBasicScene() override;
		static void showMenuFile();
		static void showMenuGameObject();
	};
}
#endif //PLATINUMENGINE_WINDOWMANAGER_H
