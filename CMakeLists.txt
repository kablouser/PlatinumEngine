#-----------------------------
# Initialising stuff
#-----------------------------
cmake_minimum_required(VERSION 3.21)
project(PlatinumEngine VERSION 0.0.2)
if (APPLE)
	enable_language(OBJC)
endif()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
# static libraries please
set(BUILD_SHARED_LIBS FALSE)
# make executable run in project's root directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
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
FetchContent_Declare(imgui GIT_REPOSITORY https://github.com/ocornut/imgui.git GIT_TAG origin/docking)
FetchContent_MakeAvailable(imgui)
add_subdirectory("CMake Subfolders/ImGui CMake")
#-----------------------------
# ImGuiFileDialog
add_compile_definitions(USE_BOOKMARK)
FetchContent_Declare(imguifiledialog GIT_REPOSITORY https://github.com/aiekick/ImGuiFileDialog.git GIT_TAG v0.6.4)
FetchContent_MakeAvailable(imguifiledialog)
include_directories(${FETCHCONTENT_BASE_DIR}/imguifiledialog-src)
#-----------------------------
# Immediate Mode Plotting (implot)
FetchContent_Declare(implot GIT_REPOSITORY https://github.com/epezent/implot.git GIT_TAG v0.13)
FetchContent_MakeAvailable(implot)
add_subdirectory("CMake Subfolders/ImPlot CMake")
#-----------------------------
# GLM
FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git GIT_TAG 0.9.9.8)
FetchContent_MakeAvailable(glm)
add_compile_definitions(GLM_FORCE_LEFT_HANDED)
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
# SDL
set(SDL_SHARED OFF)
set(SDL_STATIC ON)
FetchContent_Declare(sdl GIT_REPOSITORY https://github.com/libsdl-org/SDL.git GIT_TAG 2.0.22-RC1)
FetchContent_MakeAvailable(sdl)
set(SDL2_INCLUDE_DIR "${FETCHCONTENT_BASE_DIR}/sdl-src/include")
#-----------------------------
# SDL2 Mixer
FetchContent_Declare(sdl2mixer GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git GIT_TAG release-2.0.4)
FetchContent_MakeAvailable(sdl2mixer)
add_subdirectory("CMake Subfolders/SDL2Mixer CMake")
set(SDL2_MIXER_INCLUDE_DIR "${FETCHCONTENT_BASE_DIR}/sdl2mixer-src")
#-----------------------------
# imguizmo
FetchContent_Declare(imguizmo GIT_REPOSITORY https://github.com/CedricGuillemet/ImGuizmo.git GIT_TAG origin/master)
FetchContent_MakeAvailable(imguizmo)
#-----------------------------
# stb_image
FetchContent_Declare(stb GIT_REPOSITORY https://github.com/nothings/stb.git GIT_TAG origin/master)
FetchContent_MakeAvailable(stb)
#----------------------------
# bullet3 physics library
FetchContent_Declare(bullet3 GIT_REPOSITORY https://github.com/bulletphysics/bullet3.git GIT_TAG 3.22b)
FetchContent_MakeAvailable(bullet3)
#-----------------------------
# ozz
set(ozz_build_fbx OFF)
set(ozz_build_samples OFF)
set(ozz_build_howtos ON)
set(ozz_build_tests OFF)
set(ozz_build_tools ON)
set(ozz_build_postfix OFF)
set(ozz_build_simd_ref ON)
FetchContent_Declare(ozz GIT_REPOSITORY https://github.com/guillaumeblanc/ozz-animation.git GIT_TAG 0.14.0)
FetchContent_MakeAvailable(ozz)


#-----------------------------
# Add more libraries from git repos here:
#-----------------------------

#-----------------------------
# IMPORTANT SECTION! Source code
#-----------------------------
# New place to put all your source code files into:
set(PROJECT_SOURCES_WITHOUT_MAIN
	# order by folder structure

	"${FETCHCONTENT_BASE_DIR}/imguifiledialog-src/ImGuiFileDialog.cpp;"
	"${FETCHCONTENT_BASE_DIR}/imguizmo-src/ImGuizmo.cpp;"
	
	"Source/Animation/Animation.cpp;"
	"Source/Animation/AnimationLocalTimer.cpp;"
	
	"Source/AssetDatabase/AssetDatabase.cpp;"
	
	"Source/Audio/AudioClip.cpp;"
	
	"Source/ComponentComposition/Camera.cpp;"
	"Source/ComponentComposition/AudioComponent.cpp;"
	"Source/ComponentComposition/AnimationComponent.cpp;"
	"Source/ComponentComposition/Color.cpp;"
	"Source/ComponentComposition/Component.cpp;"
	"Source/ComponentComposition/GameObject.cpp;"
	"Source/ComponentComposition/MeshRender.cpp;"
	"Source/ComponentComposition/Transform.cpp;"	
	"Source/ComponentComposition/Light.cpp;"
	"Source/ComponentComposition/Objects.cpp;"
	"Source/ComponentComposition/RigidBody.cpp;"
	"Source/ComponentComposition/BoxCollider.cpp;"
	"Source/ComponentComposition/CapsuleCollider.cpp;"
	"Source/ComponentComposition/SphereCollider.cpp;"
	"Source/ComponentComposition/Collider.cpp;"
	"Source/ComponentComposition/ParticleEffect.cpp;"

	
	"Source/GameWindow/GameWindow.cpp;"
	
	"Source/IDSystem/IDSystem.cpp;"
	
	"Source/InputManager/InputManager.cpp;"
	
	"Source/Inspector/InspectorWindow.cpp;"
  
	"Source/Loaders/LoaderCommon.cpp;"
	"Source/Loaders/MeshLoader.cpp;"

	"Source/Logger/Logger.cpp;"
	
	"Source/Maths/Common.cpp;"
	"Source/Maths/Matrices.cpp;"
	"Source/Maths/Quaternion.cpp;"
	"Source/Maths/Vectors.cpp;"
	
	"Source/OpenGL/Framebuffer.cpp;"
	"Source/OpenGL/GLCheck.cpp;"
	"Source/OpenGL/GLEnumInfo.cpp;"
	"Source/OpenGL/Mesh.cpp;"
	"Source/OpenGL/ShaderInput.cpp;"
	"Source/OpenGL/ShaderProgram.cpp;"
	"Source/OpenGL/Texture.cpp;"
	"Source/OpenGL/Material.cpp;"
	

	"Source/Physics/Physics.cpp;"

	"Source/ParticleEffects/ParticleEmitter.cpp"
	"Source/ParticleEffects/ParticleRenderer.cpp;"

	
	"Source/Profiler/Profiler.cpp;"
	
	"Source/Project/ProjectWindow.cpp;"
	
	"Source/Renderer/Renderer.cpp;"
	
	"Source/SceneEditor/EditorCamera.cpp;"
	"Source/SceneEditor/SceneEditor.cpp;"
	
	"Source/SceneManager/HierarchyWindow.cpp;"
	"Source/SceneManager/Scene.cpp;"
	
	"Source/TypeDatabase/TypeDatabase.cpp;"
	"Source/TypeDatabase/TypeDatabaseInit.cpp;"
	
	"Source/WindowManager/Filedialog.cpp;"
	"Source/WindowManager/WindowManager.cpp;"
	
	"Source/Application.cpp;"
	)

add_executable(${PROJECT_NAME} Source/main.cpp ${PROJECT_SOURCES_WITHOUT_MAIN})
#-----------------------------

#-----------------------------
# IMPORTANT SECTION! Unit testing
#-----------------------------
# Add unit tests here:
set(UNIT_TESTS
	"Test/TestTemplate.cpp;"
	"Test/Maths/VectorsTest.cpp;"
	"Test/Maths/MatricesTest.cpp;"
	"Test/SceneEditor/EditorCameraTest.cpp;"
	"Test/TypeDatabase/TestTypeDatabase.cpp;"
	"Test/IDSystem/TestIDSystem.cpp;"
	Include/Animation/AnimationLocalTimer.h Source/Animation/AnimationLocalTimer.cpp)
add_executable(tests ${PROJECT_SOURCES_WITHOUT_MAIN} ${UNIT_TESTS})
#-----------------------------


#-----------------------------
# IMPORTANT SECTION! Include files
#-----------------------------
# list of folders to include
set(PROJECT_INCLUDES
	"Include;"
	"Fonts;"
	"${FETCHCONTENT_BASE_DIR}/imguizmo-src;"
	"${FETCHCONTENT_BASE_DIR}/stb-src;"
	"${FETCHCONTENT_BASE_DIR}/bullet3-src/src"
 	"${SDL2_INCLUDE_DIR}" 
    "${SDL2_MIXER_INCLUDE_DIR}"
	)
	
# different targets
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDES})
target_include_directories(tests PRIVATE ${PROJECT_INCLUDES})
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
	"implot;"
	"glm;"
	"assimp;"
	"libglew_static;"

	# Bullet3D libs
	"BulletDynamics;"
	"Bullet3Common;"
	"BulletCollision;"
	"LinearMath;"
	#"BulletInverseDynamics;"
	#"BulletSoftBody;" #shouldnt be necessary

	"SDL2_mixer;"

	"ozz_animation;"
	"ozz_animation_offline;"
  )

# different targets
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_LIBRARIES})
target_link_libraries(tests PRIVATE ${PROJECT_LIBRARIES} Catch2::Catch2WithMain)
#-----------------------------
