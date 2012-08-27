// vim:cindent:cino=\:0:et:fenc=utf-8:ff=unix:sw=4:ts=4:

#include <string>

#include <json/reader.h>
#include <json/value.h>
#include <json/writer.h>

#include <jsonrpc/jsonrpcmethod.h>
#include <jsonrpc/jsonrpcstream.h>

#include "jsonrpcprivate.h"

JsonRpcPrivate::JsonRpcPrivate (JsonRpcStream * const output)
    : m_output (output)
    , m_jsonReader (* this)
    , m_idCounter (0)
{
}

void
JsonRpcPrivate::addMethod
    ( std::string const & name
    , JsonRpcMethod * const method
    )
{
    this->m_methods[name] = method;
}

void
JsonRpcPrivate::invoke
    ( std::string const & methodName
    , Json::Value const & params
    , JsonRpcCallback * const callback
    )
{
    Json::Value request (Json::objectValue);
    request["jsonrpc"] = Json::Value ("2.0");
    request["method"] = Json::Value (methodName);
    request["params"] = params;
    Json::Value id;
    if (0 != callback)
    {
        id = Json::Value (this->m_idCounter);
        this->m_idCounter += 1;
        request["id"] = id;
    }
    Json::FastWriter writer;
    std::string const string (writer.write (request));
    this->m_output->feed (string);
    if (0 != callback and not id.isNull ())
    {
        this->m_callbacks[id] = callback;
    }
}

void
JsonRpcPrivate::feed (char const * const buffer, std::size_t const length)
{
    this->m_jsonReader.feed (buffer, length);
}

void
JsonRpcPrivate::feed (std::string const & buffer)
{
    this->m_jsonReader.feed (buffer);
}

void
JsonRpcPrivate::feedeof (void)
{
    this->m_jsonReader.feedeof ();
}

Json::Value
JsonRpcPrivate::errorResponse
    ( Json::Value const & id
    , int const code
    , std::string const & message
    , Json::Value const & data
    ) const
{
    Json::Value error (Json::objectValue);
    error["code"] = Json::Value (code);
    error["message"] = Json::Value (message);
    if (not data.isNull ())
    {
        error["data"] = data;
    }
    Json::Value response (Json::objectValue);
    response["jsonrpc"] = Json::Value ("2.0");
    response["error"] = error;
    response["id"] = id;
    return response;
}

Json::Value
JsonRpcPrivate::invalidRequest (Json::Value const & id) const
{
    Json::Value const null;
    Json::Value const response
        ( this->errorResponse (id, -32600, "invalid request", null)
        );
    return response;
}

Json::Value
JsonRpcPrivate::methodNotFound (Json::Value const & id) const
{
    Json::Value const null;
    Json::Value const result
        ( this->errorResponse (id, -32601, "method not found", null)
        );
    return result;
}

Json::Value
JsonRpcPrivate::parseError (void) const
{
    Json::Value const null;
    Json::Value const result
        ( this->errorResponse (null, -32700, "parse error", null)
        );
    return result;
}

bool
JsonRpcPrivate::isRequest (Json::Value const & value)
{
    bool const result
        ( value.isMember ("jsonrpc")
          and Json::Value ("2.0") == value["jsonrpc"]
          and value.isMember ("method")
          and value["method"].isString ()
        );
    return result;
}

bool
JsonRpcPrivate::isSuccessResponse (Json::Value const & value)
{
    bool const result
        ( value.isMember ("jsonrpc")
          and Json::Value ("2.0") == value["jsonrpc"]
          and value.isMember ("result")
        );
    return result;
}

bool
JsonRpcPrivate::isErrorResponse (Json::Value const & value)
{
    bool const result
        ( value.isMember ("jsonrpc")
          and Json::Value ("2.0") == value["jsonrpc"]
          and value.isMember ("error")
        );
    return result;
}

bool
JsonRpcPrivate::isResponse (Json::Value const & value)
{
    bool const result
        ( this->isSuccessResponse (value)
          or this->isErrorResponse (value)
        );
    return result;
}

Json::Value
JsonRpcPrivate::successResponse
    ( Json::Value const & id
    , Json::Value const & result
    ) const
{
    Json::Value response (Json::objectValue);
    response["jsonrpc"] = Json::Value ("2.0");
    response["result"] = result;
    response["id"] = id;
    return response;
}

Json::Value
JsonRpcPrivate::handleRequest
    ( Json::Value const & request
    , Json::Value const & id
    )
{
    std::string const methodName (request["method"].asString ());
    methods_type::const_iterator const i (this->m_methods.find (methodName));
    Json::Value response;
    if (i == this->m_methods.end ())
    {
        response = this->methodNotFound (id);
    }
    else
    {
        JsonRpcMethod * const method (i->second);
        Json::Value params;
        if (request.isMember ("params"))
        {
            params = request["params"];
        }
        try
        {
            response = this->successResponse (id, method->invoke (params));
        }
        catch (JsonRpcException const & exception)
        {
            response = this->errorResponse
                ( id
                , exception.code ()
                , exception.message ()
                , exception.data ()
                );
        }
    }
    return response;
}

void
JsonRpcPrivate::handleResponse
    ( Json::Value const & response
    , Json::Value const & id
    )
{
    callbacks_type::iterator i (this->m_callbacks.find (id));
    if (i != this->m_callbacks.end ())
    {
        JsonRpcCallback * const callback (i->second);
        this->m_callbacks.erase (i);
        callback->response (response);
    }
}

Json::Value
JsonRpcPrivate::handleObject (Json::Value const & jsonObject)
{
    Json::Value response;
    if (not jsonObject.isObject ())
    {
        Json::Value const null;
        response = this->invalidRequest (null);
    }
    else
    {
        Json::Value const id (jsonObject["id"]);
        if (this->isRequest (jsonObject))
        {
            response = this->handleRequest (jsonObject, id);
        }
        else
        if (this->isResponse (jsonObject))
        {
            Json::Value const null;
            response = null;
            this->handleResponse (jsonObject, id);
        }
        else
        {
            response = this->invalidRequest (id);
        }
    }
    return response;
}

Json::Value
JsonRpcPrivate::handleArray (Json::Value const & jsonArray)
{
    Json::Value response;
    if (0 == jsonArray.size ())
    {
        Json::Value const null;
        response = this->invalidRequest (null);
    }
    else
    {
        Json::Value response (Json::arrayValue);
        for ( Json::Value::const_iterator i (jsonArray.begin ())
            ; i != jsonArray.end ()
            ; ++i
            )
        {
            Json::Value const subrequest (* i);
            Json::Value const subresponse (this->handleObject (subrequest));
            if (not subresponse.isNull ())
            {
                response.append (subresponse);
            }
        }
        if (0 == response.size ())
        {
            Json::Value const null;
            response = null;
        }
    }
    return response;
}

void
JsonRpcPrivate::jsonReaderCallback (std::string const & jsonText)
{
    Json::Value request;
    Json::Value response;
    Json::Reader reader;
    bool const success (reader.parse (jsonText, request));
    if (not success)
    {
        response = this->parseError ();
    }
    else
    if (request.isObject ())
    {
        response = this->handleObject (request);
    }
    else
    if (request.isArray ())
    {
        response = this->handleArray (request);
    }
    else
    {
        Json::Value const null;
        response = this->invalidRequest (null);
    }

    if (not response.isNull ())
    {
        Json::FastWriter writer;
        std::string const string (writer.write (response));
        this->m_output->feed (string);
    }
}

