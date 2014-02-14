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
#ifdef Q_OS_WIN
#include <QSettings>
#endif

#include "../../Utils/src/Version.h"
#include "ScreenieApplication.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(Resources);

    QCoreApplication::setOrganizationName("till-art.net");
    QCoreApplication::setOrganizationDomain("till-art.net");
    QCoreApplication::setApplicationName(Version::getApplicationName());
    QCoreApplication::setApplicationVersion(Version::getApplicationVersion());
#ifdef Q_OS_WIN
    // On Windows prefer INI format over Registry (= NativeFormat)
    QSettings::setDefaultFormat(QSettings::IniFormat);
#endif

    ScreenieApplication app(argc, argv);
    app.show();

    return app.exec();
}

