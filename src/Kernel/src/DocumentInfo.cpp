/* This file is part of the Screenie project.
   Screenie is a fancy screenshot composer.

   Copyright (C) 2011 Oliver Knoll <till.oliver.knoll@gmail.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <QMainWindow>
#include <QString>
#include <QFileInfo>

#include "../../../Utils/src/FileUtils.h"
#include "../../../Model/src/ScreenieScene.h"

#include "DocumentInfo.h"

class DocumentInfoPrivate
{
public:
    DocumentInfoPrivate(QMainWindow &theMainWindow, const ScreenieScene *theScreenieScene)
        : mainWindow(theMainWindow),
          screenieScene(theScreenieScene),
          saveStrategy(DocumentInfo::SaveStrategy::Ask)
    {}
    ~DocumentInfoPrivate() {}

    int id;
    QMainWindow &mainWindow;
    const ScreenieScene *screenieScene;
    QString fileName;
    QString filePath;
    DocumentInfo::SaveStrategy saveStrategy;
};

// public

DocumentInfo::DocumentInfo(QMainWindow &mainWindow, const ScreenieScene *screenieScene)
{
    d = new DocumentInfoPrivate(mainWindow, screenieScene);
}

DocumentInfo::~DocumentInfo()
{
#ifdef DEBUG
    qDebug("DocumentInfo::~DocumentInfo(): called.");
#endif
    delete d;
}

int DocumentInfo::getId() const
{
    return d->id;
}

void DocumentInfo::setId(int id)
{
    d->id = id;
}

QMainWindow &DocumentInfo::getMainWindow() const
{
    return d->mainWindow;
}

const ScreenieScene *DocumentInfo::getScreenieScene() const
{
    return d->screenieScene;
}

void DocumentInfo::setScreenieScene(const ScreenieScene *screenieScene)
{
    d->screenieScene = screenieScene;
}

QString DocumentInfo::getName() const
{
    QString result;
    result = getFileName();
    if (result.endsWith(FileUtils::SceneExtension)) {
        // chop including the dot (.) -> +1
        result.chop(FileUtils::SceneExtension.length() + 1);
    } else if (result.endsWith(FileUtils::TemplateExtension)) {
        // chop including the dot (.) -> +1
        result.chop(FileUtils::TemplateExtension.length() + 1);
    }
    return result;
}

QString DocumentInfo::getFileName() const
{
    return d->fileName;
}

void DocumentInfo::setFileName(const QString &fileName)
{
    if (d->fileName != fileName) {
        d->fileName = fileName;
        d->filePath = QString();
    }
}

QString DocumentInfo::getFilePath() const
{
    return d->filePath;
}

void DocumentInfo::setFilePath(const QString &filePath)
{
    if (d->filePath != filePath) {
        d->filePath = filePath;
        d->fileName = QFileInfo(d->filePath).fileName();
    }
}

DocumentInfo::SaveStrategy DocumentInfo::getSaveStrategy() const
{
    return d->saveStrategy;
}

void DocumentInfo::setSaveStrategy(SaveStrategy saveStrategy)
{
    d->saveStrategy = saveStrategy;
}

bool DocumentInfo::isModified() const
{
    return d->screenieScene->isModified();
}
