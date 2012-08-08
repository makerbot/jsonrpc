// vim:cindent:cino=\:0:et:fenc=utf-8:ff=unix:sw=4:ts=4:

#ifndef JSONRPC_JSONRPCSTREAM_H
#define JSONRPC_JSONRPCSTREAM_H (1)

#include <cstddef>
#include <string>

#include <json/value.h>

class JsonRpcStream
{
public:
    virtual ~JsonRpcStream (void);

    virtual void feed (char const * buffer, std::size_t length) = 0;
    virtual void feed (std::string const & buffer) = 0;
    virtual void feedeof (void) = 0;
};

#endif
