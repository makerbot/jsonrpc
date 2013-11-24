// Copyright 2013 MakerBot Industries

#ifndef SRC_MAIN_INCLUDE_JSONRPC_JSONRPCCALLBACK_H_
#define SRC_MAIN_INCLUDE_JSONRPC_JSONRPCCALLBACK_H_

#include <jsoncpp/json/value.h>

class JSONRPC_API JsonRpcCallback {
 public:
  virtual ~JsonRpcCallback();
  virtual void response(Json::Value const & response) = 0;
};

#endif  // SRC_MAIN_INCLUDE_JSONRPC_JSONRPCCALLBACK_H_
