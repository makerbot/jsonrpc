// Copyright 2013 MakerBot Industries

#include <cstddef>
#include <string>

#include "cpp/jsonrpcprivate.h"
#include "jsonrpc/jsonrpc.h"

JsonRpc::JsonRpc()
    : m_private(new JsonRpcPrivate()) {
}

JsonRpc::~JsonRpc() {
}

void JsonRpc::setOutputStream(
    std::weak_ptr<JsonRpcOutputStream> outputStream) {
  m_private.reset(new JsonRpcPrivate(*m_private, outputStream));
}

void JsonRpc::addMethod(
    const std::string &methodName,
    std::weak_ptr<JsonRpcMethod> method) {
  m_private->addMethod(methodName, method);
}

void JsonRpc::removeMethod(const std::string &methodName) {
  m_private->removeMethod(methodName);
}

void JsonRpc::invoke(
    const std::string &methodName,
    const Json::Value &params,
    std::weak_ptr<JsonRpcCallback> callback) {
  m_private->invoke(methodName, params, callback);
}

void JsonRpc::invokeShared(
    const std::string &methodName,
    const Json::Value &params,
    std::shared_ptr<JsonRpcCallback> callback) {
  m_private->invokeShared(methodName, params, callback);
}

void JsonRpc::invokeRaw(
    const std::string &methodName,
    const Json::Value &params,
    const char* block,
    const size_t length,
    std::shared_ptr<JsonRpcCallback> callback) {
  m_private->invokeRaw(
      methodName,
      params,
      block,
      length,
      callback);
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

void JsonRpc::setRawHandler(
    std::shared_ptr<std::function<void(char*, const size_t)> > rawHandler,
    const size_t length) {
  m_private->setRawHandler(rawHandler, length);
}
