// Copyright 2013 MakerBot Industries

#ifndef SRC_MAIN_INCLUDE_JSONRPC_JSONRPCEXCEPTION_H_
#define SRC_MAIN_INCLUDE_JSONRPC_JSONRPCEXCEPTION_H_

#include <jsoncpp/json/value.h>

#include <stdexcept>
#include <string>

class JSONRPC_API JsonRpcException : public std::runtime_error {
 public:
  JsonRpcException(
      std::string const & methodName,
      Json::Value const & params,
      int code,
      std::string const & message,
      Json::Value const & data);

  virtual ~JsonRpcException() throw();

  Json::Value const & params() const;
  std::string const & methodName() const;
  int code() const;
  std::string const & message() const;
  Json::Value const & data() const;

 private:
  std::string const m_methodName;
  Json::Value const m_params;
  int const m_code;
  std::string const m_message;
  Json::Value const m_data;
};

class JsonRpcInvalidOutputStream : public std::runtime_error {
 public:
  JsonRpcInvalidOutputStream();
};

#endif  // SRC_MAIN_INCLUDE_JSONRPC_JSONRPCEXCEPTION_H_
