set(sources
    jsonreader.cpp
    jsonreader.h
    jsonrpc.cpp
    jsonrpccallback.cpp
    jsonrpcexception.cpp
    jsonrpcmethod.cpp
    jsonrpcprivate.cpp
    jsonrpcprivate.h
    jsonrpcstream.cpp)
add_library(jsonrpc ${JSONRPC_LIB_TYPE} ${sources})


