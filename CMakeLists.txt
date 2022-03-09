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
# glfw
set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG 3.3.6)
FetchContent_MakeAvailable(glfw)
#-----------------------------
# Dear ImGui
FetchContent_Declare(imgui GIT_REPOSITORY https://github.com/ocornut/imgui.git GIT_TAG v1.86)
FetchContent_MakeAvailable(imgui)
add_subdirectory("ImGui CMake")
#-----------------------------
# ImGuiFileDialog
FetchContent_Declare(imguifiledialog GIT_REPOSITORY https://github.com/aiekick/ImGuiFileDialog.git GIT_TAG v0.6.3)
FetchContent_MakeAvailable(imguifiledialog)
include_directories(${CMAKE_BINARY_DIR}/_deps/imguifiledialog-src)
#-----------------------------
# GLM
FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git GIT_TAG 0.9.9.8)
FetchContent_MakeAvailable(glm)
#-----------------------------
# assimp
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT FALSE)
set(ASSIMP_BUILD_OBJ_IMPORTER TRUE)
set(ASSIMP_BUILD_FBX_IMPORTER TRUE)
set(ASSIMP_BUILD_BLEND_IMPORTER FALSE)
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
# glew
set(glew-cmake_BUILD_SHARED OFF)
set(USE_GLU "Use GLU" ON)
FetchContent_Declare(glew GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git GIT_TAG glew-cmake-2.2.0)
FetchContent_MakeAvailable(glew)
#-----------------------------
# Add more libraries from git repos here:
#-----------------------------

#-----------------------------
# IMPORTANT SECTION! Source code
#-----------------------------
# New place to put all your source code files into:
set(PROJECT_SOURCES_WITHOUT_MAIN
	"Source/InputManager/InputManager.cpp;"
	"Source/Maths/Vectors.cpp;"
	"Source/RasterRenderer/RasterRenderer.cpp;"
	"Source/OpenGL/ShaderInput.cpp;"
	"Source/OpenGL/GLEnumInfo.cpp;"
	"Source/OpenGL/GLCheck.cpp;"
#  	"Source/SceneManager/SceneManager.cpp;"
  	"Source/RasterRenderer/EditorCamera.cpp;"
	"Source/WindowManager/WindowManager.cpp;"
	"${CMAKE_BINARY_DIR}/_deps/imguifiledialog-src/ImGuiFileDialog.cpp;"
	"Source/OpenGL/ShaderProgram.cpp;"
	"Source/OpenGL/Texture.cpp;"
	"Source/OpenGL/Framebuffer.cpp;"
	"Source/Maths/Matrices.cpp;")
add_executable(${PROJECT_NAME} Source/main.cpp ${PROJECT_SOURCES_WITHOUT_MAIN} )
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
	"glfw;"
	"imgui;"
	"glm;"
	"assimp;"
	"libglew_static;")
# different targets
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_LIBRARIES})
target_link_libraries(tests PRIVATE ${PROJECT_LIBRARIES} Catch2::Catch2WithMain)
#-----------------------------