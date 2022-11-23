cmake_minimum_required(VERSION 3.11)

if (${CMAKE_VERSION} VERSION_LESS 3.24)
	cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
	cmake_policy(VERSION 3.24)
endif()

project(CeVIO_AI_On_Wine VERSION 1.0 LANGUAGES C)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif()

set(BUILD_SHARED_LIBS OFF)

add_subdirectory(extern/minhook)
add_subdirectory(extern/libkcnf)

add_subdirectory(hook)
add_subdirectory(dump)

