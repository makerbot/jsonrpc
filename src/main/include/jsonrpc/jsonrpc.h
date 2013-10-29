// Copyright 2013 MakerBot Industries

#ifndef SRC_MAIN_INCLUDE_JSONRPC_JSONRPC_H_
#define SRC_MAIN_INCLUDE_JSONRPC_JSONRPC_H_

#include <jsoncpp/json/value.h>

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
/// endpoint. The invocation will be sent via the current
/// JsonRpcOutputStream object.
///
/// If no output stream is set (which is true after construction, or
/// if setOutputStream is called with a null pointer, or if the
/// weak_ptr expires), then a remote method invocation will throw a
/// JsonRpcInvalidOutputStream exception.
class JsonRpc {
 public:
  /// Construct JsonRpc object with no output stream
  JSONRPC_API JsonRpc();

  /// Destroy JsonRpc object
  JSONRPC_API ~JsonRpc();

  /// Set output stream used for sending data to the other endpoint
  JSONRPC_API void setOutputStream(
      std::weak_ptr<JsonRpcOutputStream> outputStream);

  /// Add a client method that the other endpoint can invoke
  JSONRPC_API void addMethod(
      const std::string &methodName,
      std::weak_ptr<JsonRpcMethod> method);

  /// Invoke a method on the other endpoint
  ///
  /// Note that only a weak reference to the callback is kept. If the
  /// callback is no longer valid when a response is received, the
  /// response will be dropped.
  ///
  /// If the current output stream is invalid, an InvalidOutputStream
  /// exception is thrown.
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
  JsonRpc(const JsonRpc &other);

  JsonRpc &operator=(const JsonRpc &);

  std::unique_ptr<JsonRpcPrivate> m_private;
};

#endif  // SRC_MAIN_INCLUDE_JSONRPC_JSONRPC_H_
