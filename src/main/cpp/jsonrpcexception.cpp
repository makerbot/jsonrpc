// vim:cindent:cino=\:0:et:fenc=utf-8:ff=unix:sw=4:ts=4:

#include <string>

#include <json/value.h>

#include <jsonrpc/jsonrpcexception.h>

JsonRpcException::JsonRpcException
    ( std::string const & methodName
    , Json::Value const & params
    , int const code
    , std::string const & message
    , Json::Value const & data
    )
    : std::runtime_error("JsonRpcException")
    , m_methodName (methodName)
    , m_params (params)
    , m_code (code)
    , m_message (message)
    , m_data (data)
{
}

JsonRpcException::~JsonRpcException (void) throw ()
{
}

std::string const &
JsonRpcException::methodName (void) const
{
    return m_methodName;
}

Json::Value const &
JsonRpcException::params (void) const
{
    return m_params;
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
