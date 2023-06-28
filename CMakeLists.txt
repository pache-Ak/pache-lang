cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project (compiler VERSION 0.01)

find_package(BISON)
find_package(FLEX)

BISON_TARGET(MyParser src/parser.y ${PROJECT_BINARY_DIR}/parser.cpp)

FLEX_TARGET(MyFlex src/lexer.lpp ${PROJECT_BINARY_DIR}/lexer.cpp)

ADD_FLEX_BISON_DEPENDENCY(MyFlex MyParser)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(/usr/include/llvm-14)
include_directories(/usr/include/llvm-c-14)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_compile_options("-Wall -Wno-register `llvm-config --cxxflags --ldflags --system-libs --libs core`")
add_executable(main ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${BISON_MyParser_OUTPUTS}
    ${FLEX_MyScanner_OUTPUTS}
)
target_link_libraries(main PUBLIC ast)
#add_executable(main ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp ${BISON_MyParser_OUTPUTS})



