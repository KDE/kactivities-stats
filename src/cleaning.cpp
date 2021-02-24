/*
    SPDX-FileCopyrightText: 2015, 2016 Ivan Cukic <ivan.cukic(at)kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include <QDBusReply>
#include <QDebug>

#include "cleaning.h"
#include "common/dbus/common.h"

namespace KActivities {
namespace Stats {


void forgetResource(Terms::Activity activities, Terms::Agent agents,
                    const QString &resource)
{
    KAMD_DBUS_DECL_INTERFACE(scoring, Resources/Scoring, ResourcesScoring);
    for (const auto& activity: activities.values) {
        for (const auto& agent: agents.values) {
            scoring.call(QStringLiteral("DeleteStatsForResource"), activity, agent, resource);
        }
    }
}

void forgetResources(const Query &query)
{
    KAMD_DBUS_DECL_INTERFACE(scoring, Resources/Scoring, ResourcesScoring);
    for (const auto& activity: query.activities()) {
        for (const auto& agent: query.agents()) {
            for (const auto& urlFilter: query.urlFilters()) {
                scoring.call(QStringLiteral("DeleteStatsForResource"), activity, agent, urlFilter);
            }
        }
    }
}

void forgetRecentStats(Terms::Activity activities, int count, TimeUnit what)
{
    KAMD_DBUS_DECL_INTERFACE(scoring, Resources/Scoring, ResourcesScoring);
    for (const auto& activity: activities.values) {
        /* clang-format off */
        scoring.call(QStringLiteral("DeleteRecentStats"), activity, count,
                what == Hours  ? QStringLiteral("h") :
                what == Days   ? QStringLiteral("d") :
                                 QStringLiteral("m")
            );
        /* clang-format on */
    }
}

void forgetEarlierStats(Terms::Activity activities, int months)
{
    KAMD_DBUS_DECL_INTERFACE(scoring, Resources/Scoring, ResourcesScoring);
    for (const auto& activity: activities.values) {
        scoring.call(QStringLiteral("DeleteEarlierStats"), activity, months);
    }
}

} // namespace Stats
} // namespace KActivities

