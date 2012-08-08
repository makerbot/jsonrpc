// vim:cindent:cino=\:0:et:fenc=utf-8:ff=unix:sw=4:ts=4:

#ifndef JSONRPC_JSONRPCCALLBACK_H
#define JSONRPC_JSONRPCCALLBACK_H (1)

#include <json/value.h>

class JsonRpcCallback
{
public:
    virtual ~JsonRpcCallback (void);
    virtual void response (Json::Value const & response);
};

#endif
