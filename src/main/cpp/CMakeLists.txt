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
add_library(jsonrpc ${LIB_TYPE} ${sources} ${headers})
target_link_libraries(jsonrpc
    jsoncpp)
