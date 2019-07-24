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


#ifndef KACTIVITIES_STATS_TERMS_H
#define KACTIVITIES_STATS_TERMS_H

#include <qcompilerdetection.h>

#ifdef Q_COMPILER_INITIALIZER_LISTS
#include <initializer_list>
#endif

#include <QString>
#include <QStringList>
#include <QDate>

#include "kactivitiesstats_export.h"

namespace KActivities {
namespace Stats {

namespace Terms {
    /**
     * Enumerator specifying the ordering in which the
     * results of the query should be listed
     */
    enum KACTIVITIESSTATS_EXPORT Order {
        HighScoredFirst,      ///< Resources with the highest scores first
        RecentlyUsedFirst,    ///< Recently used resources first
        RecentlyCreatedFirst, ///< Recently created resources first
        OrderByUrl,           ///< Order by uri, alphabetically
        OrderByTitle          ///< Order by uri, alphabetically
    };

    /**
     * Which resources should be returned
     */
    enum KACTIVITIESSTATS_EXPORT Select {
        LinkedResources, ///< Resources linked to an activity, or globally
        UsedResources,   ///< Resources that have been accessed
        AllResources     ///< Combined set of accessed and linked resources
    };

    /**
     * How many items do you need?
     */
    struct KACTIVITIESSTATS_EXPORT Limit {
        Limit(int value);
        static Limit all();
        int value;
    };

    /**
     * How many items to skip?
     * This can be specified only if limit is also set to a finite value.
     */
    struct KACTIVITIESSTATS_EXPORT Offset {
        Offset(int value);
        int value;
    };

    /**
     * Term to filter the resources according to their types
     */
    struct KACTIVITIESSTATS_EXPORT Type {
        /**
         * Show resources of any type
         */
        static Type any();

        #ifdef Q_COMPILER_INITIALIZER_LISTS
        inline Type(std::initializer_list<QString> types)
            : values(types)
        {
        }
        #endif

        Type(QStringList types);
        Type(QString type);

        const QStringList values;
    };

    /**
     * Term to filter the resources according the agent (application) which
     * accessed it
     */
    struct KACTIVITIESSTATS_EXPORT Agent {
        /**
         * Show resources accessed/linked by any application
         */
        static Agent any();

        /**
         * Show resources not tied to a specific agent
         */
        static Agent global();

        /**
         * Show resources accessed/linked by the current application
         */
        static Agent current();

        #ifdef Q_COMPILER_INITIALIZER_LISTS
        inline Agent(std::initializer_list<QString> agents)
            : values(agents)
        {
        }
        #endif

        Agent(QStringList agents);
        Agent(QString agent);

        const QStringList values;
    };

    /**
     * Term to filter the resources according the activity in which they
     * were accessed
     */
    struct KACTIVITIESSTATS_EXPORT Activity {
        /**
         * Show resources accessed in / linked to any activity
         */
        static Activity any();

        /**
         * Show resources linked to all activities
         */
        static Activity global();

        /**
         * Show resources linked to all activities
         */
        static Activity current();

        #ifdef Q_COMPILER_INITIALIZER_LISTS
        inline Activity(std::initializer_list<QString> activities)
            : values(activities)
        {
        }
        #endif

        Activity(QStringList activities);
        Activity(QString activity);

        const QStringList values;
    };

    /**
     * Url filtering.
     */
    struct KACTIVITIESSTATS_EXPORT Url {
        /**
         * Show only resources that start with the specified prefix
         */
        static Url startsWith(const QString &prefix);

        /**
         * Show resources that contain the specified infix
         */
        static Url contains(const QString &infix);

        /**
         * Show local files
         */
        static Url localFile();

        /**
         * Show local files, smb, fish, ftp and stfp
         */
        static Url file();

        #ifdef Q_COMPILER_INITIALIZER_LISTS
        inline Url(std::initializer_list<QString> urlPatterns)
            : values(urlPatterns)
        {
        }
        #endif

        Url(QStringList urlPatterns);
        Url(QString urlPattern);

        const QStringList values;

    };

    /**
     * On which start access date do you want to filter ?
     */
    struct KACTIVITIESSTATS_EXPORT Date {
        Date(QDate value);
        static Date today();
        static Date yesterday();
        static Date fromString(QString);
        QDate value;
    };

} // namespace Terms

} // namespace Stats
} // namespace KActivities

KACTIVITIESSTATS_EXPORT
QDebug operator<<(QDebug dbg, const KActivities::Stats::Terms::Order &order);

KACTIVITIESSTATS_EXPORT
QDebug operator<<(QDebug dbg, const KActivities::Stats::Terms::Select &select);

KACTIVITIESSTATS_EXPORT
QDebug operator<<(QDebug dbg, const KActivities::Stats::Terms::Type &type);

KACTIVITIESSTATS_EXPORT
QDebug operator<<(QDebug dbg, const KActivities::Stats::Terms::Agent &agent);

KACTIVITIESSTATS_EXPORT
QDebug operator<<(QDebug dbg, const KActivities::Stats::Terms::Activity &activity);

KACTIVITIESSTATS_EXPORT
QDebug operator<<(QDebug dbg, const KActivities::Stats::Terms::Url &url);

KACTIVITIESSTATS_EXPORT
QDebug operator<<(QDebug dbg, const KActivities::Stats::Terms::Limit &limit);

KACTIVITIESSTATS_EXPORT
QDebug operator<<(QDebug dbg, const KActivities::Stats::Terms::Offset &offset);

KACTIVITIESSTATS_EXPORT
QDebug operator<<(QDebug dbg, const KActivities::Stats::Terms::Date &date);

#endif // KACTIVITIES_STATS_TERMS_H

