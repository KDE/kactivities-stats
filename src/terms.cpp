/*
 *   Copyright (C) 2015, 2016 Ivan Cukic <ivan.cukic(at)kde.org>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) version 3, or any
 *   later version accepted by the membership of KDE e.V. (or its
 *   successor approved by the membership of KDE e.V.), which shall
 *   act as a proxy defined in Section 6 of version 3 of the license.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library.
 *   If not, see <http://www.gnu.org/licenses/>.
 */

#include "terms.h"
#include "common/specialvalues.h"

#include <QDebug>

namespace KActivities {
namespace Stats {

// Term classes
#define IMPLEMENT_TERM_CONSTRUCTORS(TYPE)                     \
    Terms::TYPE::TYPE(QStringList values)                     \
        : values(values)                                      \
    {}                                                        \
                                                              \
    Terms::TYPE::TYPE(QString value)                          \
        : values(QStringList() << value)                      \
    {}

#define IMPLEMENT_SPECIAL_TERM_VALUE(TYPE, VALUE_NAME, VALUE) \
    Terms::TYPE Terms::TYPE::VALUE_NAME()                     \
    {                                                         \
        return Terms::TYPE(VALUE);                            \
    }

IMPLEMENT_TERM_CONSTRUCTORS(Type)
IMPLEMENT_SPECIAL_TERM_VALUE(Type, any, ANY_TYPE_TAG)

IMPLEMENT_TERM_CONSTRUCTORS(Agent)
IMPLEMENT_SPECIAL_TERM_VALUE(Agent, any,     ANY_AGENT_TAG)
IMPLEMENT_SPECIAL_TERM_VALUE(Agent, global,  GLOBAL_AGENT_TAG)
IMPLEMENT_SPECIAL_TERM_VALUE(Agent, current, CURRENT_AGENT_TAG)

IMPLEMENT_TERM_CONSTRUCTORS(Activity)
IMPLEMENT_SPECIAL_TERM_VALUE(Activity, any,     ANY_ACTIVITY_TAG)
IMPLEMENT_SPECIAL_TERM_VALUE(Activity, global,  GLOBAL_ACTIVITY_TAG)
IMPLEMENT_SPECIAL_TERM_VALUE(Activity, current, CURRENT_ACTIVITY_TAG)

IMPLEMENT_TERM_CONSTRUCTORS(Url)
IMPLEMENT_SPECIAL_TERM_VALUE(Url, localFile, QStringLiteral("/*"))
IMPLEMENT_SPECIAL_TERM_VALUE(Url, file, QStringList() << QStringLiteral("/*") << QStringLiteral("smb:*") << QStringLiteral("fish:*") << QStringLiteral("sftp:*") << QStringLiteral("ftp:*"))

#undef IMPLEMENT_TERM_CONSTRUCTORS
#undef IMPLEMENT_SPECIAL_TERM_VALUE

Terms::Limit::Limit(int value)
    : value(value)
{
}

Terms::Limit Terms::Limit::all()
{
    return Limit(0);
}

Terms::Offset::Offset(int value)
    : value(value)
{
}

Terms::Date::Date(QDate value)
    : start(value)
{
}

Terms::Date::Date(QDate start, QDate end)
    : start(start), end(end)
{
}

Terms::Date Terms::Date::today()
{
    return Date(QDate::currentDate());
}

Terms::Date Terms::Date::yesterday()
{
    auto date = QDate::currentDate();
    return Date(date.addDays(-1));
}

Terms::Date Terms::Date::currentWeek()
{
    auto start = QDate::currentDate();
    auto end = start.addDays(-7);
    return Date(start, end);
}

Terms::Date Terms::Date::previousWeek()
{
    auto start = QDate::currentDate().addDays(-7);
    auto end = start.addDays(-7);
    return Date(start, end);
}

Terms::Date Terms::Date::fromString(QString string)
{
    auto splitted = string.split(QStringLiteral(","));
    if (splitted.count() == 2) {
        // date range case
        auto start = QDate::fromString(splitted[0], Qt::ISODate);
        auto end = QDate::fromString(splitted[1], Qt::ISODate);
        return Date(start, end);
    } else {
        auto date = QDate::fromString(string, Qt::ISODate);
        return Date(date);
    }
}

Terms::Url Terms::Url::startsWith(const QString &prefix)
{
    return Url(prefix + QStringLiteral("*"));
}

Terms::Url Terms::Url::contains(const QString &infix)
{
    return Url(QStringLiteral("*") + infix + QStringLiteral("*"));
}

} // namespace Stats
} // namespace KActivities

namespace KAStats = KActivities::Stats;

#define QDEBUG_TERM_OUT(TYPE, OUT)                               \
    QDebug operator<<(QDebug dbg, const KAStats::Terms::TYPE &_) \
    {                                                            \
        using namespace KAStats::Terms;                          \
        dbg.nospace() << #TYPE << ": " << (OUT);                 \
        return dbg;                                              \
    }

QDEBUG_TERM_OUT(Order,    _ == HighScoredFirst      ? "HighScore"       :
                          _ == RecentlyUsedFirst    ? "RecentlyUsed"    :
                          _ == RecentlyCreatedFirst ? "RecentlyCreated" :
                                                      "Alphabetical"    )

QDEBUG_TERM_OUT(Select,   _ == LinkedResources ? "LinkedResources" :
                          _ == UsedResources   ? "UsedResources"   :
                                                 "AllResources"    )

QDEBUG_TERM_OUT(Type,     _.values)
QDEBUG_TERM_OUT(Agent,    _.values)
QDEBUG_TERM_OUT(Activity, _.values)
QDEBUG_TERM_OUT(Url,      _.values)

QDEBUG_TERM_OUT(Limit,    _.value)
QDEBUG_TERM_OUT(Offset,   _.value)
QDEBUG_TERM_OUT(Date,     _.end.isNull() ? _.start.toString(Qt::ISODate) :
                          _.start.toString(Qt::ISODate) + QStringLiteral(",") + _.end.toString(Qt::ISODate))

#undef QDEBUG_TERM_OUT

