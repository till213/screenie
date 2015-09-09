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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QPoint>
#include <QSize>
#include <QRect>
#include <QSettings>
#include <QLocale>

#include "UtilsLib.h"

class SettingsPrivate;

/*!
 * The application settings. These settings are persisted to user configuration
 * files.
 *
 * Implementation note:
 *
 * - \b Windows: INI format
 * - \b Mac: Native format (CFPreferences API)
 * - \b Linux: INI format
 */
class Settings : public QObject
{
    Q_OBJECT
public:
    struct WindowGeometry {
        bool fullScreen;
        QRect position;
    };

    /*!
     * The render quality during edit operations.
     */
    enum class EditRenderQuality {
        Low = 0, /*!< Low quality: no anti-aliasing for pixmaps and fonts */
        Medium = 1, /*!< Not implemented yet. */
        High = 2, /*!< Not implemented yet. */
        Maximum = 3 /*!< Maximum quality: anti-aliasing for pixmaps and fonts */
    };

    UTILS_API static Settings &getInstance();
    UTILS_API static void destroyInstance();

    /*!
     * Returns the maximum image size in pixel. The size depends on the maximum available
     * resolution which defines the ratio between pixel and points (e.g. "Retina" displays
     * typically have a ratio of 2.0).
     *
     * \return the QSize which contains the maximum image size in pixel
     * \sa #getMaximumImagePointSize
     */
    UTILS_API QSize getMaximumImagePixelSize() const;

    /*!
     * Returns the maximum image size in points. The size in points is not dependent
     * on the pixel density of the display device.
     *
     * \return the QSize which contains the maximum image size in point
     * \sa #getMaximumImagePixelSize
     */
    UTILS_API const QSize &getMaximumImagePointSize() const;

    /*!
     * \sa #changed()
     */
    UTILS_API void setMaximumImagePointSize(const QSize &maximumImagePointSize);

    UTILS_API const QSize &getTemplateSize() const;

    /*!
     * \sa #changed()
     */
    UTILS_API void setTemplateSize(const QSize &templateSize);

    UTILS_API const QString &getLastImageDirectoryPath() const;

    /*!
     * \sa #changed()
     */
    UTILS_API void setLastImageDirectoryPath(const QString &lastImageDirectoryPath);

    UTILS_API const QString &getLastExportDirectoryPath() const;

    /*!
     * \sa #changed()
     */
    UTILS_API void setLastExportDirectoryPath(const QString &lastExportDirectoryPath);

    UTILS_API const QString &getLastDocumentDirectoryPath() const;

    /*!
     * \sa #changed()
     */
    UTILS_API void setLastDocumentDirectoryPath(const QString &lastDocumentDirectoryPath);

    UTILS_API qreal getRotationGestureSensitivity() const;

    /*!
     * \sa #changed()
     */
    UTILS_API void setRotationGestureSensitivity(qreal rotationGestureSensitivity);

    UTILS_API qreal getDistanceGestureSensitivity() const;

    /*!
     * \sa #changed()
     */
    UTILS_API void setDistanceGestureSensitivity(qreal distanceGestureSensitivity);

    /*!
     * \sa #changed()
     */
    UTILS_API void setMaxRecentFiles(int maxRecentFiles);

    UTILS_API EditRenderQuality getEditRenderQuality() const;

    /*!
     * \sa #changed()
     */
    UTILS_API void setEditRenderQuality(EditRenderQuality editRenderQuality);

    UTILS_API bool isToolBarVisible() const;

    /*!
     * \sa #changed()
     */
    UTILS_API void setToolBarVisible(bool enable);

    UTILS_API bool isSidePanelVisible() const;

    /*!
     * \sa #changed()
     */
    UTILS_API void setSidePanelVisible(bool enable);

    UTILS_API WindowGeometry getWindowGeometry() const;

    /*!
     * Sets the \p windowGeometry. This method does \em not emit the signal
     * #changed(), as this method is typically called upon application termination,
     * when storing the main window geometry.
     *
     * \param windowGeometry
     *        the WindowGeometry containing the values of the last MainWindow geometry
     */
    UTILS_API void setWindowGeometry(const WindowGeometry windowGeometry);

    UTILS_API static QSize getDefaultWindowSize();

    UTILS_API QLocale getLocale() const;

    /*!
     * \sa #changed()
     */
    UTILS_API void setLocale(const QLocale &locale);

    UTILS_API bool isSystemLocaleEnabled() const;

    /*!
     * \sa #changed()
     */
    UTILS_API void setSystemLocaleEnaled(bool enable);

public slots:
    /*!
     * Stores these Settings to a user configuration file.
     */
    UTILS_API void store();

    /*!
     * Restores these Settings from a user configuration file. If no user
     * configuration is present the settings are set to default values.
     *
     * \sa #changed()
     */
    UTILS_API void restore();

signals:
    /*!
     * Emitted when these Settings have changed.
     */
    void changed();

protected:
    virtual ~Settings();

private:
    Q_DISABLE_COPY(Settings)
    SettingsPrivate *d;

    Settings();

};

#endif // SETTINGS_H
