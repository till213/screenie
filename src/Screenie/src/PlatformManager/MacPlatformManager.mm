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

#import <AppKit/NSView.h>
#import <AppKit/NSWindow.h>
#import <AvailabilityMacros.h>

#import <QSysInfo>
#import <QAction>
#import <QKeySequence>
#import <QIcon>
#import <QToolBar>
#import <QApplication>

#import "../../../Utils/src/Settings.h"
#import "ui_MainWindow.h"
#import "MacPlatformManager.h"

// public

void MacPlatformManager::initialise(QMainWindow &mainWindow, Ui::MainWindow &mainWindowUi)
{
    AbstractPlatformManager::initialise(mainWindow, mainWindowUi);
    mainWindow.installEventFilter(this);
    mainWindowUi.toggleFullScreenAction->setShortcut(QKeySequence(Qt::Key_F + Qt::CTRL + Qt::META));
    initialiseToolBar(mainWindowUi);
}

bool MacPlatformManager::eventFilter(QObject *object, QEvent *event)
{
    bool result;
    const QMainWindow *mainWindow = qobject_cast<const QMainWindow *>(object);
    if (mainWindow != nullptr) {
        switch (event->type()) {
        case QEvent::ActivationChange:
            handleWindowActivation(mainWindow->isActiveWindow());
            result = false;
            break;
        default:
            result = QObject::eventFilter(object, event);
            break;
        }
    } else {
        result = QObject::eventFilter(object, event);
    }
    return result;
}

bool MacPlatformManager::isFullScreen() const
{
    bool result;
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_7
    if (isFullScreenAPISupported()) {
        QMainWindow *mainWindow = getMainWindow();
        if (mainWindow != nullptr) {
            NSView *nsview = (__bridge NSView *)reinterpret_cast<void *>(mainWindow->winId());
            NSWindow *nswindow = [nsview window];
            NSUInteger masks = [nswindow styleMask];
            result = masks & NSFullScreenWindowMask;
        } else {
            result = false;
        }
    } else {
        result = AbstractPlatformManager::isFullScreen();
    }
#else
    result = AbstractPlatformManager::isFullScreen();
#endif

    return result;
}

bool MacPlatformManager::isFullScreenAPISupported()
{
    bool result;

    result = (QSysInfo::MacintoshVersion > QSysInfo::MV_SNOWLEOPARD);
    return result;
}

// protected

void MacPlatformManager::initialisePlatformIcons(Ui::MainWindow &mainWindowUi)
{
    Q_UNUSED(mainWindowUi);
}

// private

void MacPlatformManager::initialiseToolBar(Ui::MainWindow &mainWindowUi)
{
    mainWindowUi.toolBar->setMovable(false);
    getMainWindow()->setUnifiedTitleAndToolBarOnMac(true);
    // workaround for QTBUG-36700: sheet dialogs are rendered with a /fixed/ offset
    // of 50 pixels when calling setUnifiedTitleAndToolBarOnMac in Qt 5.2.1
    mainWindowUi.toolBar->setMinimumHeight(50);
}

void MacPlatformManager::handleWindowActivation(bool active)
{
    Ui::MainWindow *mainWindowUi = getMainWindowUi();
    if (mainWindowUi != nullptr) {
        if (active) {
            mainWindowUi->sidePanel->setStyleSheet("#sidePanel {background-color: rgb(218, 223, 230); border-right: 1px solid rgb(187, 187, 187);}");
        } else {
            mainWindowUi->sidePanel->setStyleSheet("#sidePanel {background-color: rgb(237, 237, 237); border-right: 1px solid rgb(187, 187, 187);}");
        }
    }
}
