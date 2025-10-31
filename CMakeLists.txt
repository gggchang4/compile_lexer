cmake_minimum_required(VERSION 3.10)
project(ToyCLexer)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(lexer main.cpp lexer.cpp lexer.h)

# Set output name to match expected executable name
set_target_properties(lexer PROPERTIES OUTPUT_NAME "lexer")

# Compiler flags
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(lexer PRIVATE -Wall -Wextra -O2)
endif()

# For Windows
if(WIN32)
    set_target_properties(lexer PROPERTIES OUTPUT_NAME "lexer.exe")
endif()
