
find_package(JsonCpp REQUIRED)

set(headers
    jsonreader.h
    jsonrpcprivate.h)

set(sources
    jsonreader.cpp
    jsonrpc.cpp
    jsonrpccallback.cpp
    jsonrpcexception.cpp
    jsonrpcmethod.cpp
    jsonrpcprivate.cpp
    jsonrpcstream.cpp)

add_library(jsonrpc ${sources} ${headers})

use_standard_flags(jsonrpc)

target_include_directories(
    jsonrpc
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/main>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/main/include>
        $<INSTALL_INTERFACE:${HEADER_INSTALL_DIR}>)

export_api(jsonrpc JSONRPC_API)

target_link_libraries(jsonrpc PUBLIC JsonCpp::jsoncpp)

generate_and_install_config(
    NAME JsonRpc
    TARGETS jsonrpc)

install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/src/main/include/jsonrpc"
    DESTINATION "${HEADER_INSTALL_DIR}")
