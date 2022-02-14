#-----------------------------
# Initialising stuff
#-----------------------------
cmake_minimum_required(VERSION 3.21)
project(PlatinumEngine VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 14)
# static libraries please
set(BUILD_SHARED_LIBS FALSE)
# FetchContent to fetch git repos remotely
include(FetchContent)
# enable CTest
enable_testing()
#-----------------------------

#-----------------------------
# External libraries from git repos
#-----------------------------
# SFML
set(SFML_USE_STATIC_STD_LIBS TRUE)
FetchContent_Declare(sfml GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 2.5.1)
FetchContent_MakeAvailable(sfml)
#-----------------------------
# Dear ImGui
FetchContent_Declare(imgui GIT_REPOSITORY https://github.com/ocornut/imgui.git GIT_TAG v1.86)
FetchContent_MakeAvailable(imgui)
include_directories(${CMAKE_BINARY_DIR}/_deps/imgui-src)
#-----------------------------
#ImGuiFileDialog
FetchContent_Declare(imguifiledialog GIT_REPOSITORY https://github.com/aiekick/ImGuiFileDialog.git GIT_TAG v0.6.3)
FetchContent_MakeAvailable(imguifiledialog)
include_directories(${CMAKE_BINARY_DIR}/_deps/imguifiledialog-src)
#-----------------------------
# imgui-SFML
# This was so painful to figure out! Never forget.
set(IMGUI_SFML_FIND_SFML FALSE)
set(IMGUI_DIR "${CMAKE_BINARY_DIR}/_deps/imgui-src")
FetchContent_Declare(imgui_sfml GIT_REPOSITORY https://github.com/eliasdaler/imgui-sfml.git GIT_TAG v2.5)
FetchContent_MakeAvailable(imgui_sfml)
#-----------------------------
# GLM
FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git GIT_TAG 0.9.9.8)
FetchContent_MakeAvailable(glm)
#-----------------------------
# assimp
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT FALSE)
set(ASSIMP_BUILD_OBJ_IMPORTER TRUE)
set(ASSIMP_BUILD_FBX_IMPORTER TRUE)
set(ASSIMP_BUILD_BLEND_IMPORTER TRUE)
set(ASSIMP_NO_EXPORT TRUE)
set(ASSIMP_BUILD_TESTS FALSE)
FetchContent_Declare(assimp GIT_REPOSITORY https://github.com/assimp/assimp.git GIT_TAG 5.2.0)
FetchContent_MakeAvailable(assimp)
#-----------------------------
# Catch2 - testing framework
set(CATCH_BUILD_STATIC_LIBRARY TRUE)
FetchContent_Declare(Catch2 GIT_REPOSITORY https://github.com/catchorg/Catch2.git GIT_TAG v2.13.8)
FetchContent_MakeAvailable(Catch2)
#-----------------------------
# Add more libraries from git repos here:
#-----------------------------
# glew
set(glew-cmake_BUILD_SHARED OFF)
set(USE_GLU "Use GLU" ON)
FetchContent_Declare(glew GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git GIT_TAG glew-cmake-2.2.0)
FetchContent_MakeAvailable(glew)

#-----------------------------
# IMPORTANT SECTION! Source code
#-----------------------------
# New place to put all your source code files into:
set(PROJECT_SOURCES_WITHOUT_MAIN
	"Source/InputManager/InputManager.cpp;"
	"Source/RasterRenderer/RasterRenderer.cpp;"
	"Source/SceneManager/SceneManager.cpp;"
	"Source/OpenGL/ShaderInput.cpp;"
	"Source/OpenGL/GLEnumInfo.cpp;"
	"Source/OpenGL/GLCheck.cpp;"
  	"Source/SceneManager/SceneManager.cpp;"
  	"Source/RasterRenderer/EditorCamera.cpp;"
	"Source/SceneManager/SceneManager.cpp;"
	"Source/WindowManager/WindowManager.cpp;"
	"${CMAKE_BINARY_DIR}/_deps/imguifiledialog-src/ImGuiFileDialog.cpp;")
add_executable(${PROJECT_NAME} Source/main.cpp ${PROJECT_SOURCES_WITHOUT_MAIN})
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/Include")
#-----------------------------


#-----------------------------
# IMPORTANT SECTION! Unit testing
#-----------------------------
# Add unit tests here:
add_executable(tests
	${PROJECT_SOURCES_WITHOUT_MAIN}
	Test/TestTemplate.cpp
	)
target_include_directories(tests PRIVATE "${CMAKE_CURRENT_LIST_DIR}/Include")
#-----------------------------


#-----------------------------
# Linking libraries
#-----------------------------
# list of libraries we're using in the main engine
set(PROJECT_LIBRARIES
	# good luck trying to find these names and trying to match them
	# CMake is so stupid
	"sfml-audio;sfml-graphics;sfml-system;sfml-window;"
	"ImGui-SFML;"
	"glm;"
	"assimp;"
	"libglew_static;")
# different targets
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_LIBRARIES})
target_link_libraries(tests PRIVATE ${PROJECT_LIBRARIES} Catch2::Catch2WithMain)
#-----------------------------