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

#ifndef MACPLATFORMMANAGER_H
#define MACPLATFORMMANAGER_H

#include <QObject>
#include <QByteArray>

class QObject;
class QMainWindow;
class QEvent;
class QString;

#include "AbstractPlatformManager.h"

namespace Ui {
    class MainWindow;
}

class MacPlatformManager : public QObject, public AbstractPlatformManager
{
    Q_OBJECT
public:
    virtual bool eventFilter(QObject *object, QEvent *event) override;

    // Interface PlatformManager
    virtual void initialise(QMainWindow &mainWindow, Ui::MainWindow &mainWindowUi) override;

protected:
    virtual void initialisePlatformIcons(Ui::MainWindow &mainWindowUi);

private:
    void initialiseToolBar(Ui::MainWindow &mainWindowUi);
    void handleWindowActivation(bool active);
};

#endif // MACPLATFORMMANAGER_H
