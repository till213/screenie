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
#import <AvailabilityMacros.h>

#import <QtCore/QSysInfo>
#import <QtGui/QAction>
#import <QtGui/QKeySequence>
#import <QtGui/QIcon>
#import <QtGui/QApplication>

#import "../../../Utils/src/Settings.h"
#import "ui_MainWindow.h"
#import "MacPlatformManager.h"

// public

void MacPlatformManager::initialize(QMainWindow &mainWindow, Ui::MainWindow &mainWindowUi)
{
    AbstractPlatformManager::initialize(mainWindow, mainWindowUi);
    mainWindow.installEventFilter(this);
    mainWindowUi.toggleFullScreenAction->setShortcut(QKeySequence(Qt::Key_F + Qt::CTRL + Qt::META));

    // OS X 10.7 "Lion" fullscreen support
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_7
    NSView *nsview = (NSView *) mainWindow.winId();
    NSWindow *nswindow = [nsview window];
    [nswindow setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
#endif
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
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_7
    if (isFullScreenAPISupported()) {
        if (!isFullScreen()) {
            toggleFullScreen();
        }
    } else {
        AbstractPlatformManager::showFullScreen();
    }
#else
    AbstractPlatformManager::showFullScreen();
#endif
    QMainWindow *mainWindow = getMainWindow();
    if (mainWindow != 0) {
        // re-initialising the toolbar to be unified helps in Qt toolbar quirks
        mainWindow->setUnifiedTitleAndToolBarOnMac(Settings::getInstance().isToolBarVisible());
    }
}

void MacPlatformManager::showNormal()
{
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_7
    if (isFullScreenAPISupported()) {
        if (isFullScreen()) {
            toggleFullScreen();
        }
    } else {
        AbstractPlatformManager::showNormal();
    }
#else
    AbstractPlatformManager::showNormal();
#endif
    QMainWindow *mainWindow = getMainWindow();
    if (mainWindow != 0) {
        // re-initialising the toolbar to be unified helps in Qt toolbar quirks
        mainWindow->setUnifiedTitleAndToolBarOnMac(Settings::getInstance().isToolBarVisible());
    }
}

bool MacPlatformManager::isFullScreen() const
{
    bool result;
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_7
    if (isFullScreenAPISupported()) {
        QMainWindow *mainWindow = getMainWindow();
        if (mainWindow != 0) {
            NSView *nsview = (NSView *) mainWindow->winId();
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
    /*!\todo Qt 4.8 supports "Lion" */
//    result = (QSysInfo::MacVersion() > QSysInfo::MV_SNOWLEOPARD);
//    qDebug("MacPlatformManager::isFullScreenAPISupported: %d", result);
//    return result;
    /*!\todo Use http://doc.qt.nokia.com/4.7/qsysinfo.html#MacVersion-enum instead! */
    NSWindow *nswin = [[NSWindow alloc] init];
    result = (YES == [nswin respondsToSelector: @selector(toggleFullScreen:)]);
    [nswin release];
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

#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_7
void MacPlatformManager::toggleFullScreen()
{
    QMainWindow *mainWindow = getMainWindow();
    if (mainWindow != 0) {
        NSView *nsview = (NSView *) mainWindow->winId();
        NSWindow *nswindow = [nsview window];
        [nswindow toggleFullScreen:nil];
    }
}
#endif
