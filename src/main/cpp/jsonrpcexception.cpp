// vim:cindent:cino=\:0:et:fenc=utf-8:ff=unix:sw=4:ts=4:

#include <string>

#include <jsoncpp/json/value.h>

#include <jsonrpc/jsonrpcexception.h>

JsonRpcException::JsonRpcException
    ( int const code
    , std::string const & message
    , Json::Value const & data
    )
    : m_code (code)
    , m_message (message)
    , m_data (data)
{
}

JsonRpcException::~JsonRpcException (void) throw ()
{
}

int
JsonRpcException::code (void) const
{
    return this->m_code;
}

std::string const &
JsonRpcException::message (void) const
{
    return this->m_message;
}

Json::Value const &
JsonRpcException::data (void) const
{
    return this->m_data;
}
