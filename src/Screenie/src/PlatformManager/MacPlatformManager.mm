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

#import <NSView.h>
#import <NSWindow.h>

#import <QtGui/QAction>
#import <QtGui/QKeySequence>
#import <QtGui/QIcon>
#import <QtGui/QApplication>

#import "ui_MainWindow.h"
#import "MacPlatformManager.h"

// public

void MacPlatformManager::initialize(QMainWindow &mainWindow, Ui::MainWindow &mainWindowUi)
{
    AbstractPlatformManager::initialize(mainWindow, mainWindowUi);
    mainWindow.installEventFilter(this);
    mainWindowUi.toggleFullScreenAction->setShortcut(QKeySequence(Qt::Key_F + Qt::CTRL + Qt::META));

    // OS X 10.7 "Lion" fullscreen support
    NSView *nsview = (NSView *) mainWindow.winId();
    NSWindow *nswindow = [nsview window];
    [nswindow setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
}

bool MacPlatformManager::eventFilter(QObject *object, QEvent *event)
{
    bool result;
    const QMainWindow *mainWindow = qobject_cast<const QMainWindow *>(object);
    if (mainWindow != 0) {
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

void MacPlatformManager::showFullScreen()
{
    if (!isFullScreen()) {
        toggleFullScreen();
    }

}

void MacPlatformManager::showNormal()
{
    if (isFullScreen()) {
        toggleFullScreen();
    }
}

bool MacPlatformManager::isFullScreen() const
{
    bool result;
    QMainWindow *mainWindow = getMainWindow();
    if (mainWindow != 0) {
        NSView *nsview = (NSView *) mainWindow->winId();
        NSWindow *nswindow = [nsview window];
        NSUInteger masks = [nswindow styleMask];
        result = masks & NSFullScreenWindowMask;
    } else {
        result = false;
    }
    return result;
}

// protected

void MacPlatformManager::initializePlatformIcons(Ui::MainWindow &mainWindowUi)
{
    Q_UNUSED(mainWindowUi);
}

// private

void MacPlatformManager::handleWindowActivation(bool active)
{
    Ui::MainWindow *mainWindowUi = getMainWindowUi();
    if (mainWindowUi != 0) {
        if (active) {
            mainWindowUi->sidePanel->setStyleSheet("#sidePanel {background-color: rgb(218, 223, 230); border-right: 1px solid rgb(187, 187, 187);}");
        } else {
            mainWindowUi->sidePanel->setStyleSheet("#sidePanel {background-color: rgb(237, 237, 237); border-right: 1px solid rgb(187, 187, 187);}");
        }
    }
}

void MacPlatformManager::toggleFullScreen()
{
    QMainWindow *mainWindow = getMainWindow();
    if (mainWindow != 0) {
        NSView *nsview = (NSView *) mainWindow->winId();
        NSWindow *nswindow = [nsview window];
        [nswindow toggleFullScreen:nil];
    }
}