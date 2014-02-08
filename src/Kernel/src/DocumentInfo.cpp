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

#include "../../../Model/src/ScreenieScene.h"

#include "DocumentInfo.h"

class DocumentInfoPrivate
{
public:
    DocumentInfoPrivate()
    {}
    ~DocumentInfoPrivate() {}

    int id;
    QMainWindow *mainWindow;
    ScreenieScene *screenieScene;
    QString name;
    DocumentInfo::SaveStrategy saveStrategy;
};

// public

DocumentInfo::DocumentInfo()
{
    d = new DocumentInfoPrivate();
}

DocumentInfo::~DocumentInfo()
{
#ifdef DEBUG
    qDebug("DocumentInfo::~DocumentInfo(): CALLED.");
#endif
    delete d;
}

int DocumentInfo::getWindowId() const
{
    return d->id;
}

void DocumentInfo::setWindowId(int id)
{
    d->id = id;
}

QMainWindow *DocumentInfo::getMainWindow() const
{
    return d->mainWindow;
}

void DocumentInfo::setMainWindow(QMainWindow *mainWindow)
{
    d->mainWindow = mainWindow;
}

ScreenieScene *DocumentInfo::getScreenieScene() const
{
    return d->screenieScene;
}

void DocumentInfo::setScreenieScene(ScreenieScene *screenieScene)
{
    d->screenieScene = screenieScene;
}

QString DocumentInfo::getName() const
{
    return d->name;
}

void DocumentInfo::setName(const QString &name)
{
    d->name = name;
}

DocumentInfo::SaveStrategy DocumentInfo::getSaveStrategy() const
{
    return d->saveStrategy;
}

void DocumentInfo::setSaveStrategy(SaveStrategy saveStrategy)
{
    d->saveStrategy = saveStrategy;
}
