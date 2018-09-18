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

#include "window.h"

#include "ui_window.h"
#include "modeltest.h"

#include <QAction>
#include <QListView>
#include <QDebug>
#include <QCoreApplication>
#include <QItemDelegate>
#include <QPainter>
#include <QDateTime>

#include <QQmlContext>
#include <QQmlComponent>
#include <QQuickItem>

#include <KActivities/Consumer>

#include <boost/range/numeric.hpp>

namespace KAStats = KActivities::Stats;

using namespace KAStats;
using namespace KAStats::Terms;

class Delegate: public QItemDelegate {
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override
    {
        painter->save();

        const QString title = index.data(ResultModel::TitleRole).toString();

        QRect titleRect = painter->fontMetrics().boundingRect(title);
        int lineHeight = titleRect.height();

        // Header background
        titleRect.moveTop(option.rect.top());
        titleRect.setWidth(option.rect.width());

        painter->fillRect(titleRect.x(), titleRect.y(),
                          titleRect.width(), titleRect.height() + 16,
                          QColor(32, 32, 32));

        // Painting the title
        painter->setPen(QColor(255,255,255));

        titleRect.moveTop(titleRect.top() + 8);
        titleRect.setLeft(8);
        titleRect.setWidth(titleRect.width() - 8);
        painter->drawText(titleRect, index.data(ResultModel::TitleRole).toString());

        // Painting the score
        painter->drawText(titleRect,
                          QStringLiteral("Score: ") + QString::number(index.data(ResultModel::ScoreRole).toDouble()),
                          QTextOption(Qt::AlignRight));

        // Painting the moification and creation times
        titleRect.moveTop(titleRect.bottom() + 16);

        painter->fillRect(titleRect.x() - 4, titleRect.y() - 8,
                          titleRect.width() + 8, titleRect.height() + 8 + lineHeight,
                          QColor(64, 64, 64));

        titleRect.moveTop(titleRect.top() - 4);

        painter->drawText(titleRect,
                          index.data(ResultModel::ResourceRole).toString()
                          );

        auto firstUpdate = QDateTime::fromTime_t(index.data(ResultModel::FirstUpdateRole).toUInt());
        auto lastUpdate = QDateTime::fromTime_t(index.data(ResultModel::LastUpdateRole).toUInt());

        titleRect.moveTop(titleRect.top() + lineHeight);

        painter->drawText(titleRect,
                          QStringLiteral("Modified: ") + lastUpdate.toString()
                          );
        painter->drawText(titleRect,
                          QStringLiteral("Created: ") + firstUpdate.toString(),
                          QTextOption(Qt::AlignRight));

        painter->restore();

    }

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override
    {
        Q_UNUSED(option);
        Q_UNUSED(index);
        return QSize(0, 100);
    }
};

Window::Window()
    : ui(new Ui::MainWindow())
    , model(nullptr)
    , activities(new KActivities::Consumer())
{
    ui->setupUi(this);
    ui->viewResults->setItemDelegate(new Delegate());
    // ui->viewResults->setUniformItemSizes(true);
    // ui->viewResults->setGridSize(QSize(200, 100));

    while (activities->serviceStatus() == KActivities::Consumer::Unknown) {
        QCoreApplication::processEvents();
    }

    connect(ui->buttonUpdate, SIGNAL(clicked()),
            this, SLOT(updateResults()));
    connect(ui->buttonReloadRowCount, SIGNAL(clicked()),
            this, SLOT(updateRowCount()));

    for (const auto &activity :
         (QStringList() << QStringLiteral(":current")
                        << QStringLiteral(":any")
                        << QStringLiteral(":global")) + activities->activities()) {
        ui->comboActivity->addItem(activity);
    }

    foreach (const auto &arg, QCoreApplication::arguments()) {
        if (arg == QLatin1String("--used")) {
            ui->radioSelectUsedResources->setChecked(true);

        } else if (arg == QLatin1String("--linked")) {
            ui->radioSelectLinkedResources->setChecked(true);

        } else if (arg == QLatin1String("--combined")) {
            ui->radioSelectAllResources->setChecked(true);

        } else if (arg.startsWith(QLatin1String("--activity="))) {
            ui->comboActivity->setCurrentText(arg.split(QLatin1String("="))[1]);

        } else if (arg.startsWith(QLatin1String("--agent="))) {
            ui->textAgent->setText(arg.split(QLatin1String("="))[1]);

        } else if (arg.startsWith(QLatin1String("--mimetype="))) {
            ui->textMimetype->setText(arg.split(QLatin1String("="))[1]);

        } else if (arg == QLatin1String("--select")) {
            updateResults();

        }
    }

    auto redisplayAction = new QAction(this);
    addAction(redisplayAction);
    redisplayAction->setShortcut(Qt::Key_F5);
    connect(redisplayAction, SIGNAL(triggered()),
            this, SLOT(updateResults()));

    // loading the presets

    const auto recentQueryBase
                 = UsedResources
                    | RecentlyUsedFirst
                    | Agent::any()
                    | Type::any()
                    | Activity::current();

    const auto popularQueryBase
                 = UsedResources
                    | HighScoredFirst
                    | Agent::any()
                    | Type::any()
                    | Activity::current();

    presets = {
        { QStringLiteral("kicker-favorites"),
            LinkedResources
                | Agent {
                    QStringLiteral("org.kde.plasma.favorites.applications"),
                    QStringLiteral("org.kde.plasma.favorites.documents"),
                    QStringLiteral("org.kde.plasma.favorites.contacts")
                  }
                | Type::any()
                | Activity::current()
                | Activity::global()
                | Limit(15)
        },
        { QStringLiteral("kicker-recent-apps-n-docs"),
            recentQueryBase | Url::startsWith(QStringLiteral("applications:")) | Url::file() | Limit(30)
        },
        { QStringLiteral("kicker-recent-apps"),
            recentQueryBase | Url::startsWith(QStringLiteral("applications:")) | Limit(15)
        },
        { QStringLiteral("kicker-recent-docs"),
            recentQueryBase | Url::file() | Limit(15)
        },
        { QStringLiteral("kicker-popular-apps-n-docs"),
            popularQueryBase | Url::startsWith(QStringLiteral("applications:")) | Url::file() | Limit(30)
        },
        { QStringLiteral("kicker-popular-apps"),
            popularQueryBase | Url::startsWith(QStringLiteral("applications:")) | Limit(15)
        },
        { QStringLiteral("kicker-popular-docs"),
            popularQueryBase | Url::file() | Limit(15)
        }
    };

    ui->comboPreset->addItem(QStringLiteral("Choose a preset"), QVariant());
    for (const auto& presetId: presets.keys()) {
        ui->comboPreset->addItem(presetId, presetId);
    }

    connect(ui->comboPreset, SIGNAL(activated(int)),
            this, SLOT(selectPreset()));
}

Window::~Window()
{
}

void Window::selectPreset()
{
    const auto id = ui->comboPreset->currentData().toString();

    if (id.isEmpty()) return;

    const auto &query = presets[id];
    qDebug() << "Id: " << id;
    qDebug() << "Query: " << query;

    // Selection
    qDebug() << "\tSelection:" << query.selection();
    ui->radioSelectUsedResources->setChecked(query.selection() == UsedResources);
    ui->radioSelectLinkedResources->setChecked(query.selection() == LinkedResources);
    ui->radioSelectAllResources->setChecked(query.selection() == AllResources);

    // Ordering
    qDebug() << "\tOrdering:" << query.ordering();
    ui->radioOrderHighScoredFirst->setChecked(query.ordering() == HighScoredFirst);
    ui->radioOrderRecentlyUsedFirst->setChecked(query.ordering() == RecentlyUsedFirst);
    ui->radioOrderRecentlyCreatedFirst->setChecked(query.ordering() == RecentlyCreatedFirst);
    ui->radioOrderByUrl->setChecked(query.ordering() == OrderByUrl);
    ui->radioOrderByTitle->setChecked(query.ordering() == OrderByTitle);

    // Agents
    qDebug() << "\tAgents:" << query.agents();
    ui->textAgent->setText(query.agents().join(QLatin1Char(',')));

    // Types
    qDebug() << "\tTypes:" << query.types();
    ui->textMimetype->setText(query.types().join(QLatin1Char(',')));

    // Activities
    qDebug() << "\tActivities:" << query.activities();
    ui->comboActivity->setEditText(query.activities().join(QLatin1Char(',')));

    // Url filters
    qDebug() << "\tUrl filters:" << query.urlFilters();
    ui->textUrl->setText(query.urlFilters().join(QLatin1Char(',')));

    // Limit
    ui->spinLimitCount->setValue(query.limit());

    updateResults();
}

void Window::updateRowCount()
{
    ui->labelRowCount->setText(QString::number(
            ui->viewResults->model()->rowCount()
        ));
}

void Window::setQuery(const KActivities::Stats::Query &query)
{
    qDebug() << "Updating the results";

    ui->viewResults->setModel(nullptr);

    // Log results
    using boost::accumulate;

    ui->textLog->setText(
        accumulate(ResultSet(query), QString(),
            [] (const QString &acc, const ResultSet::Result &result) {
                return acc + result.title() + QStringLiteral(" (") + result.resource() + QStringLiteral(")\n");
            })
        );

    model.reset(new ResultModel(query));

    if (QCoreApplication::arguments().contains(QLatin1String("--enable-model-test"))) {
        modelTest.reset();
        modelTest.reset(new ModelTest(new ResultModel(query)));
    }

    ui->viewResults->setModel(model.get());

    // QML

    auto context = ui->viewResultsQML->rootContext();
    ui->viewResultsQML->setResizeMode(QQuickWidget::SizeRootObjectToView);

    context->setContextProperty(QStringLiteral("kamdmodel"), model.get());

    ui->viewResultsQML->setSource(QUrl(QStringLiteral("qrc:/main.qml")));
}

void Window::updateResults()
{
    qDebug() << "Updating the results";

    setQuery(
        // What should we get
        (
            ui->radioSelectUsedResources->isChecked()   ? UsedResources :
            ui->radioSelectLinkedResources->isChecked() ? LinkedResources :
                                                          AllResources
        ) |

        // How we should order it
        (
            ui->radioOrderHighScoredFirst->isChecked()      ? HighScoredFirst :
            ui->radioOrderRecentlyUsedFirst->isChecked()    ? RecentlyUsedFirst :
            ui->radioOrderRecentlyCreatedFirst->isChecked() ? RecentlyCreatedFirst :
            ui->radioOrderByUrl->isChecked()                ? OrderByUrl :
                                                              OrderByTitle
        ) |

        // Which agents?
        Agent(ui->textAgent->text().split(QLatin1Char(','))) |

        // Which mime?
        Type(ui->textMimetype->text().split(QLatin1Char(','))) |

        // Which activities?
        Activity(ui->comboActivity->currentText().split(QLatin1Char(','))) |

        // And URL filters
        Url(ui->textUrl->text().split(QLatin1Char(','), QString::SkipEmptyParts)) |

        // And how many items
        Limit(ui->spinLimitCount->value())
    );
}

