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

#include <QtCore/QObject>
#include <QtWidgets/QAction>
#include <QtGui/QKeySequence>
#include <QtWidgets/QShortcut>
#include <QtWidgets/QMainWindow>

#include "ui_MainWindow.h"
#include "AbstractPlatformManager.h"

class AbstractPlatformManagerPrivate
{
public:
    AbstractPlatformManagerPrivate(QMainWindow &theMainWindow, Ui::MainWindow &theMainWindowUi)
        : mainWindow(theMainWindow),
          mainWindowUi(theMainWindowUi)
    {}

    QMainWindow &mainWindow;
    Ui::MainWindow &mainWindowUi;
};

// public

AbstractPlatformManager::AbstractPlatformManager()
{
    d = 0;
}

AbstractPlatformManager::~AbstractPlatformManager()
{
    cleanUp();
}

void AbstractPlatformManager::initialise(QMainWindow &mainWindow, Ui::MainWindow &mainWindowUi)
{
    cleanUp();

    d = new AbstractPlatformManagerPrivate(mainWindow, mainWindowUi);

    // File
    mainWindowUi.newAction->setShortcut(QKeySequence::New);
    mainWindowUi.openAction->setShortcut(QKeySequence::Open);
    mainWindowUi.saveAction->setShortcut(QKeySequence::Save);
    mainWindowUi.saveAsAction->setShortcut(QKeySequence::SaveAs);
    mainWindowUi.exportAction->setShortcut(QKeySequence(Qt::Key_E + Qt::CTRL));
    mainWindowUi.closeAction->setShortcut(QKeySequence::Close);
    mainWindowUi.quitAction->setShortcut(QKeySequence::Quit);
    // Edit
    mainWindowUi.cutAction->setShortcut(QKeySequence::Cut);
    mainWindowUi.copyAction->setShortcut(QKeySequence::Copy);
    mainWindowUi.pasteAction->setShortcut(QKeySequence::Paste);
    mainWindowUi.deleteAction->setShortcut(QKeySequence::Delete);
    mainWindowUi.selectAllAction->setShortcut(QKeySequence::SelectAll);
    // Insert
    mainWindowUi.addImageAction->setShortcut(QKeySequence(Qt::Key_I + Qt::CTRL));
    mainWindowUi.addTemplateAction->setShortcut(QKeySequence(Qt::Key_T + Qt::CTRL));
    // Generic
    QShortcut *shortcut = new QShortcut(QKeySequence("Backspace"), &mainWindow);
    QObject::connect(shortcut, SIGNAL(activated()),
                     mainWindowUi.deleteAction, SIGNAL(triggered()));

    intializeIcons(mainWindowUi);
}

bool AbstractPlatformManager::isFullScreen() const
{
    bool result;
    if (d != nullptr) {
        result = d->mainWindow.isFullScreen();
    } else {
        result = false;
#ifdef DEBUG
        qWarning("AbstractPlatformManager::isFullScreen: Private data not initialised! Call initialise() first.");
#endif
    }
#ifdef DEBUG
        qWarning("AbstractPlatformManager::isFullScreen: %d", result);
#endif
    return result;
}

// protected

QMainWindow *AbstractPlatformManager::getMainWindow() const
{
    QMainWindow *result;
    if (d != nullptr) {
        result = &d->mainWindow;
    } else {
        result = 0;
#ifdef DEBUG
        qWarning("AbstractPlatformManager::getMainWindow: Private data not initialised! Call initialise() first.");
#endif
    }
    return result;
}

Ui::MainWindow *AbstractPlatformManager::getMainWindowUi()
{
    Ui::MainWindow *result;
    if (d != nullptr) {
        result = &d->mainWindowUi;
    } else {
        result = 0;
#ifdef DEBUG
        qWarning("AbstractPlatformManager::getMainWindowUi: Private data not initialised! Call initialise() first.");
#endif
    }
    return result;
}

// private

void AbstractPlatformManager::intializeIcons(Ui::MainWindow &mainWindowUi)
{
    QIcon addImageIcon(":/img/insert-image.png");
    mainWindowUi.addImageAction->setIcon(addImageIcon);
    QIcon addTemplateIcon(":/img/insert-template.png");
    mainWindowUi.addTemplateAction->setIcon(addTemplateIcon);

    initialisePlatformIcons(mainWindowUi);
}

void AbstractPlatformManager::cleanUp()
{
    if (d != nullptr) {
        delete d;
        d = nullptr;
    }
}
