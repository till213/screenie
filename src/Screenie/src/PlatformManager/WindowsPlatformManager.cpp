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

#include <QAction>
#include <QKeySequence>
#include <QSysInfo>

#include "ui_MainWindow.h"
#include "WindowsPlatformManager.h"
#include "Windows8ProxyStyle.h"

class WindowsPlatformManagerPrivate
{
public:
    WindowsPlatformManagerPrivate(Ui::MainWindow &theMainWindow)
        : mainWindow(theMainWindow),
          windows8ProxyStyle(new Windows8ProxyStyle) // must be allocated on heap
    {}

    ~WindowsPlatformManagerPrivate()
    {
        // This is a bit tricky: first we need to restore the style
        // to the default one...
        mainWindow.menubar->setStyle(QApplication::style());
        // ... before we delete the proxy style, as calls are still
        // being made to QStyle during QWidget deletion later on
        delete windows8ProxyStyle;
    }

    Ui::MainWindow &mainWindow;
    Windows8ProxyStyle *windows8ProxyStyle;
};

// public

WindowsPlatformManager::WindowsPlatformManager()
    :d(nullptr)
{}

WindowsPlatformManager::~WindowsPlatformManager()
{
    if (d != nullptr) {
        delete d;
    }
}

void WindowsPlatformManager::initialise(QMainWindow &mainWindow, Ui::MainWindow &mainWindowUi)
{
    d = new WindowsPlatformManagerPrivate(mainWindowUi);
    AbstractPlatformManager::initialise(mainWindow, mainWindowUi);
    mainWindowUi.toggleFullScreenAction->setShortcut(QKeySequence(Qt::Key_F11));
}

// protected

void WindowsPlatformManager::initialisePlatformIcons(Ui::MainWindow &mainWindowUi)
{
    if (QSysInfo::windowsVersion() >= QSysInfo::WV_6_2) {
        // Windows 8 and above: Use larger menu icons and brighter background
        mainWindowUi.menubar->setStyle(d->windows8ProxyStyle);
        // Windows 10 menu background - padding: top right bottom left
        mainWindowUi.menubar->setStyleSheet("QMenu { background-color: rgb(251, 252, 253) } QMenu::item { padding: 6px 25px 6px 40px}");
    }

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

