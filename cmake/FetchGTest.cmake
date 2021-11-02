FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG "release-1.11.0"
	GIT_SHALLOW ON
)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
	FetchContent_Populate(googletest)

	set(BUILD_GTEST ON CACHE BOOL "" FORCE)
	set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
	set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
	
	mark_as_advanced(BUILD_GTEST)
	mark_as_advanced(BUILD_GMOCK)
	mark_as_advanced(INSTALL_GTEST)

	if(WIN32)
		set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	endif(WIN32)

	add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

mark_as_advanced(
	gmock_build_tests
	gtest_build_samples
	gtest_build_tests
	gtest_disable_pthreads
	gtest_force_shared_crt
	gtest_hide_internal_symbols
)

set_target_properties(gtest gtest_main PROPERTIES FOLDER "External Dependencies")
