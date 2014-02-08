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

#include <QtCore/QPointF>
#include <QtCore/QSize>
#include <QtGui/QImage>

#include "../../Utils/src/SizeFitter.h"
#include "../../Model/src/ScreenieModelInterface.h"
#include "../../Model/src/ScreenieTemplateModel.h"
#include "SceneGeometry.h"

// public

QPointF SceneGeometry::calculateItemPosition(const ScreenieModelInterface &screenieModel, const QPointF &centerPosition)
{
    QPointF result;
    QSize itemSize = screenieModel.getSize();
#ifdef DEBUG
    qDebug("SceneGeometry::calculateItemPosition: item size: %d, %d, center pos: %f, %f",
           itemSize.width(), itemSize.height(),
           centerPosition.x(), centerPosition.y());
#endif
    result.setX(centerPosition.x() - itemSize.width()  / 2.0);
    result.setY(centerPosition.y() - itemSize.height() / 2.0);
    return result;
}

QImage SceneGeometry::scaleToTemplate(const ScreenieTemplateModel &templateModel, const QImage &image)
{
    QImage result;
    const SizeFitter &sizeFitter = templateModel.getSizeFitter();
    QSize fittedSize;
    QRect clippedRect;
    bool doResize = sizeFitter.fit(image.size(), fittedSize, &clippedRect);
    if (doResize) {
        if (needsClipping(image.size(), clippedRect.size())) {
            result = image.copy(clippedRect);
            result = result.scaled(fittedSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        }
        else {
            result = image.scaled(fittedSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        }
    } else {
        result = image;
    }
    return result;
}

QPointF SceneGeometry::calculateItemPosition(const QPointF &sourcePosition, const QSize &sourceSize, const QSize &targetSize)
{
    QPointF result;
    result.setX(sourcePosition.x() + sourceSize.width()  / 2.0 - targetSize.width()  / 2.0);
    result.setY(sourcePosition.y() + sourceSize.height() / 2.0 - targetSize.height() / 2.0);
    return result;
}

bool SceneGeometry::needsClipping(const QSize &originalSize, const QSize &clippedSize)
{
    int dw, dh;  // difference of original and clipped size (width/height)
    float pw, ph; // percentage of original size (width/height)
    bool result;

    dw = originalSize.width()  - clippedSize.width();
    dh = originalSize.height() - clippedSize.height();
    pw = (100.0f * dw) / originalSize.width();
    ph = (100.0f * dh) / originalSize.height();

    /*!\todo Make threshold (currently 2%) an (advanced) user setting? */
    result = qMax(pw, ph) > 2.0;
    return result;
}
