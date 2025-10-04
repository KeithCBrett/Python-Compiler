# If we are in a windows enviroment, we use cl as the compiler. Otherwise we
# will use gcc.
cmake_minimum_required(VERSION 3.30)
if (WIN32)
	message(STATUS "Building with cl (Windows detected)")
	set(RUNTIME_OUTPUT_DIRECTORY "../build/release")
else ()
	message(STATUS "Building with gcc (Linux detected)")
endif()


include(proj_version.cmake)


# Langauges default to C and C++. Since this project only builds from C, we
# explicity set that as the language.
project(pyco
	VERSION ${pyco_VER}
	LANGUAGES C
)


# Since our version numbering is automated via commit number, we need git as
# one of our build dependencies.
find_package(Git REQUIRED)


# This section gets the number of commits from git.
execute_process(
	COMMAND ${GIT_EXECUTABLE} rev-list --count main
	RESULT_VARIABLE result
	OUTPUT_VARIABLE num_commits
	ERROR_VARIABLE num_commits
)
if (result)
	message(FATAL_ERROR "failed to get number of commits")
endif()
# Supply the output of the above process to function's result.
set(pyco_NUM_COMMITS ${num_commits})


# Allows for us to make us of the version number in our source code.
# CMake will auto generate get_version_num.c with the correct version numbers
# so that we can use them in our program.
configure_file(Utils/get_version_num.c.in
	${CMAKE_CURRENT_SOURCE_DIR}/Utils/get_version_num.c @ONLY)
add_library(pyco_version STATIC ${CMAKE_CURRENT_SOURCE_DIR}/Utils/get_version_num.c)

add_library(lexer Lexer/lexer.c)
add_library(parser Parser/parser.c)
add_library(generator Code_Generator/code_generator.c)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_library(testing Testing/testing.c)
endif()
add_executable(pyco main.c)


target_link_libraries(parser PRIVATE lexer)

target_link_libraries(generator PRIVATE parser)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_link_libraries(testing PRIVATE generator)
endif()

# Conditional linking of testing apparatus based on whether we are building
# for debug or release.
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_link_libraries(pyco PRIVATE testing)
endif()

# For displaying version number when user compiles something.
target_link_libraries(pyco PRIVATE pyco_version)

target_link_libraries(pyco PRIVATE generator)

# If we are on Windows, we need to pass special flags to cl to allow
# compilation for newer C standards.
# /TC - Forces C compilation as opposed to C++.
# /std:c11 - Forces C11 support.
# /DEBUG - Adds debugging symbols to executable.
if (WIN32)
	target_compile_options(pyco PUBLIC "SHELL:/TC" "SHELL:/std:c11", "SHELL:/DEBUG")
endif()
