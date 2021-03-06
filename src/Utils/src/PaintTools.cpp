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
#include <QImage>
#include <QPixmap>
#include <QLinearGradient>
#include <QPainter>
#include <QBrush>

#include "PaintTools.h"

// public

QImage PaintTools::createDefaultImage()
{
    /*!\todo Switch back to Premultiplied once reflection rendering code can deal with it */
    //QImage result = QImage(400, 300, QImage::Format_ARGB32_Premultiplied);
    QImage result = QImage(400, 300, QImage::Format_ARGB32);
    QPainter painter(&result);
    drawBackground(painter, result);
    drawText("?", painter, result);
    return result;
}

QImage PaintTools::createTemplateImage(const QSize &size)
{
    //QImage result = QImage(size, QImage::Format_ARGB32_Premultiplied);
    QImage result = QImage(size, QImage::Format_ARGB32);
    QPainter painter(&result);
    drawBackground(painter, result);
    drawText("T", painter, result);
    return result;
}

QPixmap PaintTools::upperHalf(const QPixmap &pixmap)
{
    return pixmap.copy(0, 0, pixmap.width(), pixmap.height() / 2);
}

QBrush PaintTools::createCheckerPattern()
{
    QBrush result;

    //QImage checker(16, 16, QImage::Format_ARGB32_Premultiplied);
    QImage checker(16, 16, QImage::Format_ARGB32);
    QPainter painter(&checker);
    // Inspired by The GIMP ;)
    painter.fillRect(checker.rect(), QColor(153, 153, 153));
    painter.fillRect(0, 0, 8, 8, QColor(102, 102, 102));
    painter.fillRect(8, 8, 8, 8, QColor(102, 102, 102));
    painter.end();
    result.setTextureImage(checker);

    return result;
}

bool PaintTools::hasTransparency(const QImage &image)
{
    bool result;
    unsigned int length;
    int alpha;
    const QRgb *src = reinterpret_cast<const QRgb *>(image.constBits());

    result = false;
    length = image.width() * image.height();
    for (unsigned int p = 0; p < length; ++p) {
        alpha = qAlpha(*src);
        if (alpha < 255) {
            result = true;
            break;
        }
        src++;
    }
    return result;
}

// private

void PaintTools::drawBackground(QPainter &painter, QImage &image)
{
    QLinearGradient gradient(QPoint(0, 0), QPoint(0, image.height()));
    gradient.setColorAt(0, QColor(192, 192, 192));
    gradient.setColorAt(1, QColor(128, 128, 128));
    painter.fillRect(image.rect(), gradient);
    painter.setPen(QPen(QColor(96, 96, 96), 3, Qt::SolidLine));
    painter.drawRect(image.rect());
}

void PaintTools::drawText(const QString &text, QPainter &painter, QImage &image)
{
    QFont font;
    font.setPixelSize(100);
    painter.setFont(font);
    painter.setPen(Qt::white);
    painter.drawText(image.rect(), Qt::AlignCenter, text);
    painter.end();
}


