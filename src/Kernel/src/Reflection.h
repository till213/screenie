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

#ifndef REFLECTION_H
#define REFLECTION_H

class ReflectionPrivate;

#include <QtGui/QImage>

#include "KernelLib.h"

/*!
 * Glass reflection effect.
 */
class Reflection
{
public:
    KERNEL_API Reflection();
    KERNEL_API ~Reflection();

    /*!
     * Creates a reflected version of the \p pixmap, along with gradient translucency
     * defined by \p opacity and \p offset.
     *
     * \param image
     *        the QImage to which the reflection is to be added
     * \param opacity
     *        the opacity (translucency) of the reflection in percent [0, 100];
     *        0: fully translucent; 100: fully opaque
     * \param offset
     *        the offset of the gradient in percent [1, 100] of the \c image's height
     */
    KERNEL_API QImage createReflection(const QImage &image, int opacity, int offset);

private:
    ReflectionPrivate *d;

    void updateGradient(int height);
    void updateImages(int width, int height);
};

#endif // PAINTTOOLS_H
