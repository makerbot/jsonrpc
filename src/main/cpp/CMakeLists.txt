
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

include_directories("${PROJECT_SOURCE_DIR}/src/main")
include_directories("${PROJECT_SOURCE_DIR}/src/main/include")

add_compiler_flags()

add_library(jsonrpc SHARED ${sources} ${headers})

export_api(jsonrpc JSONRPC_API)

target_link_libraries(jsonrpc JsonCpp::jsoncpp)

install_config(
    NAME JsonRpc
    TARGET jsonrpc)

install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/src/main/include/jsonrpc"
    DESTINATION "${HEADER_INSTALL_DIR}")
