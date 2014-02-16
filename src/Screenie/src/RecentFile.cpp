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
#include <QList>

#include "../../Utils/src/SecurityToken.h"
#include "../../Utils/src/Settings.h"
#include "RecentFile.h"

namespace
{
    const int InvalidIndex = -1;
}

class RecentFilePrivate {
public:
    static const int DefaultMaxRecentFiles;
    static const int MaxRecentFiles;

    RecentFilePrivate()
    {}

    ~RecentFilePrivate()
    {}

    QSettings settings;
    QStringList recentFiles;             // the items in recentFiles must always be in sync
    QList<QByteArray> securityTokenData; // with the items in securityTokenData
    int maxRecentFiles;

    static RecentFile *instance;
};

const int RecentFilePrivate::DefaultMaxRecentFiles = 8;
const int RecentFilePrivate::MaxRecentFiles = 10; // There are 10 action shortcuts keys: 0...9
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
    bool added = prependToRecentFiles(filePath);
    if (added) {
        QByteArray securityTokenData = SecurityToken::createSecurityTokenData(filePath);
        d->securityTokenData.prepend(securityTokenData);
        if (d->recentFiles.count() > d->maxRecentFiles) {
            d->recentFiles.removeLast();
            d->securityTokenData.removeLast();
        }
        emit recentFilesChanged();
    }
}

void RecentFile::removeRecentFile(const QString &filePath)
{
    int index = removeFromRecentFiles(filePath);
    if (index != ::InvalidIndex) {
        d->securityTokenData.removeAt(index);
        emit recentFilesChanged();
    }
}

void RecentFile::moveToFront(const QString &filePath)
{
    int index = removeFromRecentFiles(filePath);
    QByteArray securityTokenData;
    if (index != ::InvalidIndex) {
        // helper functions do not emit signals...
        prependToRecentFiles(filePath);
        // ... nor modify the security token data...
        securityTokenData = d->securityTokenData.at(index);

        // ... so we have to move the token data separately
        d->securityTokenData.removeAt(index);
        d->securityTokenData.prepend(securityTokenData);
    }
}

void RecentFile::clear()
{
    if (d->recentFiles.count() > 0) {
        d->recentFiles.clear();
        d->securityTokenData.clear();
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
                d->securityTokenData.removeLast();
            }
        }
        d->maxRecentFiles = maxRecentFiles;
        emit maxRecentFilesChanged(d->maxRecentFiles);
    }
}

void RecentFile::selectRecentFile(const QString &filePath)
{
    QByteArray securityTokenData;
    moveToFront(filePath);
    securityTokenData = d->securityTokenData.first();
    SecurityToken *securityToken = SecurityToken::create(securityTokenData);
    emit recentFileSelected(filePath, securityToken);
    securityToken->release();
    securityToken = nullptr;
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
    int count;
    d->settings.beginGroup("Paths");
    {
        d->settings.beginGroup("Recent");
        {
            d->settings.setValue("maxRecentFiles", d->maxRecentFiles);
            d->settings.setValue("recentFiles", d->recentFiles);
            count = d->securityTokenData.count();
            d->settings.beginWriteArray("security", count);
            {
                for (int i = 0; i < count; ++i) {
                    d->settings.setArrayIndex(i);
                    d->settings.setValue("data", d->securityTokenData.at(i));
                }
            }
            d->settings.endArray();
        }
        d->settings.endGroup();
    }
    d->settings.endGroup();
}

void RecentFile::restore()
{
    int count;
    QByteArray data;

    d->settings.beginGroup("Paths");
    {
        d->settings.beginGroup("Recent");
        {
            d->maxRecentFiles = d->settings.value("maxRecentFiles", RecentFilePrivate::DefaultMaxRecentFiles).toInt();
            d->recentFiles = d->settings.value("recentFiles", QStringList()).toStringList();
            count = d->settings.beginReadArray("security");
            {
                for (int i = 0; i < count; ++i) {
                    d->settings.setArrayIndex(i);
                    data = d->settings.value(("data")).toByteArray();
                    d->securityTokenData.append(data);
                }
            }
            d->settings.endArray();
        }
        d->settings.endGroup();
    }
    d->settings.endGroup();
}

bool RecentFile::prependToRecentFiles(const QString &filePath)
{
    bool result;
    if (!d->recentFiles.contains(filePath)) {
        d->recentFiles.prepend(filePath);
        if (d->recentFiles.count() > d->maxRecentFiles) {
            d->recentFiles.removeLast();
            d->securityTokenData.removeLast();
        }
        result = true;
    } else {
        result = false;
    }
    return result;
}

int RecentFile::removeFromRecentFiles(const QString &filePath)
{
    int index = d->recentFiles.indexOf(filePath);
    if (index != ::InvalidIndex) {
        d->recentFiles.removeAt(index);
    }
    return index;
}

