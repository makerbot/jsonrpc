// Copyright 2013 MakerBot Industries

#ifndef JSONRPC_JSONRPC_OUTPUT_STREAM_H_
#define JSONRPC_JSONRPC_OUTPUT_STREAM_H_

#include <string>

#include <jsonrpc/jsonrpcconfig.h>
#include <jsoncpp/json/value.h>

/// Abstract interface for sending data to the other endpoint
class JsonRpcOutputStream {
 public:
  JSONRPC_API virtual ~JsonRpcOutputStream();

  /// Send 'buffer' to the other endpoint
  JSONRPC_API virtual void send(const std::string &buffer) = 0;
};

#endif  // JSONRPC_JSONRPC_OUTPUT_STREAM_H_
