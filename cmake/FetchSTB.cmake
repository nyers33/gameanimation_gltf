FetchContent_Declare(
	stb
	GIT_REPOSITORY "https://github.com/nothings/stb.git"
	GIT_TAG "origin/master"
	GIT_SHALLOW ON
)

FetchContent_MakeAvailable(stb)
set(STB_INCLUDE_DIRS ${stb_SOURCE_DIR})