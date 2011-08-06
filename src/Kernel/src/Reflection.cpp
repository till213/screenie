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

#include <QtGui/QImage>
#include <QtGui/QLinearGradient>
#include <QtGui/QPainter>

#include "Reflection.h"

class ReflectionPrivate
{
public:
    ReflectionPrivate()
        : gradient(0),
          lastWidth(-1),
          lastReflectionHeight(-1),
          lastOffset(-1) {}

    ~ReflectionPrivate()
    {
        if (gradient != 0) {
            delete[] gradient;
        }
    }

    QImage reflection;
    QImage background;
    quint8 *gradient;
    QImage mask;
    int lastWidth;
    int lastReflectionHeight;
    int lastOffset;
};

// public

Reflection::Reflection()
{
    d = new ReflectionPrivate();
}

Reflection::~Reflection() {
    delete d;
}

QImage Reflection::createReflection(const QImage &image, int opacity, int offset)
{
    int width = image.width();
    int height = image.height();
    int reflectionHeight = qRound(height * offset / 100.0);
    quint8 gradient;

    updateGradient(reflectionHeight);
    updateImages(width, reflectionHeight);
    d->lastWidth = width;
    d->lastReflectionHeight = reflectionHeight;

    for (int y = 0; y < reflectionHeight; ++y) {
        gradient = d->gradient[y];
        const QRgb *src = reinterpret_cast<const QRgb *>(image.constScanLine(height - 1 - y));
        QRgb *dst = reinterpret_cast<QRgb *>(d->reflection.scanLine(y));
#ifdef DEBUG
        qDebug("Input image format: %d", image.format());
#endif
        for (int x = 0; x < width; ++x) {
            int red = qRed(*src);
            int green = qRed(*src);
            int blue = qBlue(*src);
            int alpha = qAlpha(*src);
            if (gradient < alpha) {
                alpha = gradient;
            }
            *dst = qRgba(red, green, blue, alpha);
            src++;
            dst++;
        }
    }

//    QImage result(image.width(), image.height() * 2, QImage::Format_ARGB32_Premultiplied);
//    result.fill(Qt::transparent);

//    QPainter painter(&result);
//    painter.drawImage(0, 0, image);
//    painter.setOpacity(qMin(1.0, opacity / 100.0));
//    painter.drawImage(0, image.height(), reflect(image, offset));
//    painter.end();

    return d->reflection;
}

// private

void Reflection::updateImages(int width, int height)
{
    if (width != d->lastWidth ||
        height != d->lastReflectionHeight) {
        d->reflection = QImage(width, height, QImage::Format_ARGB32_Premultiplied);
        d->background = QImage(width, height, QImage::Format_ARGB32_Premultiplied);
    }
}

void Reflection::updateGradient(int height)
{
    if (height > 0) {
        if (height != d->lastReflectionHeight) {
            if (d->gradient != 0) {
                delete[] d->gradient;
            }
            d->gradient = new quint8[height];
            if (height > 1) {
                for (int y = 0; y < height; ++y) {
                    d->gradient[y] = qRound(255.0 - 255.0 * y / (height - 1.0));
                }
            } else {
                d->gradient[0] = 128;
            }
        }
    } else if (d->gradient != 0) {
        delete[] d->gradient;
        d->gradient = 0;
    }
}

QImage Reflection::reflect(const QImage &image, int offset) const
{
    QLinearGradient gradient(QPoint(0, 0), QPoint(0, image.height()));
    if (offset <= 0) {
        // make sure the black offset is never 0.0, but at least 1% (arbitrarily small);
        // otherwise the entire "gradient" would be white
        offset = 1;
    }
    gradient.setColorAt(qMin(1.0, offset / 100.0), Qt::black);
    gradient.setColorAt(0.0, Qt::white);

    bool recreate;
    if (image.size() != d->mask.size()) {
        /*! \todo What is the fastest format here to draw a simple linear gradient (without alpha)
                  and apply it to another image (with alpha) as an alpha mask later on? */
        d->mask = QImage(image.size(), QImage::Format_ARGB32_Premultiplied);
        recreate = true;
    } else {
        recreate = false;
    }

    // repaint gradient IF either image size or offset (since last time) have changed
    if (recreate || d->lastOffset != offset) {
        QImage alphaChannel = image.alphaChannel();
        QPainter painter(&d->mask);
        painter.setCompositionMode(QPainter::CompositionMode_Source);
        painter.fillRect(d->mask.rect(), gradient);
        painter.end();
        /*!\todo Optimise this */
        if (!alphaChannel.isNull()) {
            for (int y = 0; y < d->mask.height(); ++y) {
                for (int x = 0; x < d->mask.width(); ++x) {
                    QRgb value = d->mask.pixel(x, y);
                    QRgb alpha = alphaChannel.pixel(x, y);
                    if (alpha < value) {
                        d->mask.setPixel(x, y, alpha);
                    }
                }
            }
        }
        d->lastOffset = offset;
    }
    //alphaChannel.save("/Users/tknoll/alphaChannel.png", "PNG");
    d->mask.save("/Users/tknoll/mask.png", "PNG");

    QImage result = image.mirrored();
    result.setAlphaChannel(d->mask);

    return result;
}


