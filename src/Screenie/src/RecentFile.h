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

#ifndef RECENTFILE_H
#define RECENTFILE_H

#include <QObject>

class QString;
class QActionGroup;
class QStringList;

class RecentFilePrivate;

/*!
 * \brief The recently opened files.
 *
 * Keeps track of recently opened files.
 */
class RecentFile : public QObject
{
    Q_OBJECT

public:
    static RecentFile &getInstance();
    static void destroyInstance();

    /*!
     * \sa #recentFilesChanged()
     */
    void addRecentFile(const QString &filePath);

    /*!
     * \sa #recentFilesChanged()
     */
    void removeRecentFile(const QString &filePath);

    /*!
     * Moves the \em existing \c filePath to the most recent file position.
     * The \c filePath is not added in case it does not already exist in the
     * recent files list.
     *
     * \sa #recentFilesChanged()
     */
    void moveToFront(const QString &filePath);

    const QStringList &getRecentFiles() const;

    /*!
     * Clears the list of recent files.
     *
     * \sa #recentFilesChanged()
     */
    void clear();

    int getMaxRecentFiles() const;

    /*!
     * Sets the maximum menu entries of recent files which must be in [1, 10].
     *
     * \sa #maxRecentFilesChanged(int)
     */
    void setMaxRecentFiles(int maxRecentFile);

signals:
    /*!
     * Emitted whenever the recent file list has changed. Existing QActionGroup
     * entries must be set in-/visible.
     */
    void recentFilesChanged();

    /*!
     * Emitted whenever the maximum number of recent files has changed. In this
     * cases new QAction entries must be either created or existing ones must
     * be deleted. In any case the application menu must be updated accordingly.
     *
     * \p maxRecentFiles
     *    the new number of maximum recent files
     */
    void maxRecentFilesChanged(int maxRecentFiles);

protected:
    virtual ~RecentFile();

private:
    Q_DISABLE_COPY(RecentFile)
    RecentFilePrivate *d;

    RecentFile();

    void initialise();
    void frenchConnection();

    void store();
    void restore();
};

#endif // RECENTFILE_H
