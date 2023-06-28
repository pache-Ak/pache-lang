cmake_minimum_required(VERSION 3.22)

aux_source_directory(${PROJECT_SOURCE_DIR}/src/ast AST_SRC)
include_directories(/usr/include/llvm-14)
include_directories(/usr/include/llvm-c-14)

add_compile_options("-Wall -Wno-register `llvm-config --cxxflags --ldflags --system-libs --libs core`")
add_library(ast STATIC ${AST_SRC})
