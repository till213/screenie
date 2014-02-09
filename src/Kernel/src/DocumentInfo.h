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

#ifndef DOCUMENTINFO_H
#define DOCUMENTINFO_H

#include <QtCore/QString>

class QMainWindow;
class ScreenieScene;
class DocumentInfoPrivate;

/*!
 * \brief Stores info about the document, such as the name and file path.
 */
class DocumentInfo
{
public:
    /*!
     * Defines how to deal with unsaved documents before closing the window.
     */
    enum SaveStrategy
    {
        Discard, /*!< Discard the modifications, close the document immediatelly */
        Save, /*!< Save the modifications before closing the document */
        Ask /*!< Ask the user whether to discard or save the modifications before closing the document */
    };

    DocumentInfo(QMainWindow &mainWindow, const ScreenieScene *screenieScene);
    virtual ~DocumentInfo();

    int getId() const;
    void setId(int id);

    QMainWindow &getMainWindow() const;

    const ScreenieScene *getScreenieScene() const;
    void setScreenieScene(const ScreenieScene *screenieScene);

    QString getName() const;
    QString getFileName() const;
    void setFileName(const QString &fileName);

    QString getFilePath() const;
    void setFilePath(const QString &filePath);

    SaveStrategy getSaveStrategy() const;
    void setSaveStrategy(SaveStrategy SaveStrategy);

    bool isModified() const;

private:
    Q_DISABLE_COPY(DocumentInfo)

    DocumentInfoPrivate *d;
};

#endif // DOCUMENTINFO_H
