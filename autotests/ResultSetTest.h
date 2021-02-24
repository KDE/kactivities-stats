/*
    SPDX-FileCopyrightText: 2013 Ivan Cukic <ivan.cukic(at)kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef RESULTSETTEST_H
#define RESULTSETTEST_H

#include <common/test.h>

#include <KActivities/Controller>
#include <memory>

#include <QScopedPointer>

class ResultSetTest : public Test
{
    Q_OBJECT
public:
    ResultSetTest(QObject *parent = nullptr);

private Q_SLOTS:
    void initTestCase();

    void testLinkedResources();
    void testUsedResources();

    void cleanupTestCase();
};

#endif /* RESULTSETTEST_H */
