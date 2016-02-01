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

#ifndef KACTIVITIES_STATS_QUERY_H
#define KACTIVITIES_STATS_QUERY_H

#include <qcompilerdetection.h>

#ifdef Q_COMPILER_INITIALIZER_LISTS
#include <initializer_list>
#endif

#include <QString>
#include <QStringList>

#include "kactivitiesstats_export.h"

#include "terms.h"

namespace KActivities {
namespace Stats {

class QueryPrivate;

/**
 * The activities system tracks resources (documents, contacts, etc.)
 * that the user has used. It also allows linking resources to
 * specific activities (like bookmarks, favorites, etc.).
 *
 * The Query class specifies which resources to return -
 * the previously used ones, the linked ones, or to
 * combine these two.
 *
 * It allows filtering the results depending on the resource type,
 * the agent (application that reported the usage event,
 * @see KActivities::ResourceInstance) and the activity the resource
 * has been used in, or linked to. It also allows filtering
 * on the URL of the resource.
 *
 * While it can be explicitly instantiated, a preferred approach
 * is to use the pipe syntax like this:
 *
 * <code>
 * auto query = UsedResources
 *                 | RecentlyUsedFirst
 *                 | Agent::any()
 *                 | Type::any()
 *                 | Activity::current();
 * </code>
 */
class KACTIVITIESSTATS_EXPORT Query {
public:
    Query(Terms::Select selection = Terms::AllResources);

    // The damned rule of five minus one :)
    Query(Query && source);
    Query(const Query &source);
    Query &operator= (Query source);
    ~Query();

    // Not all are born equal
    bool operator== (const Query &right) const;
    bool operator!= (const Query &right) const;

    Terms::Select selection() const;
    QStringList types() const;
    QStringList agents() const;
    QStringList activities() const;

    QStringList urlFilters() const;
    Terms::Order ordering() const;
    int offset() const;
    int limit() const;

    void setSelection(Terms::Select selection);

    void addTypes(const QStringList &types);
    void addAgents(const QStringList &agents);
    void addActivities(const QStringList &activities);
    void addUrlFilters(const QStringList &urlFilters);
    void setOrdering(Terms::Order ordering);
    void setOffset(int offset);
    void setLimit(int limit);

    void clearTypes();
    void clearAgents();
    void clearActivities();
    void clearUrlFilters();

    void removeTypes(const QStringList &types);
    void removeAgents(const QStringList &agents);
    void removeActivities(const QStringList &activities);
    void removeUrlFilters(const QStringList &urlFilters);

private:
    inline void addTerm(const Terms::Type &term)
    {
        addTypes(term.values);
    }

    inline void addTerm(const Terms::Agent &term)
    {
        addAgents(term.values);
    }

    inline void addTerm(const Terms::Activity &term)
    {
        addActivities(term.values);
    }

    inline void addTerm(const Terms::Url &term)
    {
        addUrlFilters(term.values);
    }

    inline void addTerm(Terms::Order ordering)
    {
        setOrdering(ordering);
    }

    inline void addTerm(Terms::Select selection)
    {
        setSelection(selection);
    }

    inline void addTerm(Terms::Limit limit)
    {
        setLimit(limit.value);
    }

    inline void addTerm(Terms::Offset offset)
    {
        setOffset(offset.value);
    }

public:

    template <typename Term>
    friend
    inline Query operator| (const Query &query, Term &&term)
    {
        Query result(query);
        result.addTerm(term);
        return result;
    }

    template <typename Term>
    friend
    inline Query operator| (Query &&query, Term &&term)
    {
        query.addTerm(term);
        return query;
    }

private:
    QueryPrivate* d;
};

template <typename Term>
inline Query operator| (Terms::Select selection, Term &&term)
{
    return Query(selection) | term;
}


} // namespace Stats
} // namespace KActivities

KACTIVITIESSTATS_EXPORT
QDebug operator<<(QDebug dbg, const KActivities::Stats::Query &query);

#endif // KACTIVITIES_STATS_QUERY_H

