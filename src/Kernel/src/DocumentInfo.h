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

#include <QString>

class QMainWindow;

#include "KernelLib.h"

class ScreenieScene;
class SecurityToken;
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

    KERNEL_API DocumentInfo(QMainWindow &mainWindow, const ScreenieScene *screenieScene);
    KERNEL_API virtual ~DocumentInfo();

    KERNEL_API int getId() const;
    KERNEL_API void setId(int id);

    KERNEL_API QMainWindow &getMainWindow() const;

    KERNEL_API const ScreenieScene *getScreenieScene() const;
    KERNEL_API void setScreenieScene(const ScreenieScene *screenieScene);

    KERNEL_API QString getName() const;
    KERNEL_API QString getFileName() const;
    KERNEL_API void setFileName(const QString &fileName);

    KERNEL_API QString getFilePath() const;
    KERNEL_API void setFilePath(const QString &filePath);

    KERNEL_API SecurityToken *getSecurityToken() const;
    KERNEL_API void setSecurityToken(SecurityToken *securityToken);

    KERNEL_API SaveStrategy getSaveStrategy() const;
    KERNEL_API void setSaveStrategy(SaveStrategy SaveStrategy);

    KERNEL_API bool isModified() const;

private:
    Q_DISABLE_COPY(DocumentInfo)

    DocumentInfoPrivate *d;
};

#endif // DOCUMENTINFO_H
