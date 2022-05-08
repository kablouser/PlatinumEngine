#pragma once

#include <InputManager/InputManager.h>
#include <Renderer/Renderer.h>
#include <WindowManager/WindowManager.h>
#include <TypeDatabase/TypeDatabase.h>
#include <SceneEditor/SceneEditor.h>
#include <SceneManager/HierarchyWindow.h>

namespace PlatinumEngine
{
	class Application
	{
	private:
		// sets Application::Instance. Because of constructor ordering is weird.
		class InstanceSetter
		{
		public:
			InstanceSetter(Application* newInstance);
			~InstanceSetter();
		};

		// set the instance before other things start
		InstanceSetter _instanceSetter;

	public:
		// singleton instance
		static Application* Instance;

		Time time;
		Physics physics;
		Profiler profiler;
		// typeDatabase before idSystem, and everything that uses type reflection
		TypeDatabase typeDatabase;
		IDSystem idSystem;
		AssetDatabase assetDatabase;
		Logger logger;
		InputManager inputManager;
		Renderer renderer;
		Scene scene;
		SceneEditor sceneEditor;
		HierarchyWindow hierarchyWindow;
		InspectorWindow inspectorWindow;
		GameWindow gameWindow;
		ProjectWindow projectWindow;
		WindowManager windowManager;

		Application();
		~Application();

		// Move copy or move allowed
		Application(Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&) = delete;
		Application& operator=(Application&&) = delete;
	};
}