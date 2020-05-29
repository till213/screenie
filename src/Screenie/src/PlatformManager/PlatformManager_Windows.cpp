/* This file is part of the Screenie project.
   Screenie is a fancy screenshot composer.

   Copyright (C) 2015 Oliver Knoll <till.oliver.knoll@gmail.com>

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
#include <QString>
#include <QLibraryInfo>

#include "PlatformManager.h"

// Public

QString PlatformManager::getTranslationsPath(Translation translation)
{
    QString translationPath;

    switch(translation) {
    case Translation::Qt:
        translationPath = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
        break;
    case Translation::Application:
        translationPath = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
        break;
    default:
#ifdef DEBUG
        qDebug("PlatformManager::getTranslationsPath: Unsupported Translation Path option: %d", translation);
#endif
        break;
    }

    return translationPath;
}
