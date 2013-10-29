// Copyright 2013 MakerBot Industries

#ifndef SRC_MAIN_CPP_JSONREADER_H_
#define SRC_MAIN_CPP_JSONREADER_H_

#include <cstddef>
#include <sstream>
#include <stack>
#include <string>

#include "jsonrpc/jsonrpc.h"
#include "jsonrpc/jsonrpcstream.h"

class JsonReader : public JsonRpcStream {
 public:
  JSONRPC_API JsonReader(JsonRpcPrivate &);

  JSONRPC_API void feed(char ch);
  JSONRPC_API void feed(char const *, std::size_t);
  JSONRPC_API void feed(std::string const &);
  JSONRPC_API void feedeof(void);

 private:
  enum State { S0, S1, S2, S3 };

  void reset(void);
  void transition(char ch);
  void send(void);

  JsonRpcPrivate & m_jsonRpcPrivate;
  State m_state;
  std::stack <char> m_stack;
  std::ostringstream m_buffer;

  // Disable copy constructor and assignment.

  JsonReader(const JsonReader &other);
  JsonReader & operator=(JsonReader const &);
};

#endif  // SRC_MAIN_CPP_JSONREADER_H_
