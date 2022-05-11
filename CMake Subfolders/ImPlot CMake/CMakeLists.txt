# implot has no CMakeLists.txt in the git repository.
# This file is the substitute.
# But since _deps is in Build folder, it would be nice to distribute this file through vcs.
# So, this is outside of the implot source folder.

cmake_minimum_required(VERSION 3.21)
project(implot)

set(IMPLOT_SRC_DIR ${FETCHCONTENT_BASE_DIR}/implot-src)

add_library(implot STATIC
	${IMPLOT_SRC_DIR}/implot.cpp
	${IMPLOT_SRC_DIR}/implot_demo.cpp
	${IMPLOT_SRC_DIR}/implot_items.cpp)

target_include_directories(implot PUBLIC ${IMPLOT_SRC_DIR})

target_link_libraries(implot PRIVATE imgui)