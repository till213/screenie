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

#ifndef PLATFORMMANAGER_H
#define PLATFORMMANAGER_H

#include <QString>

class QMainWindow;
class QString;

namespace Ui {
    class MainWindow;
}

/*!
 * The PlatformManager provides platform-specific settings. Among them
 * are icons, stylesheets, shortcuts and translation paths.
 */
class PlatformManager
{
public:
    enum Translation {
        Qt,
        Application
    };

    virtual ~PlatformManager() {}

    virtual void initialise(QMainWindow &mainWindow, Ui::MainWindow &mainWindowUi) = 0;
    static QString getTranslationsPath(Translation translation);
};

#endif // PLATFORMMANAGER_H
