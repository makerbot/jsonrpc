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

#include "jsonrpc/jsonreader.h"

JsonReader::JsonReader (void)
    : m_state (S0)
{
}

void
JsonReader::feed (QString const data)
{
    for (QString::const_iterator i (data.begin ()); i != data.end (); ++i)
    {
        QChar const ch (* i);
        this->m_buffer += ch;
        this->transition (ch);
    }
}

void
JsonReader::feedeof (void)
{
    this->send ();
}

void
JsonReader::reset (void)
{
    this->m_state = S0;
    this->m_stack.clear ();
    this->m_buffer.clear ();
}

void
JsonReader::transition (QChar const ch)
{
    switch (this->m_state)
    {
    case S0:
        if ('{' == ch or '[' == ch)
        {
            this->m_state = S1;
            this->m_stack.push(ch);
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
            this->m_stack.push(ch);
        }
        else
        if ('}' == ch or ']' == ch)
        {
            bool send (false);
            if (this->m_stack.isEmpty ())
            {
                send = true;
            }
            else
            {
                QChar const firstch (this->m_stack.pop ());
                if (('{' == firstch and '}' != ch) or ('[' == firstch and ']' != ch))
                {
                    send = true;
                }
                else
                {
                    send = this->m_stack.isEmpty ();
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
JsonReader::send (void)
{
    QString const data (this->m_buffer);
    this->reset ();
    if (not data.isEmpty ())
    {
        emit readJson (data);
    }
}
