FetchContent_Declare(
	cgltf
	GIT_REPOSITORY "https://github.com/jkuhlmann/cgltf.git"
	GIT_TAG "v1.11"
	GIT_SHALLOW ON
)

FetchContent_MakeAvailable(cgltf)
set(CGLTF_INCLUDE_DIRS ${cgltf_SOURCE_DIR})