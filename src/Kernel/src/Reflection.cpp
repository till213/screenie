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

#include <QImage>
#include <QLinearGradient>
#include <QPainter>

#include "Reflection.h"

class ReflectionPrivate
{
public:
    ReflectionPrivate()
        : gradient(nullptr),
          lastWidth(-1),
          lastHeight(-1),
          lastReflectionHeight(-1),
          lastOffset(-1)
    {}

    ~ReflectionPrivate()
    {
        if (gradient != nullptr) {
            delete[] gradient;
        }
    }

    QImage reflection;
    QImage background;
    quint8 *gradient;
    QImage mask;
    int lastWidth;
    int lastHeight;
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

QImage Reflection::createReflection(const QImage &image, int offset)
{
    int width = image.width();
    int height = image.height();
    int reflectionHeight = qRound(height * offset / 100.0);
    int top = height - 1;
    int red, green, blue, alpha;
    quint8 gradient;

    updateGradient(reflectionHeight);
    updateImages(width, height);
    d->lastWidth = width;
    d->lastHeight = height;
    d->lastReflectionHeight = reflectionHeight;

    d->reflection.fill(Qt::transparent);
    for (int y = 0; y < reflectionHeight; ++y) {
        gradient = d->gradient[y];
        const QRgb *src = reinterpret_cast<const QRgb *>(image.constScanLine(top - y));
        QRgb *dst = reinterpret_cast<QRgb *>(d->reflection.scanLine(y));

        for (int x = 0; x < width; ++x) {
            red = qRed(*src);
            green = qGreen(*src);
            blue = qBlue(*src);
            alpha = qAlpha(*src);
            if (gradient < alpha) {
                alpha = gradient;
            }
            *dst = qRgba(red, green, blue, alpha);
            src++;
            dst++;
        }
    }

    return d->reflection;
}

// private

void Reflection::updateImages(int width, int height)
{
    if (width != d->lastWidth ||
        height != d->lastHeight) {
//        d->reflection = QImage(width, height, QImage::Format_ARGB32_Premultiplied);
//        d->background = QImage(width, height, QImage::Format_ARGB32_Premultiplied);
        d->reflection = QImage(width, height, QImage::Format_ARGB32);
        d->background = QImage(width, height, QImage::Format_ARGB32);
    }
}

void Reflection::updateGradient(int height)
{
    if (height > 0) {
        if (height != d->lastReflectionHeight) {
            if (d->gradient != nullptr) {
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
    } else if (d->gradient != nullptr) {
        delete[] d->gradient;
        d->gradient = nullptr;
    }
}


