// Copyright 2013 MakerBot Industries

#ifndef SRC_MAIN_INCLUDE_JSONRPC_JSONRPCMETHOD_H_
#define SRC_MAIN_INCLUDE_JSONRPC_JSONRPCMETHOD_H_

#include "jsoncpp/json/value.h"

class JsonRpcMethod {
 public:
  JSONRPC_API virtual ~JsonRpcMethod(void);
  JSONRPC_API virtual Json::Value invoke(Json::Value const & params) = 0;
};

#endif  // SRC_MAIN_INCLUDE_JSONRPC_JSONRPCMETHOD_H_
