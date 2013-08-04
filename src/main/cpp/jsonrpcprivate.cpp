// vim:cindent:cino=\:0:et:fenc=utf-8:ff=unix:sw=4:ts=4:

#include <string>

#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/writer.h>

#include <jsonrpc/jsonrpcmethod.h>
#include <jsonrpc/jsonrpcstream.h>

#include "jsonrpcprivate.h"

JsonRpcPrivate::JsonRpcPrivate ()
    : m_jsonReader (* this)
    , m_idCounter (0)
{
}

JsonRpcPrivate::JsonRpcPrivate (
    const JsonRpcPrivate &other,
    std::weak_ptr<JsonRpcOutputStream> outputStream)
    : m_output(outputStream),
      m_jsonReader (*this),
      m_methods(other.m_methods),
      m_idCounter(0) {
}

JsonRpcPrivate::~JsonRpcPrivate () {
  for (auto pair : m_callbacks) {
    auto callback(pair.second.lock());
    // If the callback is still valid, send error
    //
    // TODO(nicholasbishop): for now, just sending a null response,
    // which should be interpreted as invalid by the callback. Could
    // look into sending a more "correct" invalid response specified
    // by the JSON-RPC spec.
    if (callback) {
      callback->response(Json::nullValue);
    }
  }
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
    , std::weak_ptr<JsonRpcCallback> callback
    )
{
    Json::Value request (Json::objectValue);
    request["jsonrpc"] = Json::Value ("2.0");
    request["method"] = Json::Value (methodName);
    request["params"] = params;
    Json::Value id;
    if (!callback.expired())
    {
        id = Json::Value (this->m_idCounter);
        this->m_idCounter += 1;
        request["id"] = id;

        // Add callback to handle the response
        if (!id.isNull ())
        {
            this->m_callbacks[id] = callback;
        }
    }

    // Send the request
    Json::FastWriter writer;
    std::string const string (writer.write (request));

    auto output(m_output.lock());
    if (output) {
      output->send (string);
    } else {
      throw JsonRpcInvalidOutputStream();
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
    if (!data.isNull ())
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
          && Json::Value ("2.0") == value["jsonrpc"]
          && value.isMember ("method")
          && value["method"].isString ()
        );
    return result;
}

bool
JsonRpcPrivate::isSuccessResponse (Json::Value const & value)
{
    bool const result
        ( value.isMember ("jsonrpc")
          && Json::Value ("2.0") == value["jsonrpc"]
          && value.isMember ("result")
        );
    return result;
}

bool
JsonRpcPrivate::isErrorResponse (Json::Value const & value)
{
    bool const result
        ( value.isMember ("jsonrpc")
          && Json::Value ("2.0") == value["jsonrpc"]
          && value.isMember ("error")
        );
    return result;
}

bool
JsonRpcPrivate::isResponse (Json::Value const & value)
{
    bool const result
        ( this->isSuccessResponse (value)
          || this->isErrorResponse (value)
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
        // Get the shared pointer to the callback
        auto callback(i->second.lock());
        // Remove the weak pointer to the callback
        this->m_callbacks.erase (i);
        // If the callback is still valid, send the response
        if (callback)
        {
            callback->response (response);
        }
    }
}

Json::Value
JsonRpcPrivate::handleObject (Json::Value const & jsonObject)
{
    Json::Value response;
    if (!jsonObject.isObject ())
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
            if (!subresponse.isNull ())
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
    if (!success)
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

    if (!response.isNull ())
    {
        Json::FastWriter writer;
        std::string const string (writer.write (response));

        auto output(m_output.lock());
        // Silently fail if there's no output stream
        if (output) {
          output->send (string);
        }
    }
}

