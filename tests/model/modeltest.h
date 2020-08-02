/*
    SPDX-FileCopyrightText: 2013 Digia Plc and /or its subsidiary(-ies). <http://www.qt-project.org/legal>

    This file is part of the test suite of the Qt Toolkit.

    SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-2.1-only WITH Qt-LGPL-exception-1.1 OR GPL-3.0-only
*/

#ifndef MODELTEST_H
#define MODELTEST_H

#include <QObject>
#include <QAbstractItemModel>
#include <QStack>

class ModelTest : public QObject
{
  Q_OBJECT

public:
  ModelTest( QAbstractItemModel *model, QObject *parent = nullptr );

private Q_SLOTS:
  void nonDestructiveBasicTest();
  void rowCount();
  void columnCount();
  void hasIndex();
  void index();
  void parent();
  void data();

protected Q_SLOTS:
  void runAllTests();
  void layoutAboutToBeChanged();
  void layoutChanged();
  void rowsAboutToBeInserted( const QModelIndex &parent, int start, int end );
  void rowsInserted( const QModelIndex & parent, int start, int end );
  void rowsAboutToBeRemoved( const QModelIndex &parent, int start, int end );
  void rowsRemoved( const QModelIndex & parent, int start, int end );
  void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
  void headerDataChanged(Qt::Orientation orientation, int start, int end);

private:
  void checkChildren( const QModelIndex &parent, int currentDepth = 0 );

  QAbstractItemModel *model;

  struct Changing {
    QModelIndex parent;
    int oldSize;
    QVariant last;
    QVariant next;
  };
  QStack<Changing> insert;
  QStack<Changing> remove;

  bool fetchingMore;

  QList<QPersistentModelIndex> changing;
};

#endif
