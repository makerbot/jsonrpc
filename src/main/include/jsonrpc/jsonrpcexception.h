// vim:cindent:cino=\:0:et:fenc=utf-8:ff=unix:sw=4:ts=4:

#ifndef JSONRPC_JSONRPCEXCEPTION_H
#define JSONRPC_JSONRPCEXCEPTION_H (1)

#include <jsoncpp/json/value.h>

class JsonRpcException : public std::exception
{
public:
    JsonRpcException
        ( int code
        , std::string const & message
        , Json::Value const & data
        );
    virtual ~JsonRpcException (void) throw ();

    int code (void) const;
    std::string const & message (void) const;
    Json::Value const & data (void) const;

private:
    int const m_code;
    std::string const m_message;
    Json::Value const m_data;
};

#endif
