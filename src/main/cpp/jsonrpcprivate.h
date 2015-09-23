// Copyright 2013 MakerBot Industries

#ifndef SRC_MAIN_CPP_JSONRPCPRIVATE_H_
#define SRC_MAIN_CPP_JSONRPCPRIVATE_H_

#include <cstddef>
#include <list>
#include <map>
#include <mutex>
#include <string>

#include "cpp/jsonreader.h"
#include "jsoncpp/json/value.h"
#include "jsonrpc/jsonrpcexception.h"
#include "jsonrpc/jsonrpcmethod.h"
#include "jsonrpc/jsonrpcstream.h"

// Since we have APIs that support both weak and shared pointers,
// we use an internal representation for these that can handle
// both.  This class can only be used by locking to a shared
// pointer, but if it is backed by a shared pointer then locking
// will always succeed.
template <class T>
class MaybeWeakPtr;

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

  void removeMethod(std::string const &name);

  void invoke(
      std::string const &,
      Json::Value const &,
      std::weak_ptr<JsonRpcCallback>);

  void invokeShared(
      std::string const &,
      Json::Value const &,
      std::shared_ptr<JsonRpcCallback>);

  void invokeRaw(
      const std::string &,
      const Json::Value &,
      const char*,
      const size_t,
      std::shared_ptr<JsonRpcCallback>);

  void feed(char const *, std::size_t);
  void feed(std::string const &);
  void feedeof();
  void setRawHandler(
      std::shared_ptr<std::function<void(char*, const size_t)> > rawHandler,
      const size_t);

  void sendResponseSuccess(
      JsonRpcMethod::Response &response,
      const Json::Value &result);

  void sendResponseError(
      JsonRpcMethod::Response &response,
      const Json::Value &error);

 private:
  void invokeMaybeWeak(
      std::string const &,
      Json::Value const &,
      MaybeWeakPtr<JsonRpcCallback>);
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

  void sendFormattedResponse(
      JsonRpcMethod::Response &response,
      const Json::Value &formattedResponse);

  std::weak_ptr<JsonRpcOutputStream> m_output;
  JsonReader m_jsonReader;

  std::map<std::string, std::weak_ptr<JsonRpcMethod>> m_methods;

  int m_idCounter;
  std::map<Json::Value, MaybeWeakPtr<JsonRpcCallback>> m_callbacks;
  /// Access to m_callbacks must take this mutex
  std::mutex m_callbacksMutex;

  /// Collection of all method responses that haven't been sent yet
  ///
  /// This is used at destruction time to safely invalidate the
  /// responses so that the client can't try to send a response after
  /// the connection has changed and hit a dangling JsonRpc reference.
  std::list<std::shared_ptr<JsonRpcMethod::Response>> m_methodResponses;

  friend class JsonReader;

  // Disable copy constructor and assignment

  JsonRpcPrivate(const JsonRpcPrivate &other);
  JsonRpcPrivate & operator=(JsonRpcPrivate const &);
};

#endif  // SRC_MAIN_CPP_JSONRPCPRIVATE_H_
