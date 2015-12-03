// Copyright 2013 MakerBot Industries

#include <string>

#include "cpp/jsonrpcprivate.h"
#include "cpp/jsonreader.h"
#include "jsonrpc/jsonrpc.h"

JsonReader::JsonReader(JsonRpcPrivate & jsonRpcPrivate)
    : m_jsonRpcPrivate(jsonRpcPrivate)
    , m_state(S0) {
}

void JsonReader::reset() {
  m_state = S0;
  m_buffer.str(std::string());

  m_rawHandler.reset();
  m_rawBuffer.reset();
  m_rawBytesReceived = 0;
  m_expectedLength = 0;

  while (!m_stack.empty()) {
    m_stack.pop();
  }
}

void JsonReader::send() {
  std::string const jsonText(m_buffer.str());
  reset();
  if (!jsonText.empty()) {
    m_jsonRpcPrivate.jsonReaderCallback(jsonText);
  }
  if (m_rawHandler)
    m_state = S4;
}

void JsonReader::transition(char const ch) {
  switch (m_state) {
    case S0:
      if ('{' == ch || '[' == ch) {
        m_state = S1;
        m_stack.push(ch);
      } else if (' ' != ch && '\t' != ch && '\n' != ch && '\r' != ch) {
        send();
      }
      break;

    case S1:
      if ('\"' == ch) {
        m_state = S2;
      } else if ('{' == ch || '[' == ch) {
        m_stack.push(ch);
      } else if ('}' == ch || ']' == ch) {
        bool send(false);
        if (m_stack.empty()) {
          send = true;
        } else {
          char const firstch(m_stack.top());
          m_stack.pop();
          if (('{' == firstch && '}' != ch)
              ||('[' == firstch && ']' != ch)) {
            send = true;
          } else {
            send = m_stack.empty();
          }
        }

        if (send) {
          this->send();
        }
      }
      break;

    case S2:
      if ('\"' == ch) {
        m_state = S1;
      } else if ('\\' == ch) {
        m_state = S3;
      }
      break;

    case S3:
      m_state = S2;
      break;

    case S4:
      m_rawBuffer[m_rawBytesReceived++] = ch;
      if (m_rawHandler && (m_rawBytesReceived == m_expectedLength)) {
        (*m_rawHandler)(m_rawBuffer.release(), m_expectedLength);
        if (m_rawBytesReceived == m_expectedLength) {
            reset();
        }
      }
      break;
  }
}

void JsonReader::feed(char ch) {
  m_buffer << ch;
  transition(ch);
}

void JsonReader::feed(char const * const buffer, std::size_t const length) {
  for (std::size_t i(static_cast <std::size_t>(0u)); i < length; ++i) {
    char const ch(buffer[i]);
    feed(ch);
  }
}

void JsonReader::feed(std::string const & str) {
  for (std::string::const_iterator i(str.begin()); i != str.end(); ++i) {
    feed(* i);
  }
}

void JsonReader::feedeof() {
  send();
}

void JsonReader::setRawHandler(
    std::shared_ptr<std::function<void(char*, const size_t)> > rawHandler,
    const size_t length) {
  m_rawHandler = rawHandler;
  m_expectedLength = length;
  m_rawBuffer.reset(new char[length]);
  m_rawBytesReceived = 0;
}
