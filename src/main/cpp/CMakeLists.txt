
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

add_api(JSONRPC_API)

add_compiler_flags()

add_library(jsonrpc SHARED ${sources} ${headers})

target_link_libraries(
    jsonrpc
    JsonCpp::jsoncpp)

install_config(
    NAME JsonRpc
    TARGET jsonrpc
    API JSONRPC_API)

install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/src/main/include/jsonrpc"
    DESTINATION "${HEADER_INSTALL_DESTINATION}")
