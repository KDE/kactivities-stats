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

#include "query.h"
#include <QDate>
#include <QDebug>

namespace KActivities {
namespace Stats {

namespace details {
    inline void validateTypes(QStringList &types)
    {
        // Nothing at the moment
        Q_UNUSED(types);
    }

    inline void validateAgents(QStringList &agents)
    {
        // Nothing at the moment
        Q_UNUSED(agents);
    }

    inline void validateActivities(QStringList &activities)
    {
        // Nothing at the moment
        Q_UNUSED(activities);
    }

    inline void validateUrlFilters(QStringList &urlFilters)
    {
        auto i = urlFilters.begin();
        const auto end = urlFilters.end();

        for (; i != end ; ++i) {
            i->replace(QLatin1String("'"), QLatin1String(""));
        }
    }

} // namespace details

class QueryPrivate {
public:
    QueryPrivate()
        : ordering(Terms::HighScoredFirst)
        , limit(0)
        , offset(0)
    {
    }

    Terms::Select   selection;
    QStringList     types;
    QStringList     agents;
    QStringList     activities;
    QStringList     urlFilters;
    Terms::Order    ordering;
    QDate           date;
    int             limit;
    int             offset;
};

Query::Query(Terms::Select selection)
    : d(new QueryPrivate())
{
    d->selection = selection;
}

Query::Query(Query &&source)
    : d(nullptr)
{
    std::swap(d, source.d);
}

Query::Query(const Query &source)
    : d(new QueryPrivate(*source.d))
{
}

Query &Query::operator= (Query source)
{
    std::swap(d, source.d);
    return *this;
}


Query::~Query()
{
    delete d;
}

bool Query::operator== (const Query &right) const
{
    return selection()  == right.selection() &&
           types()      == right.types() &&
           agents()     == right.agents() &&
           activities() == right.activities() &&
           selection()  == right.selection() &&
           urlFilters() == right.urlFilters() &&
           date()       == right.date();
}

bool Query::operator!= (const Query &right) const
{
    return !(*this == right);
}

#define IMPLEMENT_QUERY_LIST_FIELD(WHAT, What, Default)                        \
    void Query::add##WHAT(const QStringList &What)                             \
    {                                                                          \
        d->What << What;                                                       \
        details::validate##WHAT(d->What);                                      \
    }                                                                          \
                                                                               \
    QStringList Query::What() const                                            \
    {                                                                          \
        return d->What.size() ? d->What : Default;                             \
    }                                                                          \
                                                                               \
    void Query::clear##WHAT()                                                  \
    {                                                                          \
        d->What.clear();                                                       \
    }

IMPLEMENT_QUERY_LIST_FIELD(Types,      types,      QStringList(QStringLiteral(":any")))
IMPLEMENT_QUERY_LIST_FIELD(Agents,     agents,     QStringList(QStringLiteral(":current")))
IMPLEMENT_QUERY_LIST_FIELD(Activities, activities, QStringList(QStringLiteral(":current")))
IMPLEMENT_QUERY_LIST_FIELD(UrlFilters, urlFilters, QStringList(QStringLiteral("*")))

#undef IMPLEMENT_QUERY_LIST_FIELD

void Query::setOrdering(Terms::Order ordering)
{
    d->ordering = ordering;
}

void Query::setSelection(Terms::Select selection)
{
    d->selection = selection;
}

void Query::setLimit(int limit)
{
    d->limit = limit;
}

void Query::setOffset(int offset)
{
    d->offset = offset;
}

void Query::setDate(QDate date)
{
    d->date = date;
}

Terms::Order Query::ordering() const
{
    return d->ordering;
}

Terms::Select Query::selection() const
{
    return d->selection;
}

int Query::limit() const
{
    return d->limit;
}

int Query::offset() const
{
    Q_ASSERT_X(d->limit > 0, "Query::offset", "Offset can only be specified if limit is set");
    return d->offset;
}

QDate Query::date() const
{
    return d->date;
}

} // namespace Stats
} // namespace KActivities

namespace KAStats = KActivities::Stats;

QDebug operator<<(QDebug dbg, const KAStats::Query &query)
{
    using namespace KAStats::Terms;

    dbg.nospace()
        << "Query { "
        << query.selection()
        << ", " << Type(query.types())
        << ", " << Agent(query.agents())
        << ", " << Activity(query.activities())
        << ", " << Url(query.urlFilters())
        << ", " << Date(query.date())
        << ", " << query.ordering()
        << ", Limit: " << query.limit()
        << " }";
    return dbg;
}
