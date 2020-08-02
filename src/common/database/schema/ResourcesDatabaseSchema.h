/*
    SPDX-FileCopyrightText: 2011, 2012, 2013, 2014, 2015 Ivan Cukic <ivan.cukic(at)kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef RESOURCESDATABASESCHEMA_H
#define RESOURCESDATABASESCHEMA_H

#include <QStringList>
#include "../Database.h"

namespace Common {
namespace ResourcesDatabaseSchema {

    QString version();

    QStringList schema();

    QString path();
    void overridePath(const QString &path);

    void initSchema(Database &database);

} // namespace ResourcesDatabase
} // namespace Common

#endif // RESOURCESDATABASESCHEMA_H

