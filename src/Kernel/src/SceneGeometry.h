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

#ifndef SCENEGEOMETRY_H
#define SCENEGEOMETRY_H

#include <QtCore/QPointF>
#include <QtGui/QImage>

class QSize;

class ScreenieModelInterface;
class ScreenieTemplateModel;

/*!
 * Geometry calculation methods for scene items.
 */
class SceneGeometry
{
public:
    SceneGeometry();

    static QPointF calculateItemPosition(const ScreenieModelInterface &screenieModel, const QPointF &centerPosition);
    static QImage scaleToTemplate(const ScreenieTemplateModel &templateModel, const QImage &image);
    static QPointF calculateItemPosition(const QPointF &sourcePosition, const QSize &sourceSize, const QSize &targetSize);
    static bool needsClipping(const QSize &originalSize, const QSize &clippedSize);
};

#endif // SCENEGEOMETRY_H
