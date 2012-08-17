// vim:cindent:cino=\:0:et:fenc=utf-8:ff=unix:sw=4:ts=4:

#ifndef JSONRPC_JSONRPC_H
#define JSONRPC_JSONRPC_H (1)

#include <cstddef>
#include <string>

#include <json/value.h>

#include <jsonrpc/jsonrpccallback.h>
#include <jsonrpc/jsonrpcmethod.h>
#include <jsonrpc/jsonrpcstream.h>

class JsonRpcPrivate;

class JsonRpc : public JsonRpcStream
{
public:
    JsonRpc (JsonRpcStream * output);
    ~JsonRpc (void);

    void addMethod
        ( std::string const & methodName
        , JsonRpcMethod * method
        );

    void invoke
        ( std::string const & methodName
        , Json::Value const & params
        , JsonRpcCallback * callback = 0
        );

    void feed (char const * buffer, std::size_t length);
    void feed (std::string const & buffer);
    void feedeof (void);

private:
    JsonRpcPrivate * const m_private;

    // Disable copy constructor and assignment.

    JsonRpc (JsonRpc const &);
    JsonRpc & operator= (JsonRpc const &);
};

#endif
