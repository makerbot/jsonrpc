// vim:cindent:cino=\:0:et:fenc=utf-8:ff=unix:sw=4:ts=4:

#ifndef JSONRPC_JSONRPCMETHOD_H
#define JSONRPC_JSONRPCMETHOD_H (1)

#include <jsoncpp/json/value.h>

class JsonRpcMethod
{
public:
    JSONRPC_API virtual ~JsonRpcMethod (void);
    JSONRPC_API virtual Json::Value invoke (Json::Value const & params) = 0;
};

#endif
