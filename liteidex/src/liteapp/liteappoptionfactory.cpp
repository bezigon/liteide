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
// Module: liteappoptionfactory.h
// Creator: visualfc <visualfc@gmail.com>
// date: 2011-6-27
// $Id: liteappoptionfactory.cpp,v 1.0 2011-6-27 visualfc Exp $

#include "liteappoption.h"
#include "liteappoptionfactory.h"

LiteAppOptionFactory::LiteAppOptionFactory(LiteApi::IApplication *app, QObject *parent)
    : LiteApi::IOptionFactory(parent),
      m_liteApp(app)
{
}

QStringList LiteAppOptionFactory::mimeTypes() const
{
    return QStringList() << "option/liteapp";
}

LiteApi::IOption *LiteAppOptionFactory::create(const QString &mimeType)
{
    if (mimeType == "option/liteapp") {
        return new LiteAppOption(m_liteApp,this);
    }
    return 0;
}
