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

#include <QSize>
#include <QString>
#include <QImage>

#include "../../Utils/src/PaintTools.h"
#include "ScreenieImageModel.h"

class ScreenieImageModelPrivate
{
public:
    ScreenieImageModelPrivate()
        : transparency(PaintTools::Transparency::Unknown)
    {}

    ScreenieImageModelPrivate(const ScreenieImageModelPrivate &other)
        : image(other.image)
    {}

    PaintTools::Transparency transparency;
    QImage image;
};

ScreenieImageModel::ScreenieImageModel(const QImage image)
    : d(new ScreenieImageModelPrivate())
{
    d->image = fitToMaximumSize(image);
}

ScreenieImageModel::ScreenieImageModel(const ScreenieImageModel &other)
    : AbstractScreenieModel(other),
      d(new ScreenieImageModelPrivate(*other.d))
{
}

ScreenieImageModel::~ScreenieImageModel()
{
    delete d;
#ifdef DEBUG
    qDebug("ScreenieImageModel:~ScreenieImageModel: called.");
#endif
}

const QImage &ScreenieImageModel::getImage() const
{
    return d->image;
}

QSize ScreenieImageModel::getSize() const
{
    return d->image.size();
}

ScreenieModelInterface *ScreenieImageModel::copy() const
{
    ScreenieImageModel *result = new ScreenieImageModel(*this);
    return result;
}

bool ScreenieImageModel::isTemplate() const
{
    return false;
}

bool ScreenieImageModel::hasTransparency() const
{
    bool result;
    switch (d->transparency) {
    case PaintTools::Transparency::Yes:
        result = true;
        break;
    case PaintTools::Transparency::No:
        result = false;
        break;
    case PaintTools::Transparency::Unknown:
        result = PaintTools::hasTransparency(getImage());
        d->transparency = result ? PaintTools::Transparency::Yes : PaintTools::Transparency::No;
        break;
    default:
#ifdef DEBUG
       qCritical("ScreenieImageModel::hasTransparency: UNSUPPORTED transparency: %d", d->transparency);
#endif
        break;
    }
    return result;
}

QString ScreenieImageModel::getOverlayText() const
{
    return QString();
}

void ScreenieImageModel::setImage(const QImage image)
{
    if (d->image.cacheKey() != image.cacheKey()) {
        d->image = fitToMaximumSize(image);
        emit imageChanged();
    }
}



