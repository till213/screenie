/* This file is part of the Screenie project.
   Screenie is a fancy screenshot composer.

   Copyright (C) 2015 Oliver Knoll <till.oliver.knoll@gmail.com>

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

#include <QProxyStyle>

#include "Windows8ProxyStyle.h"

// Public

Windows8ProxyStyle::Windows8ProxyStyle()
{
}

Windows8ProxyStyle::~Windows8ProxyStyle()
{
#ifdef DEBUG
    qDebug("Windows8ProxyStyle::~Windows8ProxyStyle: called.");
#endif
}

int Windows8ProxyStyle::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
    int pixelMetric;
    if (metric == QStyle::PM_SmallIconSize) {
        pixelMetric = 32;
    } else {
        pixelMetric = QProxyStyle::pixelMetric(metric, option, widget);
    }
    return pixelMetric;
}

