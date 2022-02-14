# Install script for directory: F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-build/lib/libassimpd.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/anim.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/aabb.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/ai_assert.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/camera.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/color4.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/color4.inl"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-build/code/../include/assimp/config.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/ColladaMetaData.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/commonMetaData.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/defs.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/cfileio.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/light.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/material.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/material.inl"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/matrix3x3.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/matrix3x3.inl"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/matrix4x4.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/matrix4x4.inl"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/mesh.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/pbrmaterial.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/GltfMaterial.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/postprocess.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/quaternion.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/quaternion.inl"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/scene.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/metadata.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/texture.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/types.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/vector2.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/vector2.inl"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/vector3.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/vector3.inl"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/version.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/cimport.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/importerdesc.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Importer.hpp"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/DefaultLogger.hpp"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/ProgressHandler.hpp"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/IOStream.hpp"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/IOSystem.hpp"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Logger.hpp"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/LogStream.hpp"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/NullLogger.hpp"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/cexport.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Exporter.hpp"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/DefaultIOStream.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/DefaultIOSystem.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/ZipArchiveIOSystem.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/SceneCombiner.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/fast_atof.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/qnan.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/BaseImporter.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Hash.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/MemoryIOWrapper.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/ParsingUtils.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/StreamReader.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/StreamWriter.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/StringComparison.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/StringUtils.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/SGSpatialSort.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/GenericProperty.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/SpatialSort.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/SkeletonMeshBuilder.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/SmallVector.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/SmoothingGroups.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/SmoothingGroups.inl"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/StandardShapes.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/RemoveComments.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Subdivision.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Vertex.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/LineSplitter.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/TinyFormatter.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Profiler.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/LogAux.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Bitmap.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/XMLTools.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/IOStreamBuffer.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/CreateAnimMesh.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/XmlParser.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/BlobIOSystem.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/MathFunctions.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Exceptional.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/ByteSwapper.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Base64.hpp"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Compiler/pushpack1.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Compiler/poppack1.h"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

