// Copyright 2013 MakerBot Industries

#ifndef SRC_MAIN_INCLUDE_JSONRPC_JSONRPCOUTPUTSTREAM_H_
#define SRC_MAIN_INCLUDE_JSONRPC_JSONRPCOUTPUTSTREAM_H_

#include <string>

#include "jsoncpp/json/value.h"

/// Abstract interface for sending data to the other endpoint
class JSONRPC_API JsonRpcOutputStream {
 public:
  virtual ~JsonRpcOutputStream();

  /// Send length bytes from buf to the other endpoint
  virtual void send(const char * buf, size_t length) = 0;

  /// Convenience function to send from a std::string
  inline void send(const std::string &buffer) {
    send(buffer.data(), buffer.size());
  }
};

#endif  // SRC_MAIN_INCLUDE_JSONRPC_JSONRPCOUTPUTSTREAM_H_
