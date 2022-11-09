cmake_minimum_required(VERSION 3.11)

if (${CMAKE_VERSION} VERSION_LESS 3.24)
	cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
	cmake_policy(VERSION 3.24)
endif()

project(CeVIO_AI_On_Wine VERSION 1.0 LANGUAGES C)

set(BUILD_SHARED_LIBS OFF)

if (MINGW)
	# MinHook failes to configure due to case sensitiveness in UNIX :(
	execute_process(COMMAND git reset --hard WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/extern/minhook COMMAND_ECHO STDOUT)
	execute_process(COMMAND patch ${CMAKE_SOURCE_DIR}/extern/minhook/CMakeLists.txt ${CMAKE_SOURCE_DIR}/misc/minhook_cmake_patch.diff COMMAND_ECHO STDOUT)
endif()

add_subdirectory(extern/minhook)

add_subdirectory(hook)

