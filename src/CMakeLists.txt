cmake_minimum_required(VERSION 4.0)


# Langauges default to C and C++. Since this project only builds from C, we
# explicity set that as the language.
project(pyco
	VERSION 0.0.0
	LANGUAGES C
)


add_library(lexer Lexer/lexer.c)
add_library(parser Parser/parser.c)
add_library(is Code_Generator/code_generator.c)
add_library(testing Testing/testing.c)
add_executable(pyco main.c)


target_link_libraries(parser PRIVATE lexer)


target_link_libraries(is PRIVATE parser)


target_link_libraries(pyco
	PRIVATE is
	PRIVATE testing
)
