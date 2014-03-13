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

#ifndef PAINTTOOLS_H
#define PAINTTOOLS_H

#include <QPixmap>
#include <QImage>
#include <QBrush>

class QPainter;
class QString;
class QSize;

#include "UtilsLib.h"

/*!
 * Helper paint functions.
 */
class PaintTools
{
public:

    /*!
     * Evaluation mode of image transparency.
     */
    enum class Transparency {
      Unknown, /*!< Transparency not yet evaluated */
      No, /*!< Image has no transparent pixels */
      Yes /*!< Image has transparent pixels */
    };

    /*!
     * Creates a stub image with a big '?' in the center.
     */
    UTILS_API static QImage createDefaultImage();

    /*!
     * Creates a template image.
     */
    UTILS_API static QImage createTemplateImage(const QSize &size);

    /*!
     * Returns the upper half of the \p pixmap.
     *
     * \return A QPixmap with a copy of the upper half area of the \p pixmap
     */
    UTILS_API static QPixmap upperHalf(const QPixmap &pixmap);

    /*!
     * Creates a 16x16 checker pattern.
     *
     * \return the QBrush containing the checker pattern
     */
    UTILS_API static QBrush createCheckerPattern();

    /*!
     * Returns whether the \p image has any (semi-)transparent pixels. That is, whether an
     * alpha channel exists and whether any alpha value is smaller than 255.
     *
     * \b Note: if you operate with a ScreenieModelInterface use ScreenieModelInterface#hasTransparency
     * instead! The implementations thereof are more efficient as they cache the result of the previous
     * operation.
     *
     * \param image
     *        the QImage to be evaluated
     * \return \c true if an alpha value < 255 exists; \c false else
     * \sa ScreenieModelInterface#hasTransparency
     */
    UTILS_API static bool hasTransparency(const QImage &image);

private:
    Q_DISABLE_COPY(PaintTools)

    static void drawBackground(QPainter &painter, QImage &image);
    static void drawText(const QString &text, QPainter &painter, QImage &image);
};

#endif // PAINTTOOLS_H
