// vim:cindent:cino=\:0:et:fenc=utf-8:ff=unix:sw=4:ts=4:

#include <string>

#include <json/value.h>
#include <json/writer.h>

#include <jsonrpc/jsonrpcexception.h>

/// As a convenience for easy logging, construct a nice what() string
static std::string constructWhatString
    ( std::string const & methodName
    , Json::Value const & params
    , std::string const & message
    , Json::Value const & data
    )
{
    return ("JsonRpcException(methodName=" + methodName +
            "\n  params=" + Json::FastWriter().write(params) +
            "\n  message=" + message +
            "\n  data=" + Json::FastWriter().write(data));
}

JsonRpcException::JsonRpcException
    ( std::string const & methodName
    , Json::Value const & params
    , int const code
    , std::string const & message
    , Json::Value const & data
    )
    : std::runtime_error(constructWhatString(methodName,
                                             params,
                                             message,
                                             data))
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
