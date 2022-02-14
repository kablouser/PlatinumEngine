IF(NOT EXISTS "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-build/install_manifest.txt")
  MESSAGE(FATAL_ERROR "Cannot find install manifest: \"F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-build/install_manifest.txt\"")
ENDIF(NOT EXISTS "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-build/install_manifest.txt")

FILE(READ "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/assimp-build/install_manifest.txt" files)
STRING(REGEX REPLACE "\n" ";" files "${files}")
FOREACH(file ${files})
  MESSAGE(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
  EXEC_PROGRAM(
    "F:/PREDATOR/JetBrains/CLion 2021.3.2/bin/cmake/win/bin/cmake.exe" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
    OUTPUT_VARIABLE rm_out
    RETURN_VALUE rm_retval
    )
  IF(NOT "${rm_retval}" STREQUAL 0)
    MESSAGE(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
  ENDIF(NOT "${rm_retval}" STREQUAL 0)
ENDFOREACH(file)
