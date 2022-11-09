cmake_minimum_required(VERSION 3.11)

if (${CMAKE_VERSION} VERSION_LESS 3.24)
	cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
	cmake_policy(VERSION 3.24)
endif()

project(CeVIO_AI_DllHook VERSION 1.0 LANGUAGES C)

add_library(hook SHARED src/main.c)

target_link_libraries(hook minhook)
