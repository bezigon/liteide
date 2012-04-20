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
// Module: packageproject.cpp
// Creator: visualfc <visualfc@gmail.com>
// date: 2012-4-19
// $Id: packageproject.cpp,v 1.0 2012-4-19 visualfc Exp $

#include "packageproject.h"
#include "filepathmodel.h"
#include "gotool.h"
#include "qjson/include/QJson/Parser"
#include "fileutil/fileutil.h"
#include "golangdocapi/golangdocapi.h"
#include <QDir>
#include <QFileInfo>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QVBoxLayout>
#include <QTimer>
#include <QUrl>
#include <QDebug>
//lite_memory_check_begin
#if defined(WIN32) && defined(_MSC_VER) &&  defined(_DEBUG)
     #define _CRTDBG_MAP_ALLOC
     #include <stdlib.h>
     #include <crtdbg.h>
     #define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
     #define new DEBUG_NEW
#endif
//lite_memory_check_end

PackageProject::PackageProject(LiteApi::IApplication *app) :
    m_liteApp(app)
{
    m_goTool = new GoTool(m_liteApp,this);

    m_widget = new QWidget;   
    m_reloadTimer = new QTimer(this);
    m_reloadTimer->setSingleShot(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    m_treeView = new PackageTree(m_widget);
    m_treeView->setEditTriggers(QTreeView::NoEditTriggers);
    m_treeView->setHeaderHidden(true);
    layout->addWidget(m_treeView);
    m_widget->setLayout(layout);

    connect(m_treeView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleClicked(QModelIndex)));
    connect(m_goTool,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(finished(int,QProcess::ExitStatus)));
    connect(m_liteApp->editorManager(),SIGNAL(editorSaved(LiteApi::IEditor*)),this,SLOT(editorSaved(LiteApi::IEditor*)));
    connect(m_reloadTimer,SIGNAL(timeout()),this,SLOT(reload()));
}

PackageProject::~PackageProject()
{
    if (m_reloadTimer) {
        m_reloadTimer->stop();
        delete m_reloadTimer;
    }
    if (m_goTool) {
        m_goTool->kill();
        delete m_goTool;
    }
}

void PackageProject::setPath(const QString &path)
{
    m_goTool->setWorkDir(path);
}

void PackageProject::reload()
{
    m_goTool->reloadEnv();
    m_goTool->start(QStringList() << "list" << "-e" << "-json" << ".");
}

void PackageProject::setJson(const QMap<QString,QVariant> &json)
{
    m_json = json;
    m_goTool->setWorkDir(m_json.value("Dir").toString());
}

QWidget *PackageProject::widget()
{
    return m_widget;
}

QString PackageProject::name() const
{
    return m_json.value("ImportPath").toString();
}
QString PackageProject::filePath() const
{
    return m_json.value("Dir").toString();
}
QString PackageProject::mimeType() const
{
    if (m_json.value("Name") == "main") {
        return "text/x-gocommand";
    }
    return "text/x-gopackage";
}
QStringList PackageProject::fileNameList() const
{
    return m_treeView->nameList;
}
QStringList PackageProject::filePathList() const
{
    return m_treeView->fileList;
}
QString PackageProject::fileNameToFullPath(const QString &filePath)
{
    QDir dir(m_json.value("Dir").toString());
    return QFileInfo(dir,filePath).filePath();
}

/*
valueForKey

EDITORPATH
EDITORNAME
EDITORDIR

PROJECTPATH
PROJECTNAME
PROJECTDIR

WORKDIR

TARGETPATH
TARGETNAME
TARGETDIR
*/

QMap<QString,QString> PackageProject::projectInfo() const
{
    QMap<QString,QString> m;
    QFileInfo info(m_json.value("Dir").toString());
    m.insert("PROJECTPATH",info.filePath());
    m.insert("PROJECTNAME",info.fileName());
    m.insert("PROJECTDIR",info.filePath());
    return m;
}

QMap<QString,QString> PackageProject::targetInfo() const
{
    QMap<QString,QString> m;
    QDir dir(m_json.value("Dir").toString());
    m.insert("WORKDIR",dir.path());
    QString name = m_json.value("ImportPath").toString();
    m.insert("TARGETPATH",QFileInfo(dir,name).filePath());
    m.insert("TARGETNAME",name);
    m.insert("TARGETDIR",dir.path());
    return m;
}

void PackageProject::load()
{
    //m_model->setRootPath(m_json.value("Dir").toString());
    m_treeView->loadJson(m_json);
}

void PackageProject::doubleClicked(QModelIndex index)
{
    if (!index.isValid()) {
        return;
    }
    QStandardItem *item = m_treeView->model->itemFromIndex(index);
    if (!item) {
        return;
    }
    switch (item->data(PackageTree::RoleItem).toInt()) {
        case PackageTree::ITEM_SOURCE: {
            QString path = item->data(RolePath).toString();
            m_liteApp->fileManager()->openEditor(path,true);
        }
        break;
    case PackageTree::ITEM_DEP:
    case PackageTree::ITEM_IMPORT: {
        QString pkg = item->data(Qt::DisplayRole).toString();
        LiteApi::IGolangDoc *doc = LiteApi::getGolangDoc(m_liteApp);
        if (doc) {
            doc->openUrl(QUrl(QString("pdoc:%1").arg(pkg)));
            doc->activeBrowser();
        }
        }
        break;
    }

}

void PackageProject::finished(int code, QProcess::ExitStatus)
{
    if (code != 0) {
        return;
    }
    QJson::Parser parser;
    bool ok = false;
    QVariant json = parser.parse(m_goTool->stdOutputData(), &ok).toMap();
    if (ok) {
        m_json = json.toMap();
        load();
    }
}

void PackageProject::editorSaved(LiteApi::IEditor *editor)
{
    if (!editor) {
        return;
    }
    QString filePath = editor->filePath();
    bool find = false;
    foreach(QString path, m_treeView->fileList) {
        if (FileUtil::compareFile(filePath,path)) {
            find = true;
            break;
        }
    }
    if (find) {
        m_reloadTimer->start(1000);
    }
}