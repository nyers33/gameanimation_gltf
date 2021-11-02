FetchContent_Declare(
	glew
	URL "https://sourceforge.net/projects/glew/files/glew/2.1.0/glew-2.1.0.zip"
)

FetchContent_GetProperties(glew)
if(NOT glew_POPULATED)
	FetchContent_Populate(glew)
	add_subdirectory(${glew_SOURCE_DIR}/build/cmake ${glew_BINARY_DIR} EXCLUDE_FROM_ALL)
	set_target_properties(glew_s PROPERTIES FOLDER "External Dependencies")
endif()

set(GLEW_INCLUDE_DIRS ${glew_SOURCE_DIR}/include)
set(GLEW_LIBRARY_DIRS ${glew_BINARY_DIR}/libs)
set(GLEW_LIBRARIES glew_s)