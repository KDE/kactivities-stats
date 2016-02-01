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

#ifndef CLEANING_H
#define CLEANING_H

#include <QString>
#include "terms.h"
#include "query.h"

namespace KActivities {
namespace Stats {

/**
 * Forget the resource(s) for the specified activity and agent
 */
void KACTIVITIESSTATS_EXPORT forgetResource(Terms::Activity activity,
                                            Terms::Agent agent,
                                            const QString &resource);

enum KACTIVITIESSTATS_EXPORT TimeUnit {
    Hours,
    Days,
    Months
};

/**
 * Forget recent stats for the specified activity and time
 */
void KACTIVITIESSTATS_EXPORT forgetRecentStats(Terms::Activity activity,
                                               int count,
                                               TimeUnit what);

/**
 * Forget events that are older than the specified number of months
 */
void KACTIVITIESSTATS_EXPORT forgetEarlierStats(Terms::Activity activity,
                                                int months);

/**
 * Forget resources that match the specified query
 */
void KACTIVITIESSTATS_EXPORT forgetResources(const Query &query);

} // namespace Stats
} // namespace KActivities

#endif // CLEANING_H

