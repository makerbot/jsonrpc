// Copyright 2013 MakerBot Industries

#include <mutex>
#include <stdexcept>

#include "cpp/jsonrpcprivate.h"
#include "jsonrpc/jsonrpcmethod.h"

class JsonRpcMethod::Response::Private {
 public:
  Private(JsonRpcPrivate *jsonRpcPrivate, const Json::Value &id)
      : m_jsonRpcPrivate(jsonRpcPrivate),
        m_id(id),
        m_sent(false) {
  }

  /// Throw an exception if the reponse can't be sent
  void ensureValid() {
    if (m_sent) {
      throw std::runtime_error("Response already sent");
    } else if (!m_jsonRpcPrivate) {
      throw std::runtime_error("Response connection died");
    }
  }

  /// Internal mutex used by all the public methods of Response
  ///
  /// The primary purpose is to ensure that m_jsonRpcPrivate doesn't
  /// get invalidated during sending. It also ensures that multiple
  /// threads can't cause multiple responses to be sent.
  std::mutex m_mutex;

  JsonRpcPrivate *m_jsonRpcPrivate;

  /// The original request ID
  const Json::Value m_id;

  /// Whether the response has already been sent
  bool m_sent;
};

JsonRpcMethod::Response::Response(
    JsonRpcPrivate *jsonRpcPrivate,
    const Json::Value &id)
    : m_private(new Private(jsonRpcPrivate, id)) {
}

JsonRpcMethod::Response::~Response() {
}

Json::Value JsonRpcMethod::Response::id() const {
  return m_private->m_id;
}

void JsonRpcMethod::Response::sendResult(const Json::Value &result) {
  std::lock_guard<std::mutex> lock(m_private->m_mutex);
  m_private->ensureValid();
  m_private->m_sent = true;
  m_private->m_jsonRpcPrivate->sendResponseSuccess(*this, result);
}

void JsonRpcMethod::Response::sendError(const Json::Value &error) {
  std::lock_guard<std::mutex> lock(m_private->m_mutex);
  m_private->ensureValid();
  m_private->m_sent = true;
  m_private->m_jsonRpcPrivate->sendResponseError(*this, error);
}

void JsonRpcMethod::Response::invalidate() {
  std::lock_guard<std::mutex> lock(m_private->m_mutex);
  m_private->m_jsonRpcPrivate = nullptr;
}

JsonRpcMethod::~JsonRpcMethod() {
}

JsonRpcNotification::~JsonRpcNotification() {
}

void JsonRpcNotification::invoke(
    const Json::Value &params,
    std::shared_ptr<Response>) {
  invoke(params);
}
