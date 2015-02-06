
cmake_minimum_required(VERSION 2.8.3)

project(jsonrpc CXX)

find_package(
    MBCMakeTools
    REQUIRED)

find_package(
    JsonCpp
    REQUIRED)

add_compiler_flags()

add_subdirectory(src/main/cpp)
