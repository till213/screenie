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
#include <QString>
#include <QLibraryInfo>

#include "ui_MainWindow.h"
#include "WindowsPlatformManager.h"
#include "Windows10ProxyStyle.h"

class WindowsPlatformManagerPrivate
{
public:
    WindowsPlatformManagerPrivate(Ui::MainWindow &theMainWindow)
        : mainWindow(theMainWindow)
    {
        if (QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS10) {
            windows10ProxyStyle = new Windows10ProxyStyle;
        } else {
            windows10ProxyStyle = nullptr;
        }

    }

    ~WindowsPlatformManagerPrivate()
    {
        if (QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS10) {
            // This is a bit tricky: first we need to restore the style
            // to the default one...
            mainWindow.menubar->setStyle(QApplication::style());
            // ... before we delete the proxy style, as calls are still
            // being made to QStyle during QWidget deletion later on
            delete windows10ProxyStyle;
        }
    }

    Ui::MainWindow &mainWindow;
    Windows10ProxyStyle *windows10ProxyStyle;
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
    QIcon newIcon;
    QIcon openIcon;
    QIcon saveIcon;
    QIcon saveAsIcon;
    QIcon exitIcon;
    QIcon cutIcon;
    QIcon copyIcon;
    QIcon pasteIcon;
    QIcon deleteIcon;

    if (QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS10) {
        // Windows 8 and above: Use larger menu icons and brighter background
        mainWindowUi.menubar->setStyle(d->windows10ProxyStyle);
        // Windows 10 menu background - padding: top right bottom left
        mainWindowUi.menubar->setStyleSheet("QMenu { background-color: rgb(251, 252, 253) } QMenu::item { padding: 6px 25px 6px 40px}");

        // Icons
        newIcon.addFile(":/10/img/document-new.png");
        openIcon.addFile(":/10/img/document-open.png");
        saveIcon.addFile(":/10/img/document-save.png");
        saveAsIcon.addFile(":/10/img/document-save-as.png");
        exitIcon.addFile(":/10/img/application-exit.png");
        cutIcon.addFile(":/10/img/edit-cut.png");
        copyIcon.addFile(":/10/img/edit-copy.png");
        pasteIcon.addFile(":/10/img/edit-paste.png");
        deleteIcon.addFile(":/10/img/edit-delete.png");
    } else {
        // Previous Windows versions
        newIcon.addFile(":/XP/img/document-new.png");
        openIcon.addFile(":/XP/img/document-open.png");
        saveIcon.addFile(":/XP/img/document-save.png");
        saveAsIcon.addFile(":/XP/img/document-save-as.png");
        exitIcon.addFile(":/XP/img/application-exit.png");
        cutIcon.addFile(":/XP/img/edit-cut.png");
        copyIcon.addFile(":/XP/img/edit-copy.png");
        pasteIcon.addFile(":/XP/img/edit-paste.png");
        deleteIcon.addFile(":/XP/img/edit-delete.png");
    }

    // File menu

    mainWindowUi.newAction->setIcon(QIcon::fromTheme("document-new", newIcon));
    mainWindowUi.openAction->setIcon(QIcon::fromTheme("document-open", openIcon));
    mainWindowUi.saveAction->setIcon(QIcon::fromTheme("document-save", saveIcon));
    mainWindowUi.saveAsAction->setIcon(QIcon::fromTheme("document-save-as", saveAsIcon));
    mainWindowUi.quitAction->setIcon(QIcon::fromTheme("application-exit", exitIcon));

    // Edit menu

    mainWindowUi.cutAction->setIcon(QIcon::fromTheme("edit-cut", cutIcon));
    mainWindowUi.copyAction->setIcon(QIcon::fromTheme("edit-copy", copyIcon));
    mainWindowUi.pasteAction->setIcon(QIcon::fromTheme("edit-paste", pasteIcon));
    mainWindowUi.deleteAction->setIcon(QIcon::fromTheme("edit-delete", deleteIcon));
}

