/*
 *   Copyright (C) 2015 Ivan Cukic <ivan.cukic(at)kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "QueryTest.h"

#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QString>
#include <QDebug>
#include <QTest>

#include <query.h>
#include <common/test.h>

namespace KAStats = KActivities::Stats;
using namespace KAStats;
using namespace KAStats::Terms;

QueryTest::QueryTest(QObject *parent)
    : Test(parent)
{
}

void QueryTest::testDefaults()
{
    TEST_CHUNK(QStringLiteral("Testing the term defaults"));

    Query query;

    QCOMPARE(query.selection(),  AllResources);
    QCOMPARE(query.types(),      {QStringLiteral(":any")});
    QCOMPARE(query.agents(),     {QStringLiteral(":current")});
    QCOMPARE(query.activities(), {QStringLiteral(":current")});
    QCOMPARE(query.ordering(),   HighScoredFirst);
}

void QueryTest::testDebuggingOutput()
{
    TEST_CHUNK(QStringLiteral("Debugging output for a query"));

    Query query;

    // Testing whether qDebug can be called (compilation check)
    qDebug() << "Writing out a query:" << query;
}

void QueryTest::testDerivationFromDefault()
{
    TEST_CHUNK(QStringLiteral("Testing query derivation from default"))

    Query queryDefault;
    auto  queryDerived = queryDefault | LinkedResources;

    // queryDefault should not have been modified
    QCOMPARE(queryDefault.selection(),  AllResources);
    QCOMPARE(queryDerived.selection(),  LinkedResources);

    // Changing queryDerived back to AllResources, should be == to queryDefault
    queryDerived.setSelection(AllResources);
    QCOMPARE(queryDefault, queryDerived);
}

void QueryTest::testDerivationFromCustom()
{
    TEST_CHUNK(QStringLiteral("Testing query derivation from custom"))

    Query queryCustom;
    auto  queryDerived = queryCustom | LinkedResources;

    // q1 should not have been modified
    QCOMPARE(queryCustom.selection(),  AllResources);
    QCOMPARE(queryDerived.selection(),  LinkedResources);

    // Changing queryDerived back to AllResources, should be == to queryDefault
    queryDerived.setSelection(AllResources);
    QCOMPARE(queryCustom, queryDerived);
}

void QueryTest::testNormalSyntaxAgentManipulation()
{
    TEST_CHUNK(QStringLiteral("Testing normal syntax manipulation: Agents"))

    Query query;
    query.addAgents(QStringList() << QStringLiteral("gvim") << QStringLiteral("kate"));

    QCOMPARE(query.agents(), QStringList() << QStringLiteral("gvim") << QStringLiteral("kate"));

    query.addAgents(QStringList() << QStringLiteral("kwrite"));

    QCOMPARE(query.agents(), QStringList() << QStringLiteral("gvim") << QStringLiteral("kate") << QStringLiteral("kwrite"));

    query.clearAgents();

    QCOMPARE(query.agents(), QStringList() << QStringLiteral(":current"));
}

void QueryTest::testNormalSyntaxTypeManipulation()
{
    TEST_CHUNK(QStringLiteral("Testing normal syntax manipulation: Types"))

    Query query;
    query.addTypes(QStringList() << QStringLiteral("text/html") << QStringLiteral("text/plain"));

    QCOMPARE(query.types(), QStringList() << QStringLiteral("text/html") << QStringLiteral("text/plain"));

    query.addTypes(QStringList() << QStringLiteral("text/xml"));

    QCOMPARE(query.types(), QStringList() << QStringLiteral("text/html") << QStringLiteral("text/plain") << QStringLiteral("text/xml"));

    query.clearTypes();

    QCOMPARE(query.types(), QStringList() << QStringLiteral(":any"));
}

void QueryTest::testNormalSyntaxActivityManipulation()
{
    TEST_CHUNK(QStringLiteral("Testing normal syntax manipulation: Activities"))

    Query query;
    query.addActivities(QStringList() << QStringLiteral("a1") << QStringLiteral("a2"));

    QCOMPARE(query.activities(), QStringList() << QStringLiteral("a1") << QStringLiteral("a2"));

    query.addActivities(QStringList() << QStringLiteral("a3"));

    QCOMPARE(query.activities(), QStringList() << QStringLiteral("a1") << QStringLiteral("a2") << QStringLiteral("a3"));

    query.clearActivities();

    QCOMPARE(query.activities(), QStringList() << QStringLiteral(":current"));
}

void QueryTest::testNormalSyntaxOrderingManipulation()
{
    TEST_CHUNK(QStringLiteral("Testing normal syntax manipulation: Activities"))

    Query query;

    QCOMPARE(query.ordering(), HighScoredFirst);

    query.setOrdering(RecentlyCreatedFirst);

    QCOMPARE(query.ordering(), RecentlyCreatedFirst);

    query.setOrdering(OrderByUrl);

    QCOMPARE(query.ordering(), OrderByUrl);
}

void QueryTest::testFancySyntaxBasic()
{
    TEST_CHUNK(QStringLiteral("Testing the fancy syntax, non c++11"))

    auto query = LinkedResources
                    | Type(QStringLiteral("text"))
                    | Type(QStringLiteral("image"))
                    | Agent(QStringLiteral("test"))
                    | RecentlyCreatedFirst;

    QCOMPARE(query.selection(),  LinkedResources);
    QCOMPARE(query.types(),      QStringList() << QStringLiteral("text") << QStringLiteral("image"));
    QCOMPARE(query.agents(),     QStringList() << QStringLiteral("test"));
    QCOMPARE(query.activities(), QStringList() << QStringLiteral(":current"));
    QCOMPARE(query.ordering(),   RecentlyCreatedFirst);

    #ifdef Q_COMPILER_INITIALIZER_LISTS
    TEST_CHUNK(QStringLiteral("Testing the fancy syntax, c++11"))

    // Testing the fancy c++11 syntax
    auto queryCXX11 = LinkedResources
                | Type{QStringLiteral("text"), QStringLiteral("image")}
                | Agent{QStringLiteral("test")}
                | RecentlyCreatedFirst;

    QCOMPARE(query, queryCXX11);
    #endif
}

void QueryTest::testFancySyntaxAgentDefinition()
{
    TEST_CHUNK(QStringLiteral("Testing the fancy syntax, agent definition"))

    {
        auto query = LinkedResources | OrderByUrl;
        QCOMPARE(query.agents(), QStringList() << QStringLiteral(":current"));
    }

    {
        auto query = LinkedResources | Agent(QStringLiteral("gvim"));
        QCOMPARE(query.agents(), QStringList() << QStringLiteral("gvim"));
    }

    {
        auto query = LinkedResources | Agent(QStringLiteral("gvim")) | Agent(QStringLiteral("kate"));
        QCOMPARE(query.agents(), QStringList() << QStringLiteral("gvim") << QStringLiteral("kate"));
    }

    {
        auto query = LinkedResources | Agent(QStringList() << QStringLiteral("gvim") << QStringLiteral("kate"));
        QCOMPARE(query.agents(), QStringList() << QStringLiteral("gvim") << QStringLiteral("kate"));
    }
}

void QueryTest::testFancySyntaxTypeDefinition()
{
    TEST_CHUNK(QStringLiteral("Testing the fancy syntax, type definition"))

    {
        auto query = LinkedResources | OrderByUrl;
        QCOMPARE(query.types(), QStringList() << QStringLiteral(":any"));
    }

    {
        auto query = LinkedResources | Type(QStringLiteral("text/plain"));
        QCOMPARE(query.types(), QStringList() << QStringLiteral("text/plain"));
    }

    {
        auto query = LinkedResources | Type(QStringLiteral("text/plain")) | Type(QStringLiteral("text/html"));
        QCOMPARE(query.types(), QStringList() << QStringLiteral("text/plain") << QStringLiteral("text/html"));
    }

    {
        auto query = LinkedResources | Type(QStringList() << QStringLiteral("text/plain") << QStringLiteral("text/html"));
        QCOMPARE(query.types(), QStringList() << QStringLiteral("text/plain") << QStringLiteral("text/html"));
    }
}

void QueryTest::testFancySyntaxActivityDefinition()
{
    TEST_CHUNK(QStringLiteral("Testing the fancy syntax, activity definition"))

    {
        auto query = LinkedResources | OrderByUrl;
        QCOMPARE(query.activities(), QStringList() << QStringLiteral(":current"));
    }

    {
        auto query = LinkedResources | Activity(QStringLiteral("gvim"));
        QCOMPARE(query.activities(), QStringList() << QStringLiteral("gvim"));
    }

    {
        auto query = LinkedResources | Activity(QStringLiteral("gvim")) | Activity(QStringLiteral("kate"));
        QCOMPARE(query.activities(), QStringList() << QStringLiteral("gvim") << QStringLiteral("kate"));
    }

    {
        auto query = LinkedResources | Activity(QStringList() << QStringLiteral("gvim") << QStringLiteral("kate"));
        QCOMPARE(query.activities(), QStringList() << QStringLiteral("gvim") << QStringLiteral("kate"));
    }
}

void QueryTest::testFancySyntaxOrderingDefinition()
{
    TEST_CHUNK(QStringLiteral("Testing the fancy syntax, activity definition"))

    {
        auto query = LinkedResources | OrderByUrl;
        QCOMPARE(query.ordering(), OrderByUrl);
    }

    {
        auto query = LinkedResources | HighScoredFirst;
        QCOMPARE(query.ordering(), HighScoredFirst);
    }

    {
        auto query = LinkedResources | RecentlyCreatedFirst;
        QCOMPARE(query.ordering(), RecentlyCreatedFirst);
    }

    {
        auto query = LinkedResources | RecentlyCreatedFirst | OrderByUrl;
        QCOMPARE(query.ordering(), OrderByUrl);
    }

    {
        auto query = LinkedResources | RecentlyCreatedFirst | HighScoredFirst;
        QCOMPARE(query.ordering(), HighScoredFirst);
    }
}

void QueryTest::initTestCase()
{
    // CHECK_CONDITION(isActivityManagerRunning, FailIfTrue);
}

void QueryTest::cleanupTestCase()
{
    emit testFinished();
}

