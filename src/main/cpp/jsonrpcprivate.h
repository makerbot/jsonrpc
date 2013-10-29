// Copyright 2013 MakerBot Industries

#ifndef SRC_MAIN_CPP_JSONRPCPRIVATE_H_
#define SRC_MAIN_CPP_JSONRPCPRIVATE_H_

#include <cstddef>
#include <map>
#include <string>

#include "cpp/jsonreader.h"
#include "jsoncpp/json/value.h"
#include "jsonrpc/jsonrpcexception.h"
#include "jsonrpc/jsonrpcmethod.h"
#include "jsonrpc/jsonrpcstream.h"

class JsonRpcPrivate : public JsonRpcStream {
 public:
  JsonRpcPrivate();

  /// Partial copy constructor
  ///
  /// The methods from 'other' are copied and the new outputStream
  /// is set. Everything else is initialized as with the no-argument
  /// constructor.
  JsonRpcPrivate(
      const JsonRpcPrivate &other,
      std::weak_ptr<JsonRpcOutputStream> outputStream);

  /// On destruction, send failure to any active callbacks
  ~JsonRpcPrivate();

  void addMethod(
      std::string const &name,
      std::weak_ptr<JsonRpcMethod> method);

  void invoke(
      std::string const &,
      Json::Value const &,
      std::weak_ptr<JsonRpcCallback>);

  void feed(char const *, std::size_t);
  void feed(std::string const &);
  void feedeof();

 private:
  Json::Value errorResponse(
      Json::Value const &,
      int,
      std::string const &,
      Json::Value const &) const;
  Json::Value invalidRequest(Json::Value const &) const;
  Json::Value methodNotFound(Json::Value const &) const;
  Json::Value parseError() const;
  bool isRequest(Json::Value const &);
  bool isSuccessResponse(Json::Value const &);
  bool isErrorResponse(Json::Value const &);
  bool isResponse(Json::Value const &);
  Json::Value successResponse(
      Json::Value const &,
      Json::Value const &) const;
  Json::Value handleRequest(Json::Value const &, Json::Value const &);
  void handleResponse(Json::Value const &, Json::Value const &);
  Json::Value handleObject(Json::Value const &);
  Json::Value handleArray(Json::Value const &);
  void jsonReaderCallback(std::string const &);

  std::weak_ptr<JsonRpcOutputStream> m_output;
  JsonReader m_jsonReader;

  std::map<std::string, std::weak_ptr<JsonRpcMethod>> m_methods;

  int m_idCounter;
  std::map<Json::Value, std::weak_ptr<JsonRpcCallback>> m_callbacks;

  friend class JsonReader;

  // Disable copy constructor and assignment

  JsonRpcPrivate(const JsonRpcPrivate &other);
  JsonRpcPrivate & operator=(JsonRpcPrivate const &);
};

#endif  // SRC_MAIN_CPP_JSONRPCPRIVATE_H_
