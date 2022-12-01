cmake_minimum_required(VERSION 3.11)

if (${CMAKE_VERSION} VERSION_LESS 3.24)
	cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
	cmake_policy(VERSION 3.24)
endif()

project(CeVIO_AI_DllHook VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_SHARED_LIBRARY_PREFIX "")
add_library(hook SHARED src/dllmain.c src/dllmain.def src/misc.c src/debug.c src/hooks.c src/wmi/wminet.cpp src/wmi/com/wbem.cpp)

set_target_properties(hook PROPERTIES OUTPUT_NAME imm32)

target_link_libraries(hook minhook)

if (MINGW)
	set(DIST_FOLDER ${CMAKE_BINARY_DIR}/dist/hook)
	add_custom_command(
		TARGET hook
		POST_BUILD
		COMMENT "Constructing Dist Folder..."
		COMMAND rm -rf ${DIST_FOLDER} ${DIST_FOLDER}/../hook.zip
		COMMAND mkdir -p ${DIST_FOLDER}
		COMMAND cp ${CMAKE_BINARY_DIR}/hook/imm32.dll ${DIST_FOLDER}
		COMMAND cp ${CMAKE_SOURCE_DIR}/misc/*.reg ${DIST_FOLDER}
		COMMAND zip -j ${DIST_FOLDER}/../hook.zip ${DIST_FOLDER}/*
	)
endif(MINGW)

