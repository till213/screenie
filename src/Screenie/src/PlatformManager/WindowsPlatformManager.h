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

#ifndef WINDOWSPLATFORMMANAGER_H
#define WINDOWSPLATFORMMANAGER_H

class QMainWindow;

#include "AbstractPlatformManager.h"

class WindowsPlatformManagerPrivate;

namespace Ui {
    class MainWindow;
}

class WindowsPlatformManager : public AbstractPlatformManager
{
public:
    WindowsPlatformManager();
    virtual ~WindowsPlatformManager();

    virtual void initialize(QMainWindow &mainWindow, Ui::MainWindow &mainWindowUi);

protected:
    virtual void initializePlatformIcons(Ui::MainWindow &mainWindowUi);

private:
    WindowsPlatformManagerPrivate *d;
};

#endif // WINDOWSPLATFORMMANAGER_H
