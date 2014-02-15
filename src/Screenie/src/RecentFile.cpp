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

#include <QObject>
#include <QFileInfo>
#include <QStringList>

#include "../../Utils/src/FileUtils.h"
#include "../../Utils/src/Settings.h"
#include "RecentFile.h"

class RecentFilePrivate {
public:
    static const int DefaultMaxRecentFiles;
    static const int MaxRecentFiles;

    RecentFilePrivate()
    {}

    ~RecentFilePrivate()
    {}

    QSettings settings;
    QStringList recentFiles;
    int maxRecentFiles;

    static RecentFile *instance;
};

const int RecentFilePrivate::DefaultMaxRecentFiles = 8;
const int RecentFilePrivate::MaxRecentFiles = 10; // There are 10 action shortcuts, keys 0...9
RecentFile *RecentFilePrivate::instance = nullptr;

// public

RecentFile &RecentFile::getInstance()
{
    if (RecentFilePrivate::instance == nullptr) {
        RecentFilePrivate::instance = new RecentFile();
    }
    return *RecentFilePrivate::instance;
}

void RecentFile::destroyInstance()
{
    if (RecentFilePrivate::instance != nullptr) {
        delete RecentFilePrivate::instance;
        RecentFilePrivate::instance = nullptr;
    }
}

void RecentFile::addRecentFile(const QString &filePath)
{
    if (!d->recentFiles.contains(filePath)) {
        /*!\todo Add security scoped bookmark */
        // QByteArray bookmark = FileUtils::createFileAccessBookmark(fileInfo.filePath());
        d->recentFiles.prepend(filePath);
        if (d->recentFiles.count() > d->maxRecentFiles) {
            d->recentFiles.removeLast();
        }
        emit recentFilesChanged();
    }
}

void RecentFile::removeRecentFile(const QString &filePath)
{
    int index = d->recentFiles.indexOf(filePath);
    if (index != -1) {
        d->recentFiles.removeAt(index);
        emit recentFilesChanged();
    }
}

void RecentFile::moveToFront(const QString &filePath)
{
    if (d->recentFiles.contains(filePath)) {
        blockSignals(true);
        removeRecentFile(filePath);
        blockSignals(false);
        addRecentFile(filePath);
    }
}

void RecentFile::clear()
{
    if (d->recentFiles.count() > 0) {
        d->recentFiles.clear();
        emit recentFilesChanged();
    }
}

const QStringList &RecentFile::getRecentFiles() const
{
    return d->recentFiles;
}

int RecentFile::getMaxRecentFiles() const
{
    return d->maxRecentFiles;
}

void RecentFile::setMaxRecentFiles(int maxRecentFiles)
{
    if (d->maxRecentFiles != maxRecentFiles && maxRecentFiles >= 1 && maxRecentFiles <= RecentFilePrivate::MaxRecentFiles) {
        if (d->maxRecentFiles > maxRecentFiles) {
            while (d->recentFiles.count() > maxRecentFiles) {
                d->recentFiles.removeLast();
            }
        }
        d->maxRecentFiles = maxRecentFiles;
        emit maxRecentFilesChanged(d->maxRecentFiles);
    }
}

// protected

RecentFile::~RecentFile()
{
    store();
    delete d;
}

// private

RecentFile::RecentFile()
{
    d = new RecentFilePrivate();
    restore();
}

void RecentFile::store()
{
    d->settings.beginGroup("Paths");
    {
        d->settings.beginGroup("Recent");
        {
            d->settings.setValue("maxRecentFiles", d->maxRecentFiles);
            d->settings.setValue("RecentFile", d->recentFiles);
        }
        d->settings.endGroup();
    }
    d->settings.endGroup();
}

void RecentFile::restore()
{
    d->settings.beginGroup("Paths");
    {
        d->settings.beginGroup("Recent");
        {
            d->maxRecentFiles = d->settings.value("maxRecentFiles", RecentFilePrivate::DefaultMaxRecentFiles).toInt();
            d->recentFiles = d->settings.value("RecentFile", QStringList()).toStringList();
        }
        d->settings.endGroup();
    }
    d->settings.endGroup();
}

