# Install script for directory: F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/imgui_sfml-src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/PlatinumEngine")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "F:/PREDATOR/JetBrains/CLion 2021.3.2/bin/mingw/bin/objdump.exe")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/imgui_sfml-build/libImGui-SFMLd.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/imgui_sfml-src/imgui-SFML.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/imgui_sfml-src/imgui-SFML_export.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/imgui_sfml-src/imconfig-SFML.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/imgui-src/imconfig.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/imgui-src/imgui.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/imgui-src/imgui_internal.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/imgui-src/imstb_rectpack.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/imgui-src/imstb_textedit.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/imgui-src/imstb_truetype.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/imgui-src/misc/cpp/imgui_stdlib.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ImGui-SFML/ImGui-SFMLConfig.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ImGui-SFML/ImGui-SFMLConfig.cmake"
         "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/imgui_sfml-build/CMakeFiles/Export/lib/cmake/ImGui-SFML/ImGui-SFMLConfig.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ImGui-SFML/ImGui-SFMLConfig-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ImGui-SFML/ImGui-SFMLConfig.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/ImGui-SFML" TYPE FILE FILES "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/imgui_sfml-build/CMakeFiles/Export/lib/cmake/ImGui-SFML/ImGui-SFMLConfig.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/ImGui-SFML" TYPE FILE FILES "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/imgui_sfml-build/CMakeFiles/Export/lib/cmake/ImGui-SFML/ImGui-SFMLConfig-debug.cmake")
  endif()
endif()

