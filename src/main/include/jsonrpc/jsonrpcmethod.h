// Copyright 2013 MakerBot Industries

#ifndef SRC_MAIN_INCLUDE_JSONRPC_JSONRPCMETHOD_H_
#define SRC_MAIN_INCLUDE_JSONRPC_JSONRPCMETHOD_H_

#include <memory>

#include "jsoncpp/json/value.h"

class JsonRpcPrivate;

class JsonRpcMethod {
 public:
  /// Client response sender
  ///
  /// When a request comes in, a Response is created and passed to the
  /// JsonRpcMethod invoke() method. The client can immediately send a
  /// response (i.e. a synchronous response) or hold on to the pointer
  /// and send a response at some later time.
  class Response {
   public:
    Response(JsonRpcPrivate *jsonRpcPrivate, const Json::Value &id);

    ~Response();

    /// Get the original request ID
    Json::Value id() const;

    /// Send a successful response
    ///
    /// An exception will be thrown if a response has already been
    /// sent or if the connection is dead.
    JSONRPC_API void sendResult(const Json::Value &result);

    /// Send an error response
    ///
    /// An exception will be thrown if a response has already been
    /// sent or if the connection is dead.
    JSONRPC_API void sendError(const Json::Value &error);

    /// Close the response so that it never gets sent
    ///
    /// This is used if the connection dies.
    void invalidate();

   private:
    class Private;
    std::unique_ptr<Private> m_private;
  };

  virtual ~JsonRpcMethod();

  /// Handle a request
  ///
  /// The Response object must be used to send a response eventually,
  /// either a result or an error. The response can be sent
  /// immediately if the result is ready, or the client can hold on to
  /// the response and send it at some later time.
  JSONRPC_API virtual void invoke(
      const Json::Value &params,
      std::shared_ptr<Response> response) = 0;
};

/// Convenience wrapper for notification handlers
///
/// JSON-RPC notifications are the same as regular requests except no
/// response is expected. This class just provides a more convenient
/// override of invoke() that does not take a response parameter.
class JsonRpcNotification : public JsonRpcMethod {
 public:
  virtual ~JsonRpcNotification();

  /// Handle a request
  JSONRPC_API virtual void invoke(const Json::Value &params) = 0;

  /// Call the other invoke() override without the response
  virtual void invoke(
      const Json::Value &params,
      std::shared_ptr<Response> response) override;
};

#endif  // SRC_MAIN_INCLUDE_JSONRPC_JSONRPCMETHOD_H_
