// Copyright 2013 MakerBot Industries

#ifndef SRC_MAIN_INCLUDE_JSONRPC_JSONRPCOUTPUTSTREAM_H_
#define SRC_MAIN_INCLUDE_JSONRPC_JSONRPCOUTPUTSTREAM_H_

#include <string>

#include "jsoncpp/json/value.h"

/// Abstract interface for sending data to the other endpoint
class JsonRpcOutputStream {
 public:
  JSONRPC_API virtual ~JsonRpcOutputStream();

  /// Send 'buffer' to the other endpoint
  JSONRPC_API virtual void send(const std::string &buffer) = 0;
};

#endif  // SRC_MAIN_INCLUDE_JSONRPC_JSONRPCOUTPUTSTREAM_H_
