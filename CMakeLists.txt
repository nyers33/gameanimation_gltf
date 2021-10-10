cmake_minimum_required(VERSION 3.9)
project(gameanimation_gltf)

message("-- CMAKE_SYSTEM:                ${CMAKE_SYSTEM}")
message("-- CMAKE_SYSTEM_NAME:           ${CMAKE_SYSTEM_NAME}")
message("-- CMAKE_SYSTEM_INFO_FILE:      ${CMAKE_SYSTEM_INFO_FILE}")
message("-- CMAKE_SYSTEM_PROCESSOR:      ${CMAKE_SYSTEM_PROCESSOR}")

message("-- CMAKE_CXX_COMPILER_ID:       ${CMAKE_CXX_COMPILER_ID}")
message("-- CMAKE_CXX_COMPILER_VERSION:  ${CMAKE_CXX_COMPILER_VERSION}")

if(NOT CMAKE_VERSION VERSION_LESS 3.9)
  get_property(_isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
elseif(CMAKE_CONFIGURATION_TYPES)
  set(_isMultiConfig True)
else()
  set(_isMultiConfig False)
endif()

if(_isMultiConfig)
	if(NOT DEFINED CMAKE_CONFIGURATION_TYPES)
		set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
	endif()
	message("-- CMAKE_CONFIGURATION_TYPES:   ${CMAKE_CONFIGURATION_TYPES}")
else()
	if(NOT DEFINED CMAKE_BUILD_TYPE)
		set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
	endif()
	message("-- CMAKE_BUILD_TYPE:            ${CMAKE_BUILD_TYPE}")
endif()

set( CMAKE_VERBOSE_MAKEFILE ON )
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

string(TOUPPER ${CMAKE_CXX_COMPILER_ID} CXX_COMPILER_ID_UPPER)
if(${CXX_COMPILER_ID_UPPER} STREQUAL MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj /MP")
endif()

IF(WIN32)
	add_definitions(-DNOMINMAX)
	add_definitions(-D_USE_MATH_DEFINES)
	add_definitions(-D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS)
ENDIF(WIN32)

include(FetchContent)

### Adding GLEW
#add_subdirectory(./src_extern/glew-2.1.0)
FetchContent_Declare(
    glew
    URL "https://sourceforge.net/projects/glew/files/glew/2.1.0/glew-2.1.0.zip"
	SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/glew
)
FetchContent_GetProperties(glew)
if(NOT glew_POPULATED)
    FetchContent_Populate(glew)
    # top level doesn't contain the CMakeLists.txt, it is in the "build/cmake" subdirectory
    add_subdirectory(${glew_SOURCE_DIR}/build/cmake ${glew_BINARY_DIR} EXCLUDE_FROM_ALL)
	set_target_properties(glew_s PROPERTIES FOLDER "External Dependencies")
endif()
set(GLEW_INCLUDE_DIRS ${glew_SOURCE_DIR}/include)
set(GLEW_LIBRARY_DIRS ${glew_BINARY_DIR}/libs)
set(GLEW_LIBRARIES glew_s)

### Adding GLFW
#add_subdirectory(./src_extern/glfw-3.3.2)
FetchContent_Declare(
    glfw
    URL "https://github.com/glfw/glfw/releases/download/3.3.2/glfw-3.3.2.zip"
	SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/glfw
)
if(NOT glfw_POPULATED)
    FetchContent_Populate(glfw)
    set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
    set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
    set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
    set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
    add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR} EXCLUDE_FROM_ALL)
	set_target_properties(glfw PROPERTIES FOLDER "External Dependencies")
endif()

find_package(OpenGL REQUIRED)

add_subdirectory(./Code)
