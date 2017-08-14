cmake_minimum_required(VERSION 3.8.0)
set(PROJECT_NAME Ripples)
project(${PROJECT_NAME})
set(${PROJECT_NAME}_VERSION 1.0.0)

include (ExternalProject)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/lib)

# Organize unrelated targets to clean IDE hierarchy.
set(DEPENDENCY_FOLDER "Dependencies")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER ${DEPENDENCY_FOLDER})
# set(CMAKE_SUPPRESS_REGENERATION TRUE)

# Output binary to predictable location (fixes cyclic dependency issues).
set(BINARY_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_OUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BINARY_OUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BINARY_OUT_DIR})

foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
	string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BINARY_OUT_DIR})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BINARY_OUT_DIR})
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BINARY_OUT_DIR})
endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)


set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_BUILD_TYPE Release)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c" "src/*.hpp" "src/*.h")
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE src)

# target_compile_definitions(${PROJECT_NAME} PUBLIC _HAS_EXCEPTIONS=0
#	_STATIC_CPPLIB=1 ENABLE_PROFILING=0)

# set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY
#	VS_STARTUP_PROJECT ${PROJECT_NAME})

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra #-Wpedantic
		-Werror -fno-exceptions)# -fno-rtti)

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra #-Wpedantic
		-Werror -fno-exceptions)# -fno-rtti)

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # using Intel C++

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	string(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")
	target_compile_options(${PROJECT_NAME} PRIVATE /W3)# /WX)# /GR-)

endif()

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})

##
# Platform dependencies.
##
if (APPLE)
	find_library(COCOA_LIB Cocoa)
	find_library(IOKIT_LIB IOKit)
	find_library(COREVIDEO_LIB CoreVideo)
	target_link_libraries(${PROJECT_NAME} PRIVATE ${COCOA_LIB}
		${IOKIT_LIB} ${COREVIDEO_LIB})
endif()

##
# Forward cmake args to external dependencies.
##
get_cmake_property(CACHE_VARS CACHE_VARIABLES)
foreach(CACHE_VAR ${CACHE_VARS})
	get_property(CACHE_VAR_HELPSTRING CACHE ${CACHE_VAR} PROPERTY HELPSTRING)

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

##
# External dependencies.
##
set(LIB_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${LIB_INCLUDE_DIR})

ExternalProject_Add(glad_ext
	GIT_REPOSITORY https://github.com/Dav1dde/glad.git
	GIT_TAG v0.1.14a0
	CMAKE_ARGS ${CMAKE_ARGS}
		-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}
		-DCMAKE_PREFIX_PATH=${CMAKE_CURRENT_BINARY_DIR}
		-DGLAD_PROFILE="core"
		-DGLAD_API="gl=4.1"
		-DGLAD_INSTALL=On
	UPDATE_COMMAND ""
)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)

ExternalProject_Add(glm_ext
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG 0.9.8.4
	CMAKE_ARGS ${CMAKE_ARGS}
		-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}
		-DCMAKE_PREFIX_PATH=${CMAKE_CURRENT_BINARY_DIR}
	UPDATE_COMMAND ""
)
target_link_libraries(${PROJECT_NAME} INTERFACE glm)

ExternalProject_Add(glfw3_ext
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG 3.2.1
	CMAKE_ARGS ${CMAKE_ARGS}
		-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}
		-DCMAKE_PREFIX_PATH=${CMAKE_CURRENT_BINARY_DIR}
		-DGLFW_BUILD_DOCS=Off
		-DGLFW_BUILD_TESTS=Off
		-DGLFW_BUILD_EXAMPLES=Off
	UPDATE_COMMAND ""
)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw3)

ExternalProject_Add(stb_ext
	GIT_REPOSITORY https://github.com/nothings/stb.git
	CONFIGURE_COMMAND ""
	BUILD_COMMAND
		COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>
				${LIB_INCLUDE_DIR}/stb
		COMMAND ${CMAKE_COMMAND} -E remove_directory ${LIB_INCLUDE_DIR}/stb/.git
		COMMAND ${CMAKE_COMMAND} -E remove_directory ${LIB_INCLUDE_DIR}/stb/.github
		COMMAND ${CMAKE_COMMAND} -E remove_directory ${LIB_INCLUDE_DIR}/stb/data
		COMMAND ${CMAKE_COMMAND} -E remove_directory
				${LIB_INCLUDE_DIR}/stb/deprecated
		COMMAND ${CMAKE_COMMAND} -E remove_directory ${LIB_INCLUDE_DIR}/stb/docs
		COMMAND ${CMAKE_COMMAND} -E remove_directory ${LIB_INCLUDE_DIR}/stb/tests
		COMMAND ${CMAKE_COMMAND} -E remove_directory ${LIB_INCLUDE_DIR}/stb/tools
		COMMAND ${CMAKE_COMMAND} -E remove ${LIB_INCLUDE_DIR}/stb/.travis.yml
		COMMAND ${CMAKE_COMMAND} -E remove ${LIB_INCLUDE_DIR}/stb/README.md
	INSTALL_COMMAND ""
	UPDATE_COMMAND ""
)
set_target_properties(glad_ext glfw3_ext glm_ext stb_ext PROPERTIES
		FOLDER ${DEPENDENCY_FOLDER})
add_dependencies(${PROJECT_NAME} glad_ext glfw3_ext glm_ext stb_ext)

##
# Copy shaders on build.
##
set(SHADER_IN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders)
set(SHADER_OUT_DIR ${BINARY_OUT_DIR}/shaders)
set(STAMP_DIR ${CMAKE_CURRENT_BINARY_DIR}/.stamps)

file(GLOB_RECURSE SHADERS "${SHADER_IN_DIR}/*.glsl")

set(STAMP_FILES "")
foreach(SHADER ${SHADERS})
	get_filename_component(SHADER_FILENAME ${SHADER} NAME)
	set(STAMP_FILE ${STAMP_DIR}/${SHADER_FILENAME}.stamp)
	add_custom_command(
		OUTPUT ${STAMP_FILE}
		COMMAND ${CMAKE_COMMAND} -E make_directory ${STAMP_DIR}
		COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADER_OUT_DIR}
		COMMAND ${CMAKE_COMMAND} -E touch ${STAMP_FILE}
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SHADER}
				${SHADER_OUT_DIR}/${SHADER_FILENAME}
		DEPENDS ${SHADER}
	)
	list(APPEND STAMP_FILES ${STAMP_FILE})
endforeach()

# Display shaders in IDE, in main project folder.
add_custom_target(Shaders
	SOURCES ${SHADERS}
	DEPENDS ${STAMP_FILES})
# set_target_properties(Shaders PROPERTIES FOLDER ${PROJECT_NAME}) # Pretty hacky :) Doesn't work on VS
add_dependencies(${PROJECT_NAME} Shaders)
