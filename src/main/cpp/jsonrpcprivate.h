// vim:cindent:cino=\:0:et:fenc=utf-8:ff=unix:sw=4:ts=4:

#ifndef JSONRPC_JSONRPCPRIVATE_H
#define JSONRPC_JSONRPCPRIVATE_H (1)

#include <cstddef>
#include <map>
#include <string>

#include <jsoncpp/json/value.h>

#include <jsonrpc/jsonrpcexception.h>
#include <jsonrpc/jsonrpcmethod.h>
#include <jsonrpc/jsonrpcstream.h>

#include "jsonreader.h"

class JsonRpcPrivate : public JsonRpcStream
{
public:
    JsonRpcPrivate (JsonRpcOutputStream *);

    void addMethod (std::string const &, JsonRpcMethod *);

    void invoke (std::string const &, Json::Value const &, std::weak_ptr<JsonRpcCallback>);

    void feed (char const *, std::size_t);
    void feed (std::string const &);
    void feedeof (void);

private:
    typedef std::map <std::string, JsonRpcMethod *> methods_type;
    typedef std::map <Json::Value, std::weak_ptr<JsonRpcCallback>> callbacks_type;

    Json::Value errorResponse
        ( Json::Value const &
        , int
        , std::string const &
        , Json::Value const &
        ) const;
    Json::Value invalidRequest (Json::Value const &) const;
    Json::Value methodNotFound (Json::Value const &) const;
    Json::Value parseError (void) const;
    bool isRequest (Json::Value const &);
    bool isSuccessResponse (Json::Value const &);
    bool isErrorResponse (Json::Value const &);
    bool isResponse (Json::Value const &);
    Json::Value successResponse
        ( Json::Value const &
        , Json::Value const &
        ) const;
    Json::Value handleRequest (Json::Value const &, Json::Value const &);
    void handleResponse (Json::Value const &, Json::Value const &);
    Json::Value handleObject (Json::Value const &);
    Json::Value handleArray (Json::Value const &);
    void jsonReaderCallback (std::string const &);

    JsonRpcOutputStream * const m_output;
    JsonReader m_jsonReader;
    methods_type m_methods;
    int m_idCounter;
    callbacks_type m_callbacks;

    friend class JsonReader;

    // Disable copy constructor and assignment

    JsonRpcPrivate (JsonRpcPrivate const &);
    JsonRpcPrivate & operator= (JsonRpcPrivate const &);
};

#endif
