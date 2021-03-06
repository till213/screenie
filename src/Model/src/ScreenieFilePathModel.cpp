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
#include <QDir>
#include <QImage>

#include "../../Utils/src/SizeFitter.h"
#include "../../Utils/src/PaintTools.h"
#include "ScreenieFilePathModel.h"

class ScreenieFilePathModelPrivate
{
public:

    ScreenieFilePathModelPrivate(const QString &theFilePath, const SizeFitter *theSizeFitter)
        : valid(false),
          transparency(PaintTools::Transparency::Unknown),
          filePath(theFilePath),
          sizeFitter(theSizeFitter)
    {}

    ScreenieFilePathModelPrivate(const ScreenieFilePathModelPrivate &other)
        : valid(other.valid),
          transparency(other.transparency),
          filePath(other.filePath),
          image(other.image),
          sizeFitter(other.sizeFitter)
    {}

    bool valid;
    PaintTools::Transparency transparency;
    QString filePath;
    QImage image;
    const SizeFitter *sizeFitter;    
};

ScreenieFilePathModel::ScreenieFilePathModel(const QString &filePath, const SizeFitter *sizeFitter)
    : d(new ScreenieFilePathModelPrivate(filePath, sizeFitter))
{
}

ScreenieFilePathModel::ScreenieFilePathModel(const ScreenieFilePathModel &other)
    : AbstractScreenieModel(other),
      d(new ScreenieFilePathModelPrivate(*other.d))
{
}

ScreenieFilePathModel::~ScreenieFilePathModel()
{
    delete d;
#ifdef DEBUG
    qDebug("ScreenieFilePathModel:~ScreenieFilePathModel: called.");
#endif
}

const QImage &ScreenieFilePathModel::getImage() const
{
    if (d->image.isNull()) {
        d->image.load(d->filePath);

#ifdef DEBUG
        qDebug("ScreenieFilePathModel::getImage: image format: %d has Alpha: %d",
               d->image.format(), d->image.hasAlphaChannel());
#endif
        if (!d->image.isNull()) {

            /*!\todo Switch back to Premultiplied once reflection rendering code can deal with it */
            if (d->image.format() != QImage::Format_ARGB32_Premultiplied) {
                //d->image = d->image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
            }
            if (d->sizeFitter != nullptr) {
                QSize fittedSize;
                bool doResize = d->sizeFitter->fit(d->image.size(), fittedSize);
                if (doResize) {
                    d->image = d->image.scaled(fittedSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                }
            } else {
                d->image = fitToMaximumSize(d->image);
            }
            d->valid = true;
        } else {
            d->image = PaintTools::createDefaultImage();
            d->valid = false;
        }
    }
    return d->image;
}

QSize ScreenieFilePathModel::getSize() const
{
    QSize result;
    if (!d->image.isNull()) {
        result = d->image.size();
    } else {
        /*!\todo Optimisation: use Exiv2 (or http://www.sentex.net/~mwandel/jhead/) library to quickly read the image size from disk (EXIF data). */
        result = getImage().size();
    }
    return result;
}

ScreenieModelInterface *ScreenieFilePathModel::copy() const
{
    ScreenieFilePathModel *result = new ScreenieFilePathModel(*this);
    return result;
}

bool ScreenieFilePathModel::isTemplate() const
{
    return false;
}

bool ScreenieFilePathModel::hasTransparency() const
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
       qCritical("ScreenieFilePathModel::hasTransparency: UNSUPPORTED transparency: %d", d->transparency);
#endif
        break;
    }
    return result;
}

QString ScreenieFilePathModel::getOverlayText() const
{
    QString result;
    if (!d->valid) {
        result = QDir::toNativeSeparators(d->filePath);
    }
    return result;
}


void ScreenieFilePathModel::setFilePath(const QString &filePath)
{
    if (d->filePath != filePath) {
        d->filePath = filePath;
        d->image = QImage();
        emit filePathChanged(filePath);
    }
}

QString ScreenieFilePathModel::getFilePath() const
{
    return d->filePath;
}



