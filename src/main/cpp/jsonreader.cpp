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

  while (!m_stack.empty()) {
    m_stack.pop();
  }
}

void JsonReader::send() {
  std::string const jsonText(m_buffer.str());
  reset();

  if (jsonText.empty())
    return;

  if(jsonText.size() == 1){
    if( jsonText.find(" ") == 0
       || jsonText.find("\t") == 0
       || jsonText.find("\n") == 0
       || jsonText.find("\r") == 0
    )
      return;
  }
  if(jsonText.size() == 2 && jsonText.find("\r\n") == 0 )
    return;

  m_jsonRpcPrivate.jsonReaderCallback(jsonText);
}

bool JsonReader::transition(char const ch) {
  switch (m_state) {
    case S0:
      if ('{' == ch || '[' == ch) {
        m_state = S1;
        m_stack.push(ch);
      } else if (' ' == ch || '\t' == ch || '\n' == ch || '\r' == ch) {
        return true;
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

        return send;
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
  }

  return false;
}

void JsonReader::feed(char ch) {
  m_buffer << ch;
  if(transition(ch))
	  send();
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
