cmake_minimum_required (VERSION 2.6)

project(jsonrpc)

set(COMIPLE_FLAGS
    -std=c++11
    -Wno-unused-local-typedefs
    -pedantic
    -Wall
    -Wextra
    -Wno-variadic-macros
    -Wno-long-long
    -Wswitch-enum
    -O2)
set(FLAG_STRING "")
foreach(flag ${COMIPLE_FLAGS})
    set(FLAG_STRING "${FLAG_STRING} ${flag}")
endforeach()
set(CMAKE_CXX_FLAGS ${FLAG_STRING})
set(CMAKE_VERBOSE_MAKEFILE true)

option(JSONRPC_LIB_BUILD_SHARED "Build JSONRPC_lib as a shared library." OFF)
if(JSONRPC_LIB_BUILD_SHARED)
  set(JSONRPC_LIB_TYPE SHARED)
  add_definitions( -DJSON_DLL_BUILD )
else()
  set(JSONRPC_LIB_TYPE STATIC)
endif()

add_definitions("\"-DJSONRPC_API=__attribute__ ((visibility (\\\"default\\\")))\"")
add_definitions("\"-DJSON_API=__attribute__ ((visibility (\\\"default\\\")))\"")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../build/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../build/lib)

include_directories(src/main/include)
include_directories(src/main)
add_subdirectory(src/main/cpp)

