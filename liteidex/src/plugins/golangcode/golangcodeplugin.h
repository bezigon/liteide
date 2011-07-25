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
// Module: golangcodeplugin.h
// Creator: visualfc <visualfc@gmail.com>
// date: 2011-5-19
// $Id: golangcodeplugin.h,v 1.0 2011-7-25 visualfc Exp $

#ifndef GOLANGCODEPLUGIN_H
#define GOLANGCODEPLUGIN_H

#include "golangcode_global.h"
#include "liteapi.h"
#include <QtPlugin>

class GolangCode;
class GolangCodePlugin : public LiteApi::IPlugin
{
    Q_OBJECT
    Q_INTERFACES(LiteApi::IPlugin)
public:
    GolangCodePlugin();
    virtual bool initWithApp(LiteApi::IApplication *app);
    virtual QStringList dependPluginList() const;
protected slots:
    void currentEditorChanged(LiteApi::IEditor*);
protected:
    GolangCode *m_code;
};

#endif // GOLANGCODEPLUGIN_H
