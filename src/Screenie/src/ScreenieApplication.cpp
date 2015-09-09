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


// @todo KNO REMOVE ME
#include <QMessageBox>

#include "../../Utils/src/Settings.h"
#include "../../Utils/src/RecentFile.h"
#include "../../Kernel/src/DocumentManager.h"
#include "PlatformManager/PlatformManagerFactory.h"
#include "PlatformManager/PlatformManager.h"
#include "MainWindow.h"
#include "ScreenieApplication.h"

// public

ScreenieApplication::ScreenieApplication(int &argc, char **argv)
    : QApplication(argc, argv)
{
    frenchConnection();
    initialiseTranslations();
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
#ifdef DEBUG
        qDebug("ScreenieApplication::event: %s", qPrintable(static_cast<QFileOpenEvent *>(event)->file()));
#endif
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

void ScreenieApplication::initialiseTranslations()
{
    QString qtTranslationPath = PlatformManager::getTranslationsPath(PlatformManager::Translation::Qt);
    QString appTranslationPath = PlatformManager::getTranslationsPath(PlatformManager::Translation::Application);

    QLocale locale = Settings::getInstance().getLocale();
    m_qtTranslator.load(locale, "qtbase", "_", qtTranslationPath);
    QCoreApplication::instance()->installTranslator(&m_qtTranslator);

    bool loadSuccess = m_appTranslator.load(locale, "screenie", "_", appTranslationPath);
    bool installSuccess = QCoreApplication::instance()->installTranslator(&m_appTranslator);
    QMessageBox::information(nullptr, "Debug", QString("Load %1 Install %2 Path: %3 Locale %4")
                             .arg(loadSuccess).arg(installSuccess).arg(appTranslationPath).arg(locale.name()));
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
