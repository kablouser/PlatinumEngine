#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <InputManager/InputManager.h>
#include <Inspector/InspectorWindow.h>
#include <Renderer/Renderer.h>
#include <WindowManager/WindowManager.h>
#include <SceneManager/HierarchyWindow.h>
#include <Logger/Logger.h>
#include <SceneEditor/SceneEditor.h>


#include <OpenGL/GLCheck.h>

static void GlfwErrorCallback(int error, const char* description)
{
	std::cerr << "Glfw Error " << error << ": " << description << std::endl;
}

#include <Helpers/VectorHelpers.h>

#include <iostream>
#include <vector>
#include <string>
#include <experimental/filesystem>

#include <cctype>
#include <random>
#include <limits>

using std::experimental::filesystem::recursive_directory_iterator;

class AssetsDatabase
{
public:

	typedef size_t AssetID;

	const std::uintmax_t BIG_FILE_THRESHOLD_BYTES = 1000000; // 1MB

	AssetsDatabase() :
		_generator(std::random_device()()),
		_anyNumber(std::numeric_limits<size_t>::min(), std::numeric_limits<size_t>::max())
	{
	}

	friend std::ostream& operator<<(std::ostream& output, AssetsDatabase& assetsDatabase)
	{
		for(auto i : assetsDatabase._assetIDToFilePath)
			output << i.first << ' ' << i.second << std::endl;
		return output;
	}

	friend std::istream& operator>>(std::istream& input, AssetsDatabase& assetsDatabase)
	{
		// enforce a empty object
		assetsDatabase._assetIDToFilePath.clear();
		assetsDatabase._filePathToAssetID.clear();

		AssetID id;
		std::string filePath;
		size_t line = 1;
		while (input >> id && input.get() && std::getline(input, filePath))
		{
			auto insertID = assetsDatabase._assetIDToFilePath.insert({id, filePath});
			// if inserted ID successfully
			if(insertID.second)
			{
				auto insertFilePath = assetsDatabase._filePathToAssetID.insert({filePath, id});
				if(!insertFilePath.second)
				{
					// File path already in map
					std::string& existingPath = (insertID.first)->second;
					PLATINUM_WARNING_STREAM
						<< "AssetsDatabase input data is bad. File path has already been declared before: "
						<< existingPath
						<< " at line " << line;
					// undo insertID
					assetsDatabase._assetIDToFilePath.erase(insertID.first);
				}
			}
			else
			{
				// AssetID already in map
				std::string& existingPath = (insertID.first)->second;
				PLATINUM_WARNING_STREAM
					<< "AssetsDatabase input data is bad. Same AssetID used by multiple files: "
					<< existingPath
					<< " and "
				   	<< filePath
				   	<< " at line " << line;
			}

			++line;
		}

		if (!input.eof())
			PLATINUM_WARNING_STREAM << "AssetsDatabase input stream has format error at line " << line;

		return input;
	}

	bool TryGetFilePath(AssetID assetID, std::string& outFilePath)
	{
		auto i = _assetIDToFilePath.find(assetID);
		if (i == _assetIDToFilePath.end())
			return false;
		else
		{
			outFilePath = i->second;
			return true;
		}
	}

	bool TryGetID(const std::string& filePath, AssetID& outAssetID)
	{
		auto i = _filePathToAssetID.find(filePath);
		if (i == _filePathToAssetID.end())
			return false;
		else
		{
			outAssetID = i->second;
			return true;
		}
	}

	/**
	 * "plz notice me senpai" sort of situation
	 */
	void DetectFilePath(const std::experimental::filesystem::path& filePath)
	{
		if (AssetID getID; TryGetID(filePath, getID))
		{
			// remove old pair
			_assetIDToFilePath.erase(getID);
			_filePathToAssetID.erase(filePath);

			PLATINUM_WARNING("File path already exists. Deleting existing entry.");
		}

		// generate unique ID for file
		std::size_t newID;
		if (BIG_FILE_THRESHOLD_BYTES < std::experimental::filesystem::file_size(filePath))
		{
			// BIG FILE
			// generate AssetID from hash of file name
			newID = std::hash<std::string>{}(filePath.filename());
			PLATINUM_INFO_STREAM << "Hashed BIG FILE" << filePath;
		}
		else
		{
			// small file
			// generate AssetID from hash of file content
			std::ifstream fileStream(filePath);
			std::stringstream buffer;
			buffer << fileStream.rdbuf();
			newID = std::hash<std::string>{}(buffer.str());
			PLATINUM_INFO_STREAM << "Hashed small file" << filePath;
		}

		std::string existingFilePath;
		if (TryGetFilePath(newID, existingFilePath))
		{
			PLATINUM_WARNING_STREAM
				<< "Generating AssetID failed for " << filePath
				<< ". Content already exists from another file: " << existingFilePath;

			// hash with filePath
			newID = newID ^ (std::hash<std::string>{}(filePath) << 1);

			while (TryGetFilePath(newID, existingFilePath))
			{
				PLATINUM_WARNING_STREAM
					<< "Generating AssetID failed for " << filePath
					<< " again! AssetID already used by another file: " << existingFilePath;
				// generate totally random
				newID = _anyNumber(_generator);
			}
		}

		_assetIDToFilePath.insert({newID, filePath});
		_filePathToAssetID.insert({filePath, newID});
		PLATINUM_INFO_STREAM << "Added new file " <<  filePath << ", AssetID=" << newID;
	}

private:

	std::map<AssetID, std::string> _assetIDToFilePath;
	std::map<std::string, AssetID> _filePathToAssetID;

	std::mt19937 _generator;
	std::uniform_int_distribution<size_t> _anyNumber;
};

int main()
{
	const std::string ASSETS_DATABASE = "./Assets/AssetsDatabase.txt";
	const std::string ASSETS_FOLDER = "./Assets";

	AssetsDatabase assetsDatabase;

	{
		std::ifstream inputFile(ASSETS_DATABASE);
		if (inputFile.is_open())
			inputFile >> assetsDatabase;
	}

	for (recursive_directory_iterator i(ASSETS_FOLDER), end; i != end; ++i)
	{
		if (!is_directory(i->path()))
		{
			assetsDatabase.DetectFilePath(i->path());
		}
	}

	{
		std::ofstream outputFile(ASSETS_DATABASE);
		outputFile << assetsDatabase;
	}

    return EXIT_SUCCESS;
}


int main2(int, char**)
{
	// Setup window
	glfwSetErrorCallback(GlfwErrorCallback);
	if (!glfwInit())
		return EXIT_FAILURE;

	// OpenGL+GLSL versions
    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac

	// Create window with graphics context
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
	if (window == NULL)
		return EXIT_FAILURE;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync


	{
		// this scope has a valid OpenGL context initialised

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		// construct logger before everything to save all logs
		bool isLoggerOpen = true;
		PlatinumEngine::Logger logger;

		bool isRasterRendererOpen = true;
		PlatinumEngine::Renderer rasterRenderer;

		bool isInputWindowOpen = true;
		PlatinumEngine::InputManager inputManager;

		bool isSceneEditorOpen = true;
		PlatinumEngine::Scene scene;
		PlatinumEngine::SceneEditor sceneEditor(&inputManager, &scene, &rasterRenderer);

		bool isHierarchyWindowOpen = true;
		PlatinumEngine::HierarchyWindow hierarchyWindow;

		bool isInspectorWindowOpen = true;
		PlatinumEngine::InspectorWindow inspectorWindow;

		PlatinumEngine::WindowManager windowManager;


		//---------TEST-----------

		for (int i = 0; i < 5; i++)
		{
			scene.AddGameObject("GameObject");
		}

		for (int i = 0; i < 5; i++)
		{
			scene.AddGameObject(std::string("GameObject2").append(std::to_string(i)), scene.GetRootGameObject(i));
		}

		//--------END TEST----------



		// Main loop
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			//--------------------------------------------------------------------------------------------------------------
			// GUI HERE
			//--------------------------------------------------------------------------------------------------------------
			ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
			if (isSceneEditorOpen)
				sceneEditor.ShowGUIWindow(&isSceneEditorOpen);

			if(isInputWindowOpen)
				inputManager.ShowGUIWindow(&isInputWindowOpen);

			if(isInputWindowOpen)
				inputManager.ShowGUIWindow(&isInputWindowOpen);
			if(isHierarchyWindowOpen)
			{
				hierarchyWindow.ShowGUIWindow(&isHierarchyWindowOpen, scene);
				inspectorWindow.SetActiveGameObject(hierarchyWindow.selectedGameObject);

				if(isInspectorWindowOpen)
					inspectorWindow.ShowGUIWindow(&isInspectorWindowOpen);
			}
			if(isLoggerOpen)
				logger.ShowGUIWindow(&isLoggerOpen);

			windowManager.ShowGUI();
			//ImGui::ShowDemoWindow();
			//--------------------------------------------------------------------------------------------------------------
			// END OF GUI
			//--------------------------------------------------------------------------------------------------------------

			// Rendering
			ImGui::Render();

			int display_w, display_h;
			glfwGetFramebufferSize(window, &display_w, &display_h);
			GL_CHECK(glViewport(0, 0, display_w, display_h));
			ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
			GL_CHECK(glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w));
			GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
		}



		// Cleanup ImGui
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	// Cleanup glfw
	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}