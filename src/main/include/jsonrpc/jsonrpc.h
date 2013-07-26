// Copyright 2013 MakerBot Industries

#ifndef JSONRPC_JSONRPC_H_
#define JSONRPC_JSONRPC_H_

#include <jsoncpp/json/value.h>

#include <jsonrpc/jsonrpcconfig.h>
#include <jsonrpc/jsonrpccallback.h>
#include <jsonrpc/jsonrpcmethod.h>
#include <jsonrpc/jsonrpcoutputstream.h>
#include <jsonrpc/jsonrpcstream.h>
#include <jsonrpc/jsonrpcexception.h>

#include <memory>
#include <string>

class JsonRpcPrivate;

/// Public interface for a JSON-RPC endpoint
///
/// This class does not directly handle socket IO. It does handle
/// parsing input from, and formatting output to, the other endpoint.
///
/// When data is received from the other endpoint, it should be passed
/// into the JsonRpc object via one of the feedInput() overloaded
/// methods.
///
/// The invoke() method is used to invoke a method on the other
/// endpoint. The invocation will be sent via the JsonRpcStream
/// subclass passed in to the constructor.
class JsonRpc {
 public:
  /// Construct JsonRpc object
  ///
  /// The 'output' stream is used to send data to the other endpoint.
  JSONRPC_API JsonRpc(JsonRpcOutputStream * const output);

  /// Destroy JsonRpc object
  JSONRPC_API ~JsonRpc();

  /// Add a client method that the other endpoint can invoke
  JSONRPC_API void addMethod(
      const std::string &methodName,
      JsonRpcMethod * const method);

  /// Invoke a method on the other endpoint
  ///
  /// Note that only a weak reference to the callback is kept. If the
  /// callback is no longer valid when a response is received, the
  /// response will be dropped.
  JSONRPC_API void invoke(
      const std::string &methodName,
      const Json::Value &params,
      std::weak_ptr<JsonRpcCallback> callback);

  /// Input data received from the other endpoint to JsonReader
  JSONRPC_API void feedInput(
      const char * const buffer,
      const std::size_t length);

  /// Input data received from the other endpoint to JsonReader
  JSONRPC_API void feedInput(const std::string &buffer);

  /// Input end-of-file received from the other endpoint to JsonReader
  JSONRPC_API void feedEOF();

 private:
  JsonRpc(JsonRpc const &);

  JsonRpc &operator=(const JsonRpc &);

  std::unique_ptr<JsonRpcPrivate> m_private;
};

#endif  // JSONRPC_JSONRPC_H_
