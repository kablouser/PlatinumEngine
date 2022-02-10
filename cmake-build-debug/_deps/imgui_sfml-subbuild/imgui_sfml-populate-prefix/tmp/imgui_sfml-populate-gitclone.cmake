
if(NOT "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/imgui_sfml-subbuild/imgui_sfml-populate-prefix/src/imgui_sfml-populate-stamp/imgui_sfml-populate-gitinfo.txt" IS_NEWER_THAN "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/imgui_sfml-subbuild/imgui_sfml-populate-prefix/src/imgui_sfml-populate-stamp/imgui_sfml-populate-gitclone-lastrun.txt")
  message(STATUS "Avoiding repeated git clone, stamp file is up to date: 'F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/imgui_sfml-subbuild/imgui_sfml-populate-prefix/src/imgui_sfml-populate-stamp/imgui_sfml-populate-gitclone-lastrun.txt'")
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/imgui_sfml-src"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: 'F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/imgui_sfml-src'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "D:/Program Files/Git/cmd/git.exe"  clone --no-checkout --config "advice.detachedHead=false" "https://github.com/eliasdaler/imgui-sfml.git" "imgui_sfml-src"
    WORKING_DIRECTORY "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps"
    RESULT_VARIABLE error_code
    )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once:
          ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/eliasdaler/imgui-sfml.git'")
endif()

execute_process(
  COMMAND "D:/Program Files/Git/cmd/git.exe"  checkout v2.5 --
  WORKING_DIRECTORY "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/imgui_sfml-src"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: 'v2.5'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "D:/Program Files/Git/cmd/git.exe"  submodule update --recursive --init 
    WORKING_DIRECTORY "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/imgui_sfml-src"
    RESULT_VARIABLE error_code
    )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: 'F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/imgui_sfml-src'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/imgui_sfml-subbuild/imgui_sfml-populate-prefix/src/imgui_sfml-populate-stamp/imgui_sfml-populate-gitinfo.txt"
    "F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/imgui_sfml-subbuild/imgui_sfml-populate-prefix/src/imgui_sfml-populate-stamp/imgui_sfml-populate-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: 'F:/PREDATOR/GitHub/PlatinumEngine/cmake-build-debug/_deps/imgui_sfml-subbuild/imgui_sfml-populate-prefix/src/imgui_sfml-populate-stamp/imgui_sfml-populate-gitclone-lastrun.txt'")
endif()

