// vim:cindent:cino=\:0:et:fenc=utf-8:ff=unix:sw=4:ts=4:

#ifndef JSONRPC_JSONRPCCALLBACK_H
#define JSONRPC_JSONRPCCALLBACK_H (1)

#include <jsoncpp/json/value.h>
#include <jsonrpc/jsonrpcconfig.h>

class JsonRpcCallback
{
public:
    JSONRPC_API virtual ~JsonRpcCallback (void);
    JSONRPC_API virtual void response (Json::Value const & response) = 0;
};

#endif
