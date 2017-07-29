cmake_minimum_required(VERSION 3.8.0)
set(PROJECT_NAME Ripples)
project(${PROJECT_NAME})
set(${PROJECT_NAME}_VERSION 1.0.0)

include (ExternalProject)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/lib)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_BUILD_TYPE Release)
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c" "src/*.hpp" "src/*.h")
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE src)

target_compile_definitions(${PROJECT_NAME} PUBLIC _HAS_EXCEPTIONS=0
	_STATIC_CPPLIB=1 ENABLE_PROFILING=0)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY
	VS_STARTUP_PROJECT ${PROJECT_NAME})

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra #-Wpedantic
		-Werror -fno-exceptions -fno-rtti)

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra #-Wpedantic
		-Werror -fno-exceptions -fno-rtti)

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # using Intel C++

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	string(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")
	target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX /GR-)

endif()

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})

if (APPLE)
	find_library(COCOA_LIB Cocoa)
	find_library(IOKIT_LIB IOKit)
	find_library(COREVIDEO_LIB CoreVideo)
	target_link_libraries(${PROJECT_NAME} PRIVATE ${COCOA_LIB}
		${IOKIT_LIB} ${COREVIDEO_LIB})
endif()

get_cmake_property(CACHE_VARS CACHE_VARIABLES)
foreach(CACHE_VAR ${CACHE_VARS})
	get_property(CACHE_VAR_HELPSTRING CACHE ${CACHE_VAR}
		PROPERTY HELPSTRING)

	if(CACHE_VAR_HELPSTRING
			STREQUAL "No help, variable specified on the command line.")
		get_property(CACHE_VAR_TYPE CACHE ${CACHE_VAR} PROPERTY TYPE)

		if(CACHE_VAR_TYPE STREQUAL "UNINITIALIZED")
			set(CACHE_VAR_TYPE)
		else()
			set(CACHE_VAR_TYPE :${CACHE_VAR_TYPE})
	endif()
	set(CMAKE_ARGS "${CMAKE_ARGS}
		-D${CACHE_VAR}${CACHE_VAR_TYPE}=\"${${CACHE_VAR}}\"")
	endif()
	endforeach()
message("CMAKE_ARGS: ${CMAKE_ARGS}")

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_BINARY_DIR}/include)

ExternalProject_Add(glad_ext
	GIT_REPOSITORY https://github.com/Dav1dde/glad.git
	GIT_TAG v0.1.14a0
	CMAKE_ARGS ${CMAKE_ARGS}
		-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}
		-DCMAKE_PREFIX_PATH=${CMAKE_CURRENT_BINARY_DIR}
		-DGLAD_PROFILE="core"
		-DGLAD_API="gl=4.1"
		-DGLAD_INSTALL=On
)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)

#set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
#set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
#set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
ExternalProject_Add(glfw3_ext
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG 3.2.1
	CMAKE_ARGS ${CMAKE_ARGS}
		-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}
		-DCMAKE_PREFIX_PATH=${CMAKE_CURRENT_BINARY_DIR}
		-DGLFW_BUILD_DOCS=Off
		-DGLFW_BUILD_TESTS=Off
		-DGLFW_BUILD_EXAMPLES=Off
)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw3)

ExternalProject_Add(glm_ext
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG 0.9.8.4
	CMAKE_ARGS ${CMAKE_ARGS}
		-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}
		-DCMAKE_PREFIX_PATH=${CMAKE_CURRENT_BINARY_DIR}
)
target_link_libraries(${PROJECT_NAME} INTERFACE glm)
add_dependencies(${PROJECT_NAME} glad_ext glfw3_ext glm_ext)
