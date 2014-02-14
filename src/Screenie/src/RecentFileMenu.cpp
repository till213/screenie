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

#include <QActionGroup>
#include <QAction>
#include <QKeySequence>
#include <QFileInfo>
#include <QVector>

#include "RecentFile.h"
#include "RecentFileMenu.h"

namespace
{
    const QVector<Qt::Key> ActionKeys = {Qt::Key_1, Qt::Key_2, Qt::Key_3, Qt::Key_4, Qt::Key_5, Qt::Key_6, Qt::Key_7, Qt::Key_8, Qt::Key_9, Qt::Key_0};
}

class RecentFileMenuPrivate
{
public:
    RecentFileMenuPrivate(QObject *parent)
        : recentFileActionGroup(new QActionGroup(parent))
    {}

    ~RecentFileMenuPrivate()
    {}

    QActionGroup *recentFileActionGroup;
    QAction *clearRecentFileAction;
};

// public

RecentFileMenu::RecentFileMenu(QObject *parent) :
    QObject(parent)
{
    d = new RecentFileMenuPrivate(parent);
    initialise();
    frenchConnections();
}

RecentFileMenu::~RecentFileMenu()
{
    delete d;
}

QActionGroup &RecentFileMenu::getRecentFileActionGroup() const
{
    return *d->recentFileActionGroup;
}

// private

void RecentFileMenu::initialise()
{
    int maxRecentFiles = RecentFile::getInstance().getMaxRecentFiles();
    updateNofRecentFileActions(maxRecentFiles);
    updateRecentFileActions();

    QAction *separator = new QAction(d->recentFileActionGroup);
    separator->setSeparator(true);
    d->clearRecentFileAction = new QAction(tr("Clear &menu", "Clear the recent files menu"), d->recentFileActionGroup);
}

void RecentFileMenu::frenchConnections()
{
    RecentFile &recentFile = RecentFile::getInstance();
    connect(&recentFile, SIGNAL(recentFilesChanged()),
            this, SLOT(updateRecentFileActions()));
    connect(&recentFile, SIGNAL(maxRecentFilesChanged(int)),
            this, SLOT(updateNofRecentFileActions(int)));
    connect(d->clearRecentFileAction, SIGNAL(triggered()),
            this, SLOT(clearRecentFileMenu()));
}

// private slots

void RecentFileMenu::updateRecentFileActions()
{
    int n;
    int nofRecentFiles;
    int maxRecentFiles;
    RecentFile &recentFile = RecentFile::getInstance();
    const QStringList &recentFiles = recentFile.getRecentFiles();
    const QList<QAction *> recentFileActions = d->recentFileActionGroup->actions();

    nofRecentFiles = recentFiles.count();
    n = qMin(nofRecentFiles, ActionKeys.length());
    for (int i = 0; i < n; ++i) {
        const QFileInfo fileInfo(recentFiles[i]);
        const QString text = fileInfo.fileName();

        recentFileActions[i]->setText(text);
        recentFileActions[i]->setData(recentFiles[i]);
        recentFileActions[i]->setVisible(true);
        recentFileActions[i]->setShortcut(QKeySequence(ActionKeys[i] | Qt::CTRL));
    }

    // set all not yet used actions to invisible
    maxRecentFiles = recentFile.getMaxRecentFiles();
    n = qMin(maxRecentFiles, ActionKeys.length());
    for (int i = nofRecentFiles; i < n; ++i) {
        recentFileActions[i]->setVisible(false);
    }
}

void RecentFileMenu::updateNofRecentFileActions(int maxRecentFiles)
{
    int max;
    int nofRecentFilesActions;
    QList<QAction *> recentFileActions = d->recentFileActionGroup->actions();
    QAction *action;
    bool changed;

    changed = false;
    max = qMin(maxRecentFiles, ActionKeys.length());
    nofRecentFilesActions = recentFileActions.count() - 2; // two extra actions: separator and "clear" entry

    // add menu entries
    for (int i = nofRecentFilesActions; i < max; ++i) {
        action = new QAction(d->recentFileActionGroup);
        action->setVisible(false);
        connect(action, SIGNAL(triggered()),
                this, SLOT(handleRecentFileAction()));
        changed = true;
    }
#ifdef DEBUG
    qDebug("RecentFileMenu::updateNofRecentFileActions: created %d actions (max: %d nofRecentFilesActions: %d)", max - nofRecentFilesActions, max, nofRecentFilesActions);
#endif

    // remove menu entries - index starts at 0 (-> -1)
    for (int i = nofRecentFilesActions - 1; i >= max; --i) {
        delete recentFileActions.at(i);
        recentFileActions.removeAt(i);
        changed = true;
    }

    if (changed) {
        emit actionGroupChanged();
    }
}

void RecentFileMenu::handleRecentFileAction()
{
    RecentFile &recentFile = RecentFile::getInstance();

    if (const QAction *action = qobject_cast<const QAction *>(sender())) {
        QString filePath = action->data().toString();
        recentFile.moveToFront(filePath);
        emit openRecentFile(filePath);
    }
}

void RecentFileMenu::clearRecentFileMenu()
{
    RecentFile::getInstance().clear();
}

