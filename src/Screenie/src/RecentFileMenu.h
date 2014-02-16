/* This file is part of the Screenie project.
   Screenie is a fancy screenshot composer.

   Copyright (C) 2014 Oliver Knoll <till.oliver.knoll@gmail.com>

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

#ifndef RECENTFILEMENU_H
#define RECENTFILEMENU_H

#include <QObject>
#include <QActionGroup>

class RecentFileMenuPrivate;

/*!
 * \brief Provides QAction menu entries for a recent files menu.
 *
 * The QActions are updated by connecting to the corresponding changes
 * in the RecentFile instance.
 */
class RecentFileMenu : public QObject
{
    Q_OBJECT
public:
    explicit RecentFileMenu(QObject *parent = 0);
    virtual ~RecentFileMenu();

    QActionGroup &getRecentFileActionGroup() const;

signals:
    /*!
     * Emitted whenever the QActionGroup has changed, that is when QAction items
     * have been added or removed.
     */
    void actionGroupChanged();

private:
    Q_DISABLE_COPY(RecentFileMenu)

    RecentFileMenuPrivate *d;

    void initialise();
    void frenchConnections();

private slots:
    void updateRecentFileActions();
    void updateNofRecentFileActions(int maxRecentFiles);
    void handleRecentFileAction();
    void clearRecentFileMenu();
};

#endif // RECENTFILEMENU_H
