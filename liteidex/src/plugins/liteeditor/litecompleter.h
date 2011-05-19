/**************************************************************************
** This file is part of LiteIDE
**
** Copyright (c) 2011 LiteIDE Team. All rights reserved.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** In addition, as a special exception,  that plugins developed for LiteIDE,
** are allowed to remain closed sourced and can be distributed under any license .
** These rights are included in the file LGPL_EXCEPTION.txt in this package.
**
**************************************************************************/
// Module: litecompleter.h
// Creator: visualfc <visualfc@gmail.com>
// date: 2011-3-26
// $Id: litecompleter.h,v 1.0 2011-5-12 visualfc Exp $

#ifndef LITECOMPLETER_H
#define LITECOMPLETER_H

#include <QObject>
#include <QModelIndex>
#include "liteeditorapi/liteeditorapi.h"

class QCompleter;
class QPlainTextEdit;
class TreeModelCompleter;
class QStandardItemModel;
class LiteCompleter : public LiteApi::ICompleter
{
    Q_OBJECT
public:
    explicit LiteCompleter(QObject *parent = 0);
    void setEditor(QPlainTextEdit *editor);
    virtual QCompleter *completer() const;
    virtual void appendItem(QString item,bool temp);
    virtual void appendItems(QStringList items,bool temp);
    virtual void clear();
    virtual void clearTemp();
    virtual void show();
public slots:
    virtual void completionPrefixChanged(QString);
    virtual void insertCompletion(QModelIndex);
protected:
    TreeModelCompleter *m_completer;
    QStandardItemModel *m_model;
    QPlainTextEdit *m_editor;
    QChar           m_stop;
};

#endif // LITECOMPLETER_H
