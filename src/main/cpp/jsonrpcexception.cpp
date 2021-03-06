// Copyright 2013 MakerBot Industries

#include <string>

#include "jsoncpp/json/value.h"
#include "jsoncpp/json/writer.h"

#include "jsonrpc/jsonrpcexception.h"

/// As a convenience for easy logging, construct a nice what() string
static std::string constructWhatString(
    std::string const & methodName,
    Json::Value const & params,
    std::string const & message,
    Json::Value const & data) {
  return ("JsonRpcException(methodName=" + methodName +
          "\n  params=" + Json::FastWriter().write(params) +
          "\n  message=" + message +
          "\n  data=" + Json::FastWriter().write(data));
}

JsonRpcException::JsonRpcException(
    std::string const & methodName,
    Json::Value const & params,
    int const code,
    std::string const & message,
    Json::Value const & data)
    : std::runtime_error(constructWhatString(
        methodName,
        params,
        message,
        data)),
      m_methodName(methodName),
      m_params(params),
      m_code(code),
      m_message(message),
      m_data(data) {
}

JsonRpcException::~JsonRpcException() throw() {
}

std::string const &JsonRpcException::methodName() const {
    return m_methodName;
}

Json::Value const &JsonRpcException::params() const {
    return m_params;
}

int JsonRpcException::code() const {
    return this->m_code;
}

std::string const &JsonRpcException::message() const {
    return this->m_message;
}

Json::Value const &JsonRpcException::data() const {
    return this->m_data;
}

JsonRpcInvalidOutputStream::JsonRpcInvalidOutputStream()
    : std::runtime_error("Invalid JsonRpcOutputStream") {
}
