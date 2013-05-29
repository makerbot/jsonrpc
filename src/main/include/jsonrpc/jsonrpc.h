// vim:cindent:cino=\:0:et:fenc=utf-8:ff=unix:sw=4:ts=4:

#ifndef JSONRPC_JSONRPC_H
#define JSONRPC_JSONRPC_H (1)

#include <cstddef>
#include <string>

#include <jsoncpp/json/value.h>

#include <jsonrpc/jsonrpcconfig.h>
#include <jsonrpc/jsonrpccallback.h>
#include <jsonrpc/jsonrpcmethod.h>
#include <jsonrpc/jsonrpcstream.h>
#include <jsonrpc/jsonrpcexception.h>

class JsonRpcPrivate;

class JsonRpc : public JsonRpcStream
{
public:
    JSONRPC_API JsonRpc (JsonRpcStream * output);
    JSONRPC_API ~JsonRpc (void);

    JSONRPC_API void addMethod
        ( std::string const & methodName
        , JsonRpcMethod * method
        );

    JSONRPC_API void invoke
        ( std::string const & methodName
        , Json::Value const & params
        , JsonRpcCallback * callback = 0
        );

    JSONRPC_API void feed (char const * buffer, std::size_t length);
    JSONRPC_API void feed (std::string const & buffer);
    JSONRPC_API void feedeof (void);

private:
    JsonRpcPrivate * const m_private;

    // Disable copy constructor and assignment.

    JsonRpc (JsonRpc const &);
    JsonRpc & operator= (JsonRpc const &);
};

#endif
