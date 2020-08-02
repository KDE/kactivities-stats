/*
    SPDX-FileCopyrightText: 2015, 2016 Ivan Cukic <ivan.cukic(at)kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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

/**
 * @namespace KActivities::Stats::Terms
 * Provides enums and strucss to use.for building queries with @c Query.
 */
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
     * @struct KActivities::Stats::Terms::Limit terms.h <KActivities/Stats/Terms>
     *
     * How many items do you need?
     */
    struct KACTIVITIESSTATS_EXPORT Limit {
        Limit(int value);
        static Limit all();
        int value;
    };

    /**
     * @struct KActivities::Stats::Terms::Offset terms.h <KActivities/Stats/Terms>
     *
     * How many items to skip?
     * This can be specified only if limit is also set to a finite value.
     */
    struct KACTIVITIESSTATS_EXPORT Offset {
        Offset(int value);
        int value;
    };

    /**
     * @struct KActivities::Stats::Terms::Type terms.h <KActivities/Stats/Terms>
     *
     * Term to filter the resources according to their types
     */
    struct KACTIVITIESSTATS_EXPORT Type {
        /**
         * Show resources of any type
         */
        static Type any();
        /**
         * Show non-directory resources
         */
        static Type files();
        /**
         * Show directory resources aka folders
         */
        static Type directories();

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
     * @struct KActivities::Stats::Terms::Agent terms.h <KActivities/Stats/Terms>
     *
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
     * @struct KActivities::Stats::Terms::Activity terms.h <KActivities/Stats/Terms>
     *
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
     * @struct KActivities::Stats::Terms::Url terms.h <KActivities/Stats/Terms>
     *
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
     * @struct KActivities::Stats::Terms::Date terms.h <KActivities/Stats/Terms>
     *
     * On which start access date do you want to filter ?
     */
    struct KACTIVITIESSTATS_EXPORT Date {
        Date(QDate value);
        Date(QDate start, QDate end);

        static Date today();
        static Date yesterday();
        static Date currentWeek();
        static Date previousWeek();
        static Date fromString(QString);

        QDate start, end;
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

