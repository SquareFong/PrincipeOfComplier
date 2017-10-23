cmake_minimum_required(VERSION 3.8)
project(PrincipleOfComplier_FirstProgram)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp RegexToNFA.h NFAtoDFA.h)
add_executable(PrincipleOfComplier_FirstProgram ${SOURCE_FILES})