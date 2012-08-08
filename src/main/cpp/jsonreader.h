// vim:cindent:cino=\:0:et:ff=unix:fileencoding=utf-8:sw=4:ts=4:

#ifndef JSONREADER_H
#define JSONREADER_H (1)

#include <cstddef>
#include <sstream>
#include <stack>

#include <jsonrpc/jsonrpc.h>
#include <jsonrpc/jsonrpcstream.h>

class JsonReader : public JsonRpcStream
{
public:
    JsonReader (JsonRpcPrivate &);

    void feed (char ch);
    void feed (char const *, std::size_t);
    void feed (std::string const &);
    void feedeof (void);

private:
    enum State { S0, S1, S2, S3 };

    void reset (void);
    void transition (char ch);
    void send (void);

    JsonRpcPrivate & m_jsonRpcPrivate;
    State m_state;
    std::stack <char> m_stack;
    std::ostringstream m_buffer;
};

#endif
