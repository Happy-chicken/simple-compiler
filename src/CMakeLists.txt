cmake_minimum_required(VERSION 3.8)

project(Compiler)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SRC_FILES lexical_analysis.cpp syntax_analysis.cpp main.cpp)

add_subdirectory(lex)
add_subdirectory(syn)

add_executable (Compiler main.cpp)

target_include_directories(Compiler PUBLIC  
                         ${PROJECT_SOURCE_DIR}/lex
                         ${PROJECT_SOURCE_DIR}/syn
	         include
)

target_link_libraries(Compiler PUBLIC lex syn)