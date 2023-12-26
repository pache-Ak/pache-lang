cmake_minimum_required(VERSION 3.20.0)
project(pache_lang)

find_package(BISON)
find_package(FLEX)

BISON_TARGET(MyParser src/parser.y ${PROJECT_BINARY_DIR}/parser.tab.cc
            DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.hh)

FLEX_TARGET(MyFlex src/lexer.lpp ${PROJECT_BINARY_DIR}/lexer.cpp)

ADD_FLEX_BISON_DEPENDENCY(MyFlex MyParser)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.

include_directories(${CMAKE_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_BINARY_DIR})


include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

# Now build our tools
add_executable(${PROJECT_NAME}
                main.cpp driver.cpp
                ${BISON_MyParser_OUTPUTS}
                ${FLEX_MyScanner_OUTPUTS}
)

add_subdirectory(ast)
target_link_libraries(${PROJECT_NAME} PUBLIC ast)
target_include_directories(${PROJECT_NAME} PUBLIC
                           ${PROJECT_BINARY_DIR}
                           ${PROJECT_SOURCE_DIR}/ast
                           )

add_subdirectory(IRbuild)
target_link_libraries(${PROJECT_NAME} PUBLIC IRbuild)
target_include_directories(${PROJECT_NAME} PUBLIC
                           ${PROJECT_BINARY_DIR}
                           ${PROJECT_SOURCE_DIR}/IRbuild
                           )





# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader)

# Link against LLVM libraries
target_link_libraries(${PROJECT_NAME} PUBLIC ${llvm_libs})

target_link_libraries(${PROJECT_NAME} PUBLIC ${FLEX_LIBRARIES})
