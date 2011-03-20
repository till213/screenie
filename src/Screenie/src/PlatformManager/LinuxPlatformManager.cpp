/* This file is part of the Screenie project.
   Screenie is a fancy screenshot composer.

   Copyright (C) 2008 Ariya Hidayat <ariya.hidayat@gmail.com>

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

#include <QtGui/QAction>
#include <QtGui/QKeySequence>
#include <QtGui/QMainWindow>

#include "ui_MainWindow.h"
#include "LinuxPlatformManager.h"

class LinuxPlatformManagerPrivate
{
public:
    LinuxPlatformManagerPrivate(Ui::MainWindow &theMainWindowUi)
        : mainWindowUi(theMainWindowUi)
    {}

    Ui::MainWindow &mainWindowUi;
};

// public

LinuxPlatformManager::LinuxPlatformManager()
    :d(0)
{}

LinuxPlatformManager::~LinuxPlatformManager()
{
    if (d != 0) {
        delete d;
    }
}

void LinuxPlatformManager::initialize(QMainWindow &mainWindow, Ui::MainWindow &mainWindowUi)
{
    d = new LinuxPlatformManagerPrivate(mainWindowUi);
    AbstractPlatformManager::initialize(mainWindow, mainWindowUi);
    mainWindowUi.toggleFullScreenAction->setShortcut(QKeySequence(Qt::Key_F11));
}

// protected

void LinuxPlatformManager::initializePlatformIcons(Ui::MainWindow &mainWindowUi)
{
    // File menu
    QIcon newIcon(":/img/document-new.png");
    mainWindowUi.newAction->setIcon(QIcon::fromTheme("document-new", newIcon));

    QIcon openIcon(":/img/document-open.png");
    mainWindowUi.openAction->setIcon(QIcon::fromTheme("document-open", openIcon));

    QIcon saveIcon(":/img/document-save.png");
    mainWindowUi.saveAction->setIcon(QIcon::fromTheme("document-save", saveIcon));

    QIcon saveAsIcon(":/img/document-save-as.png");
    mainWindowUi.saveAsAction->setIcon(QIcon::fromTheme("document-save-as", saveAsIcon));

    QIcon exitIcon(":/img/application-exit.png");
    mainWindowUi.quitAction->setIcon(QIcon::fromTheme("application-exit", exitIcon));

    // Edit menu
    QIcon cutIcon(":/img/edit-cut.png");
    mainWindowUi.cutAction->setIcon(QIcon::fromTheme("edit-cut", cutIcon));

    QIcon copyIcon(":/img/edit-copy.png");
    mainWindowUi.copyAction->setIcon(QIcon::fromTheme("edit-copy", copyIcon));

    QIcon pasteIcon(":/img/edit-paste.png");
    mainWindowUi.pasteAction->setIcon(QIcon::fromTheme("edit-paste", pasteIcon));

    QIcon deleteIcon(":/img/edit-delete.png");
    mainWindowUi.deleteAction->setIcon(QIcon::fromTheme("edit-delete", deleteIcon));
}
