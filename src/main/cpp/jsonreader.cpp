// vim:cindent:cino=\:0:et:ff=unix:fileencoding=utf-8:sw=4:ts=4:
// jsonrpc/src/main/cpp/jsonreader.h
//
// jsonrpc - JSON-RPC 2.0 for QT
// Copyright © 2012 Matthew W. Samsonoff <matthew.samsonoff@makerbot.com>
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Affero General Public License as published by the
// Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License
// for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <string>

#include <jsonrpc/jsonrpc.h>

#include "jsonrpcprivate.h"
#include "jsonreader.h"

JsonReader::JsonReader (JsonRpcPrivate & jsonRpcPrivate)
    : m_jsonRpcPrivate (jsonRpcPrivate)
    , m_state (S0)
{
}

void
JsonReader::reset (void)
{
    this->m_state = S0;
    this->m_buffer.str (std::string ());

    while (not this->m_stack.empty ())
    {
        this->m_stack.pop ();
    }
}

void
JsonReader::send (void)
{
    std::string const jsonText (this->m_buffer.str ());
    this->reset ();
    if (not jsonText.empty ())
    {
        this->m_jsonRpcPrivate.jsonReaderCallback (jsonText);
    }
}

void
JsonReader::transition (char const ch)
{
    switch (this->m_state)
    {
    case S0:
        if ('{' == ch or '[' == ch)
        {
            this->m_state = S1;
            this->m_stack.push (ch);
        }
        else
        if (' ' != ch and '\t' != ch and '\n' != ch and '\r' != ch)
        {
            this->send ();
        }
        break;

    case S1:
        if ('\"' == ch)
        {
            this->m_state = S2;
        }
        else
        if ('{' == ch or '[' == ch)
        {
            this->m_stack.push (ch);
        }
        else
        if ('}' == ch or ']' == ch)
        {
            bool send (false);
            if (this->m_stack.empty ())
            {
                send = true;
            }
            else
            {
                char const firstch (this->m_stack.top ());
                this->m_stack.pop ();
                if (('{' == firstch and '}' != ch)
                    or ('[' == firstch and ']' != ch))
                {
                    send = true;
                }
                else
                {
                    send = this->m_stack.empty ();
                }
            }
            if (send)
            {
                this->send ();
            }
        }
        break;

    case S2:
        if ('\"' == ch)
        {
            this->m_state = S1;
        }
        else
        if ('\\' == ch)
        {
            this->m_state = S3;
        }
        break;

    case S3:
        this->m_state = S2;
        break;
    }
}

void
JsonReader::feed (char ch)
{
    this->m_buffer << ch;
    this->transition (ch);
}

void
JsonReader::feed (char const * const buffer, std::size_t const length)
{
    for (std::size_t i (static_cast <std::size_t> (0u)); i < length; ++i)
    {
        char const ch (buffer[i]);
        this->feed (ch);
    }
}

void
JsonReader::feed (std::string const & str)
{
    for (std::string::const_iterator i (str.begin ()); i != str.end (); ++i)
    {
        this->feed (* i);
    }
}

void
JsonReader::feedeof (void)
{
    this->send ();
}
