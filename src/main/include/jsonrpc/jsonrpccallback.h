// Copyright 2013 MakerBot Industries

#ifndef SRC_MAIN_INCLUDE_JSONRPC_JSONRPCCALLBACK_H_
#define SRC_MAIN_INCLUDE_JSONRPC_JSONRPCCALLBACK_H_

#include <jsoncpp/json/value.h>

class JsonRpcCallback {
 public:
  JSONRPC_API virtual ~JsonRpcCallback();
  JSONRPC_API virtual void response(Json::Value const & response) = 0;
};

#endif  // SRC_MAIN_INCLUDE_JSONRPC_JSONRPCCALLBACK_H_
