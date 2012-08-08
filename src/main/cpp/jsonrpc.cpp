// vim:cindent:cino=\:0:et:fenc=utf-8:ff=unix:sw=4:ts=4:

#include <cstddef>
#include <string>

#include <jsonrpc/jsonrpc.h>
#include <jsonrpc/jsonrpccallback.h>
#include <jsonrpc/jsonrpcmethod.h>

#include "jsonrpcprivate.h"

JsonRpc::JsonRpc (JsonRpcStream * const output)
    : m_private (new JsonRpcPrivate (output))
{
}

JsonRpc::~JsonRpc (void)
{
    delete this->m_private;
}

void
JsonRpc::addMethod (std::string const & name, JsonRpcMethod * const method)
{
    this->m_private->addMethod (name, method);
}

void
JsonRpc::invoke
    ( std::string const & name
    , Json::Value const & params
    , JsonRpcCallback * const callback
    )
{
    this->m_private->invoke (name, params, callback);
}

void
JsonRpc::feed (char const * const buffer, std::size_t const length)
{
    this->m_private->feed (buffer, length);
}

void
JsonRpc::feed (std::string const & buffer)
{
    this->m_private->feed (buffer);
}

void
JsonRpc::feedeof (void)
{
    this->m_private->feedeof ();
}
