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

#include <QString>
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
        scoring.call(QStringLiteral("DeleteRecentStats"), activity, count,
                what == Hours  ? QStringLiteral("h") :
                what == Days   ? QStringLiteral("d") :
                                 QStringLiteral("m")
            );
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

