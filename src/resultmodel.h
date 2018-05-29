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

#ifndef KACTIVITIES_STATS_RESULTMODEL_H
#define KACTIVITIES_STATS_RESULTMODEL_H

// Qt
#include <QObject>
#include <QAbstractListModel>

// Local
#include "query.h"

class QModelIndex;
class QDBusPendingCallWatcher;

class KConfigGroup;

namespace KActivities {
namespace Stats {

class ResultModelPrivate;

/**
 * Provides a model which displays the resources matching
 * the specified Query.
 */
class KACTIVITIESSTATS_EXPORT ResultModel : public QAbstractListModel {
    Q_OBJECT

public:
    ResultModel(Query query, QObject *parent = nullptr);
    ResultModel(Query query, const QString &clientId, QObject *parent = nullptr);
    ~ResultModel() override;

    enum Roles {
        ResourceRole         = Qt::UserRole,
        TitleRole            = Qt::UserRole + 1,
        ScoreRole            = Qt::UserRole + 2,
        FirstUpdateRole      = Qt::UserRole + 3,
        LastUpdateRole       = Qt::UserRole + 4,
        LinkStatusRole       = Qt::UserRole + 5,
        LinkedActivitiesRole = Qt::UserRole + 6
    };

    int rowCount(const QModelIndex &parent
                 = QModelIndex()) const override;
    QVariant data(const QModelIndex &item,
                  int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    void fetchMore(const QModelIndex &parent) override;
    bool canFetchMore(const QModelIndex &parent) const override;

    void linkToActivity(const QUrl &resource,
                        const Terms::Activity &activity
                            = Terms::Activity(QStringList()),
                        const Terms::Agent &agent
                            = Terms::Agent(QStringList()));

    void unlinkFromActivity(const QUrl &resource,
                            const Terms::Activity &activity
                                = Terms::Activity(QStringList()),
                            const Terms::Agent &agent
                                = Terms::Agent(QStringList()));

public Q_SLOTS:
    /**
     * Removes the specified resource from the history
     */
    void forgetResource(const QString &resource);

    /**
     * Removes the specified resource from the history
     */
    void forgetResource(int row);

    /**
     * Clears the history of all resources that match the current
     * model query
     */
    void forgetAllResources();

    /**
     * Moves the resource to the specified position.
     *
     * Note that this only applies to the linked resources
     * since the recently/frequently used ones have
     * their natural order.
     *
     * @note This requires the clientId to be specified on construction.
     */
    void setResultPosition(const QString &resource, int position);

    /**
     * Sort the items by title.
     *
     * Note that this only affects the linked resources
     * since the recently/frequently used ones have
     * their natural order.
     *
     * @note This requires the clientId to be specified on construction.
     */
    void sortItems(Qt::SortOrder sortOrder);

private:
    friend class ResultModelPrivate;
    ResultModelPrivate *const d;
};

} // namespace Stats
} // namespace KActivities

#endif // KACTIVITIES_STATS_RESULTMODEL_H

