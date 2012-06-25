// vim:cindent:cino=\:0:et:ff=unix:fileencoding=utf-8:sw=4:ts=4:
// jsonrpc/src/main/cpp/jsonrpc.h
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

#ifndef JSONRPC_JSONREADER_H
#define JSONRPC_JSONREADER_H (1)

#include <QObject>
#include <QStack>

class JsonReader : public QObject
{
    Q_OBJECT

public:
    JsonReader (void);

    void feed (QString data);
    void feedeof (void);

signals:
    void readJson (QString data);

private:
    enum State { S0, S1, S2, S3 };

    void reset (void);
    void transition (QChar ch);
    void send (void);

    State m_state;
    QStack<QChar> m_stack;
    QString m_buffer;
};

#endif
