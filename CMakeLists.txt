
cmake_minimum_required(VERSION 3.1.0)

project(jsonrpc CXX)

find_package(MBCMakeTools REQUIRED)
find_package(JsonCpp REQUIRED)

add_subdirectory(src/main/cpp)
