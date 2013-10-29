// Copyright 2013 MakerBot Industries

#ifndef SRC_MAIN_INCLUDE_JSONRPC_JSONRPCSTREAM_H_
#define SRC_MAIN_INCLUDE_JSONRPC_JSONRPCSTREAM_H_

#include <cstddef>
#include <string>

#include "jsoncpp/json/value.h"

class JsonRpcStream {
 public:
  JSONRPC_API virtual ~JsonRpcStream();

  JSONRPC_API virtual void feed(char const * buffer, std::size_t length) = 0;
  JSONRPC_API virtual void feed(std::string const & buffer) = 0;
  JSONRPC_API virtual void feedeof() = 0;
};

#endif  // SRC_MAIN_INCLUDE_JSONRPC_JSONRPCSTREAM_H_
