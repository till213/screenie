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

#include <QCoreApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#ifdef Q_OS_WIN
#include <QSettings>
#endif

#include "../../Model/src/ScreenieModelInterface.h"
#include "../../Utils/src/Version.h"
#include "ScreenieApplication.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(Resources);

    QCoreApplication::setOrganizationName("till-art.net");
    QCoreApplication::setOrganizationDomain("till-art.net");
    QCoreApplication::setApplicationName(Version::getApplicationName());
    QCoreApplication::setApplicationVersion(Version::getApplicationVersion());

    qRegisterMetaType<ScreenieModelInterface::ReflectionMode>();
#ifdef Q_OS_WIN
    // On Windows prefer INI format over Registry (= NativeFormat)
    QSettings::setDefaultFormat(QSettings::IniFormat);
#endif

    ScreenieApplication app(argc, argv);
#ifdef Q_OS_WIN
    QString appTranslationPath = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
#elif defined Q_OS_LINUX
    QString appTranslationPath = QCoreApplication::applicationDirPath() + "/translations";
#elif defined Q_OS_MAC
    QString appTranslationPath = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
#else
    QString appTranslationPath = QCoreApplication::applicationDirPath() + "/translations";
#endif

    QTranslator qtTranslator;
    qtTranslator.load(QLocale::system(), "qtbase", "_", QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    QTranslator appTranslator;
    appTranslator.load(QLocale::system(), "screenie", "_", appTranslationPath);
    app.installTranslator(&appTranslator);

#ifdef DEBUG
    qDebug("System LOCALE: %s, app translation path: %s, Qt translation path: %s", qPrintable(QLocale::system().name()), qPrintable(appTranslationPath), qPrintable(QLibraryInfo::location(QLibraryInfo::TranslationsPath)));
#endif

#ifdef Q_OS_MAC
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif
    app.show();

    return app.exec();
}

