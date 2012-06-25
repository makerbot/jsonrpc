// vim:cindent:cino=\:0:et:ff=unix:fileencoding=utf-8:sw=4:ts=4:
// jsonrpc/src/test/cpp/test-jsonreader.cpp
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

#include <QSignalSpy>
#include <QString>
#include <QtTest/QtTest>

#include "jsonrpc/jsonreader.h"
#include "test-jsonreader.h"

void
TestJsonReader::testObject (void)
{
    JsonReader jsonReader;
    QSignalSpy signalSpy (& jsonReader, SIGNAL (readJson (QString)));

    jsonReader.feed("{\"key\":\"value\"");
    QCOMPARE (signalSpy.count (), 0);

    jsonReader.feed("}");
    QCOMPARE (signalSpy.count (), 1);
    QCOMPARE
        ( signalSpy.takeFirst().at(0).toString()
        , QString ("{\"key\":\"value\"}")
        );
}

void
TestJsonReader::testNestedObject (void)
{
    JsonReader jsonReader;
    QSignalSpy signalSpy (& jsonReader, SIGNAL (readJson (QString)));

    jsonReader.feed("{\"key0\":{\"key1\":\"value\"");
    QCOMPARE (signalSpy.count (), 0);

    jsonReader.feed("}");
    QCOMPARE (signalSpy.count (), 0);

    jsonReader.feed("}");
    QCOMPARE (signalSpy.count (), 1);
    QCOMPARE
        ( signalSpy.takeFirst().at(0).toString()
        , QString ("{\"key0\":{\"key1\":\"value\"}}")
        );
}

void
TestJsonReader::testEscape (void)
{
    JsonReader jsonReader;
    QSignalSpy signalSpy (& jsonReader, SIGNAL (readJson (QString)));

    jsonReader.feed("{\"key\":\"value\\\"");
    QCOMPARE (signalSpy.count (), 0);

    jsonReader.feed("\"");
    QCOMPARE (signalSpy.count (), 0);

    jsonReader.feed("}");
    QCOMPARE (signalSpy.count (), 1);
    QCOMPARE
        ( signalSpy.takeFirst().at(0).toString()
        , QString ("{\"key\":\"value\\\"\"}")
        );
}

void
TestJsonReader::testInvalid (void)
{
    JsonReader jsonReader;
    QSignalSpy signalSpy (& jsonReader, SIGNAL (readJson (QString)));

    jsonReader.feed("]");
    QCOMPARE (signalSpy.count (), 1);
    QCOMPARE
        ( signalSpy.takeFirst().at(0).toString()
        , QString ("]")
        );
}

QTEST_MAIN(TestJsonReader)
