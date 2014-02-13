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

#include <QTranslator>
#include <QString>
#include <QLocale>
#include <QStringList>
#include <QLibraryInfo>
#include <QIcon>
#include <QEvent>
#include <QFileOpenEvent>

#include "../../Utils/src/Settings.h"
#include "../../Kernel/src/DocumentManager.h"
#include "PlatformManager/PlatformManagerFactory.h"
#include "MainWindow.h"
#include "RecentFile.h"
#include "ScreenieApplication.h"

// public

ScreenieApplication::ScreenieApplication(int &argc, char **argv)
    : QApplication(argc, argv)
{
    frenchConnection();
}

void ScreenieApplication::show()
{
#ifdef Q_OS_MAC
    // Mac apps prefer not to have icons in menus
    QCoreApplication::setAttribute(Qt::AA_DontShowIconsInMenus);
#endif

    m_mainWindow = new MainWindow();
    m_mainWindow->setAttribute(Qt::WA_DeleteOnClose, true);
    // simplistic command line parsing: first arg is assumed to be a file path
    QStringList args = arguments();
    if (args.count() > 1) {
        m_mainWindow->read(args.at(1));
    }
    m_mainWindow->show();
}

// protected

bool ScreenieApplication::event(QEvent *event)
{
    bool result;
    switch (event->type()) {
    case QEvent::FileOpen:
        result = true;
        qDebug("ScreenieApplication::event: %s", qPrintable(static_cast<QFileOpenEvent *>(event)->file()));
        m_mainWindow->read(static_cast<QFileOpenEvent *>(event)->file());
        event->accept();
        break;
    default:
        result = QApplication::event(event);
    }
    return result;
}

// private

void ScreenieApplication::frenchConnection()
{
    connect(QApplication::instance(), SIGNAL(lastWindowClosed()),
            this, SLOT(handleLastWindowClosed()));
}

// private slots

void ScreenieApplication::handleLastWindowClosed()
{
    // destroy singletons

    // RecentFile stores its settings using the Settings instance, hence it
    // must be destroyed before Settings
    RecentFile::destroyInstance();
    Settings::destroyInstance();
    DocumentManager::destroyInstance();
    PlatformManagerFactory::destroyInstance();
}
