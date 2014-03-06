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

#include <QObject>

#include "DefaultScreenieModel.h"

class DefaultScreenieModelPrivate
{
public:
    DefaultScreenieModelPrivate()
        : distance(0.0),
          rotation(0),
          reflectionOffset(40),
          reflectionOpacity(50),
          reflectionMode(ScreenieModelInterface::ReflectionMode::Opaque)
    {}

    qreal distance;
    int rotation;
    int reflectionOffset;
    int reflectionOpacity;
    ScreenieModelInterface::ReflectionMode reflectionMode;

};

const qreal DefaultScreenieModel::Distance = 0.0;
const int DefaultScreenieModel::Rotation = 0;
const int DefaultScreenieModel::ReflectionOffset = 40;
const int DefaultScreenieModel::ReflectionOpacity = 50;
const ScreenieModelInterface::ReflectionMode DefaultScreenieModel::ReflectionMode = ScreenieModelInterface::ReflectionMode::Opaque;

// public

DefaultScreenieModel::DefaultScreenieModel()
{
    d = new DefaultScreenieModelPrivate();
}

DefaultScreenieModel::~DefaultScreenieModel()
{
    delete d;
}

qreal DefaultScreenieModel::getDistance() const
{
    return d->distance;
}

void DefaultScreenieModel::setDistance(qreal distance)
{
    d->distance = distance;
}

int DefaultScreenieModel::getRotation() const
{
    return d->rotation;
}

void DefaultScreenieModel::setRotation(int rotation)
{
    d->rotation = rotation;
}

int DefaultScreenieModel::getReflectionOffset() const
{
    return d->reflectionOffset;
}

void DefaultScreenieModel::setReflectionOffset(int reflectionOffset)
{
    d->reflectionOffset = reflectionOffset;
}

int DefaultScreenieModel::getReflectionOpacity() const {
    return d->reflectionOpacity;
}

void DefaultScreenieModel::setReflectionOpacity(int reflectionOpacity)
{
    d->reflectionOpacity = reflectionOpacity;
}

ScreenieModelInterface::ReflectionMode DefaultScreenieModel::getReflectionMode() const
{
    return d->reflectionMode;
}

void DefaultScreenieModel::setReflectionMode(ScreenieModelInterface::ReflectionMode reflectionMode)
{
    d->reflectionMode = reflectionMode;
}

void DefaultScreenieModel::reset()
{
    d->distance = Distance;
    d->rotation = Rotation;
    d->reflectionOffset = ReflectionOffset;
    d->reflectionOpacity = ReflectionOpacity;
    d->reflectionMode = ReflectionMode;
}




