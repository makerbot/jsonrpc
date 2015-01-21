message("Configureing jsonrpc")
cmake_minimum_required(VERSION 2.8.3)
project(jsonrpc)
set(MBCMakeTools_DIR ${PROJECT_SOURCE_DIR}/../../mw-cmake-tools/)
find_package(
    MBCMakeTools
    REQUIRED)
setup_environment()

add_compiler_flags()

add_api(JSONRPC)
add_import_api(JSON)

set_shared(true)
install_headers(src/main/include/)

include_directories(${INCLUDE_OUTPUT_PATH})
include_directories(src/main)
add_subdirectory(src/main/cpp)
