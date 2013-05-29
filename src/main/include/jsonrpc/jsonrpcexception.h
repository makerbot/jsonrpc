// vim:cindent:cino=\:0:et:fenc=utf-8:ff=unix:sw=4:ts=4:

#ifndef JSONRPC_JSONRPCEXCEPTION_H
#define JSONRPC_JSONRPCEXCEPTION_H (1)

#include <jsonrpc/jsonrpcconfig.h>
#include <jsoncpp/json/value.h>

#include <stdexcept>

class JsonRpcException : public std::runtime_error
{
public:
    JSONRPC_API JsonRpcException
        ( std::string const & methodName
        , Json::Value const & params
        , int code
        , std::string const & message
        , Json::Value const & data
        );
    JSONRPC_API virtual ~JsonRpcException (void) throw ();

    JSONRPC_API Json::Value const & params (void) const;
    JSONRPC_API std::string const & methodName (void) const;
    JSONRPC_API int code (void) const;
    JSONRPC_API std::string const & message (void) const;
    JSONRPC_API Json::Value const & data (void) const;

private:
    std::string const m_methodName;
    Json::Value const m_params;
    int const m_code;
    std::string const m_message;
    Json::Value const m_data;
};

#endif
