// Copyright 2013 MakerBot Industries

#include "jsonrpcprivate.h"

#include <jsonrpc/jsonrpc.h>

#include <cstddef>
#include <string>

JsonRpc::JsonRpc(JsonRpcStream * const output)
    : m_private(new JsonRpcPrivate(output)) {
}

JsonRpc::~JsonRpc() {
}

void JsonRpc::addMethod(
    const std::string &methodName,
    JsonRpcMethod * const method) {
  m_private->addMethod(methodName, method);
}

void JsonRpc::invoke(
    const std::string &methodName,
    const Json::Value &params,
    JsonRpcCallback * callback) {
  m_private->invoke(methodName, params, callback);
}

void JsonRpc::feedInput(
    const char * const buffer,
    const std::size_t length) {
  m_private->feed(buffer, length);
}

void JsonRpc::feedInput(const std::string &buffer) {
  m_private->feed(buffer);
}

void JsonRpc::feedEOF() {
  m_private->feedeof();
}
