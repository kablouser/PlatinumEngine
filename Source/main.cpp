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

	struct Asset
	{
		// note: id is NOT hash
		AssetID id;
		size_t hash;
		bool doesExist;
		std::experimental::filesystem::path path;
	};

	// 1MB
	static constexpr std::uintmax_t BIG_FILE_THRESHOLD_BYTES = 1000000;
	static constexpr const char* FIRST_LINE = "id, hash, doesExist, path";

	AssetsDatabase() : _nextAssetID(0)
	{
	}

	friend std::ostream& operator<<(std::ostream& output, AssetsDatabase& assetsDatabase)
	{
		output << FIRST_LINE << std::endl;
		for(auto asset : assetsDatabase._database)
		{
			output << asset.id << ',' << asset.hash << ',' << asset.doesExist << ',' << asset.path << std::endl;
		}
		return output;
	}

	friend std::istream& operator>>(std::istream& input, AssetsDatabase& assetsDatabase)
	{
		// enforce a empty object
		assetsDatabase._database.clear();
		assetsDatabase._assetIDsMap.clear();

		{
			std::string firstLine;
			std::getline(input, firstLine);
			if(firstLine != FIRST_LINE)
			{
				PLATINUM_WARNING("AssetsDatabase stream input: unexpected first line");
			}
		}

		Asset asset;
		std::string filePath;
		size_t line = 1;
		while (input >> asset.id && input.get() &&
			input >> asset.hash && input.get() &&
			input >> asset.doesExist && input.get() &&
			std::getline(input, filePath))
		{
			// construct path object, could exist or not exist
			asset.path = std::experimental::filesystem::path(filePath);

			// find id
			auto i = assetsDatabase._assetIDsMap.find(asset.id);
			// if id is unique
			if (i == assetsDatabase._assetIDsMap.end())
			{
				assetsDatabase._database.push_back(asset);
				assetsDatabase._assetIDsMap.insert({ asset.id, assetsDatabase._database.size() - 1});
			}
			else
			{
				// index of existing AssetID in database
				size_t databaseIndex = i->second;
				PLATINUM_WARNING_STREAM
						<< "AssetsDatabase input data is bad. Same AssetID was declared for multiple files: "
						<< assetsDatabase._database[databaseIndex].path
						<< " and "
						<< asset.path
						<< " at line " << line;
			}

			++line;
		}

		if (!input.eof())
			PLATINUM_WARNING_STREAM << "AssetsDatabase input stream has format error at line " << line;

		return input;
	}

	static std::size_t HashFile(const std::experimental::filesystem::path& path)
	{
		if (std::experimental::filesystem::file_size(path) < BIG_FILE_THRESHOLD_BYTES)
		{
			// small file
			// hash file's content
			std::ifstream fileStream(path);
			if(fileStream.is_open())
			{
				std::stringstream buffer;
				buffer << fileStream.rdbuf();
				return std::hash<std::string>{}(buffer.str());
			}
			else
			{
				// if file cannot open, just hash the same way as BIG FILE
				PLATINUM_WARNING_STREAM << "Cannot hash file content because it can't open: " << path;
			}
		}

		// BIG FILE or cannot open file
		// hash file name
		return std::hash<std::string>{}(path.filename());
	}

	bool TryGetAsset(AssetID id, Asset& outAsset)
	{
		auto i = _assetIDsMap.find(id);
		if (i == _assetIDsMap.end())
			return false;
		else
		{
			// range-checked indexing
			outAsset = _database.at(i->second);
			return true;
		}
	}

	bool TryGetAsset(const std::string& filePath, Asset& outAsset)
	{
		for(auto& i : _database)
		{
			if (i.path == filePath)
			{
				outAsset = i;
				return true;
			}
		}
		return false;
	}

	// nodiscard means compiler creates warning if you don't use this function's return value
	[[nodiscard]] const std::vector<Asset>& GetDatabase() const
	{
		return _database;
	}

	void Update(const std::vector<std::experimental::filesystem::path>& currentExistingPaths)
	{
		// build map from paths to database index, 1-to-1
		std::map<std::experimental::filesystem::path, size_t> pathsMap;
		// build map from hashes to database index, 1-to-many
		std::multimap<size_t, size_t> hashesMap;

		{
			size_t databaseIndex = 0;
			for (auto& asset: _database)
			{
				// ensure current database is update to date
				asset.doesExist = std::experimental::filesystem::exists(asset.path);
				// if asset no longer exists, keep it in the database still,
				// asset might "come back" later
				// "come back" could be moved/renamed

				// if path insert failed because path is not unique
				if (!pathsMap.insert({ asset.path, databaseIndex }).second)
				{
					PLATINUM_WARNING("AssetsDatabase structure is wrong");
				}

				hashesMap.insert({ asset.hash, databaseIndex });

				++databaseIndex;
			}
		}

		// add any new assets in currentPaths
		for(auto& path : currentExistingPaths)
		{
			if (!std::experimental::filesystem::exists(path))
				// programmer made mistake with the input
				continue;

			size_t hash = HashFile(path);

			auto findPath = pathsMap.find(path);
			if(findPath == pathsMap.end())
			{
				// no path
				auto findHashes = hashesMap.equal_range(hash);
				auto findHashesBegin = findHashes.first;
				auto findHashesEnd = findHashes.second;

				// count the number of Assets that exist/don't exist out of the found hashes
				size_t existCount = 0;
				size_t nonExistCount = 0;
				size_t lastDatabaseIndex = -1;
				for(auto findHash = findHashesBegin; findHash != findHashesEnd; ++findHash)
				{
					size_t databaseIndex = findHash->second;
					if (_database[databaseIndex].doesExist)
					{
						++existCount;
						lastDatabaseIndex = databaseIndex;
						break;
					}
					else
					{
						++nonExistCount;
						lastDatabaseIndex = databaseIndex;
						if(1 < nonExistCount)
							break;
					}
				}

				if(existCount == 0 && nonExistCount == 0)
				{
					// probably new file
					_database.push_back()
				}

			}
			else
			{
				// found path
			}
		}
	}

	// TODO function to purge all

private:

	std::vector<Asset> _database;
	// maps assetID to database index
	std::map<AssetID, size_t> _assetIDsMap;

	size_t _nextAssetID;

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