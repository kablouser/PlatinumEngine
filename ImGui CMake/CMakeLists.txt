# imgui has no CMakeLists.txt in the git repository.
# This file is the substitute.
# But since _deps is in Build folder, it would be nice to distribute this file through vcs.
# So, this is outside of the imgui source folder.

cmake_minimum_required(VERSION 3.21)
project(imgui)

set(IMGUI_SRC_DIR ${CMAKE_BINARY_DIR}/_deps/imgui-src)

add_library(imgui STATIC
	${IMGUI_SRC_DIR}/imgui.cpp
	${IMGUI_SRC_DIR}/imgui_demo.cpp
	${IMGUI_SRC_DIR}/imgui_draw.cpp
	${IMGUI_SRC_DIR}/imgui_tables.cpp
	${IMGUI_SRC_DIR}/imgui_widgets.cpp
	
	${IMGUI_SRC_DIR}/backends/imgui_impl_glfw.cpp
	${IMGUI_SRC_DIR}/backends/imgui_impl_opengl3.cpp)

target_include_directories(imgui PUBLIC
	${IMGUI_SRC_DIR}
	${IMGUI_SRC_DIR}/backends)

target_link_libraries(imgui PRIVATE glfw)

add_library(imgui::imgui ALIAS imgui)