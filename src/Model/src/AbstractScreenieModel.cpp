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

#include <cmath>
#include <vector>

#include <QStringList>
#include <QPointF>
#include <QImage>
#include <QCoreApplication>

#include "../../Utils/src/Settings.h"
#include "SceneLimits.h"
#include "DefaultScreenieModel.h"

#include "AbstractScreenieModel.h"

class AbstractScreenieModelPrivate
{
public:
    AbstractScreenieModelPrivate()
        : position(QPointF(0.0, 0.0)),
          distance(DefaultScreenieModel::Distance),
          rotation(DefaultScreenieModel::Rotation),
          reflectionOffset(DefaultScreenieModel::ReflectionOffset),
          reflectionOpacity(DefaultScreenieModel::ReflectionOpacity),
          reflectionMode(DefaultScreenieModel::ReflectionMode),
          selected(false)

    {}

    AbstractScreenieModelPrivate(const AbstractScreenieModelPrivate &other)
        : position(other.position),
          distance(other.distance),
          rotation(other.rotation),
          reflectionOffset(other.reflectionOffset),
          reflectionOpacity(other.reflectionOpacity),
          reflectionMode(other.reflectionMode),
          selected(other.selected)
    {}

    QPointF position;
    qreal distance;
    int rotation;
    int reflectionOffset;
    int reflectionOpacity;
    ScreenieModelInterface::ReflectionMode reflectionMode;
    bool selected;

    static const qreal Epsilon;
    static const std::vector<char const *> ReflectionModeItems;
};

const qreal AbstractScreenieModelPrivate::Epsilon = 0.001;
const std::vector<char const *> AbstractScreenieModelPrivate::ReflectionModeItems = {QT_TRANSLATE_NOOP("AbstractScreenieModel", "None"),
                                                                                     QT_TRANSLATE_NOOP("AbstractScreenieModel", "Opaque"),
                                                                                     QT_TRANSLATE_NOOP("AbstractScreenieModel", "Transparent")
                                                                                    };

// public

AbstractScreenieModel::AbstractScreenieModel()
    : d(new AbstractScreenieModelPrivate())
{
}

AbstractScreenieModel::AbstractScreenieModel(const AbstractScreenieModel &other)
    : ScreenieModelInterface(),
      d(new AbstractScreenieModelPrivate(*other.d))
{
}

AbstractScreenieModel::~AbstractScreenieModel()
{
    delete d;
#ifdef DEBUG
    qDebug("AbstractScreenieModel:~AbstractScreenieModel: called.");
#endif
}

QPointF AbstractScreenieModel::getPosition() const {
    return d->position;
}

void AbstractScreenieModel::setPosition(QPointF position)
{
    if (d->position != position) {
        d->position = position;
        emit positionChanged();
    }
}

void AbstractScreenieModel::setPositionX(qreal x)
{
    if (d->position.x() != x) {
        d->position.setX(x);
        emit positionChanged();
    }
}

void AbstractScreenieModel::setPositionY(qreal y)
{
    if (d->position.y() != y) {
        d->position.setY(y);
        emit positionChanged();
    }
}

void AbstractScreenieModel::translate(qreal dx, qreal dy)
{
    QPointF newPosition = QPointF(d->position.x() + dx, d->position.y() + dy);
    setPosition(newPosition);
}

qreal AbstractScreenieModel::getDistance() const
{
    return d->distance;
}

void AbstractScreenieModel::setDistance(qreal distance)
{
    if (::fabs(d->distance - distance) > AbstractScreenieModelPrivate::Epsilon && SceneLimits::MinDistance <= distance && distance <= SceneLimits::MaxDistance) {
        d->distance = distance;
        emit distanceChanged();
    }
}

void AbstractScreenieModel::addDistance(qreal distance)
{
    if (qAbs(distance) > AbstractScreenieModelPrivate::Epsilon) {
        int oldDistance = d->distance;
        d->distance += distance;
        if (d->distance < 0.0) {
            d->distance = 0.0;
        } else if (d->distance > SceneLimits::MaxDistance) {
            d->distance = SceneLimits::MaxDistance;
        }
        if (qAbs(d->distance - oldDistance) > AbstractScreenieModelPrivate::Epsilon) {
            emit distanceChanged();
        }
    }
}

int AbstractScreenieModel::getRotation() const
{
    return d->rotation;
}

void AbstractScreenieModel::setRotation(int rotation) {
    if (d->rotation != rotation) {
        d->rotation = rotation;
        emit rotationChanged();
    }
}

void AbstractScreenieModel::rotate(int angle) {
    if (angle != 0) {
        d->rotation = (d->rotation + angle) % 360;
        while (d->rotation < 0) {
            d->rotation += 360;
        }
        emit rotationChanged();
    }
}

bool AbstractScreenieModel::isReflectionEnabled() const
{
    return d->reflectionMode != ReflectionMode::None;
}

int AbstractScreenieModel::getReflectionOffset() const
{
    return d->reflectionOffset;
}

void AbstractScreenieModel::setReflectionOffset(int reflectionOffset)
{
    if (d->reflectionOffset != reflectionOffset) {
        d->reflectionOffset = reflectionOffset;
        emit reflectionChanged();
    }
}

void AbstractScreenieModel::addReflectionOffset(int reflectionOffset)
{
    if (reflectionOffset != 0) {
        int oldReflectionOffset = d->reflectionOffset;
        d->reflectionOffset += reflectionOffset;
        if (d->reflectionOffset < 1) {
            d->reflectionOffset = 1;
        } else if (d->reflectionOffset > 100) {
            d->reflectionOffset = 100;
        }
        if (d->reflectionOffset != oldReflectionOffset) {
            emit reflectionChanged();
        }
    }
}

int AbstractScreenieModel::getReflectionOpacity() const
{
    return d->reflectionOpacity;
}

void AbstractScreenieModel::setReflectionOpacity(int reflectionOpacity)
{
    if (d->reflectionOpacity != reflectionOpacity) {
        d->reflectionOpacity = reflectionOpacity;
        emit reflectionChanged();
    }
}

void AbstractScreenieModel::addReflectionOpacity(int reflectionOpacity)
{
    if (reflectionOpacity != 0) {
        int oldReflectionOpacity = d->reflectionOpacity;
        d->reflectionOpacity += reflectionOpacity;
        if (d->reflectionOpacity < 0) {
            d->reflectionOpacity = 0;
        } else if (d->reflectionOpacity > 100) {
            d->reflectionOpacity = 100;
        }
        if (d->reflectionOpacity != oldReflectionOpacity) {
            emit reflectionChanged();
        }
    }
}

ScreenieModelInterface::ReflectionMode AbstractScreenieModel::getReflectionMode() const
{
    return d->reflectionMode;
}

void AbstractScreenieModel::setReflectionMode(ReflectionMode reflectionMode)
{
    if (d->reflectionMode != reflectionMode) {
        d->reflectionMode = reflectionMode;
        emit reflectionModeChanged(d->reflectionMode);
    }
}

void AbstractScreenieModel::setSelected(bool enable)
{
    if (d->selected != enable) {
        d->selected = enable;
        emit selectionChanged();
    }
}

bool AbstractScreenieModel::isSelected() const
{
    return d->selected;
}

void AbstractScreenieModel::convert(ScreenieModelInterface &source)
{
    d->position = source.getPosition();
    d->distance = source.getDistance();
    d->rotation = source.getRotation();
    d->reflectionOffset = source.getReflectionOffset();
    d->reflectionOpacity = source.getReflectionOpacity();
    d->reflectionMode = source.getReflectionMode();
}

// protected

QImage AbstractScreenieModel::fitToMaximumSize(const QImage &image) const
{
    QImage result;
    QSize maximumImageSize = Settings::getInstance().getMaximumImagePixelSize();
    QSize actualSize = image.size();
    if (actualSize.width() > maximumImageSize.width() || actualSize.height() > maximumImageSize.height()) {
        result = image.scaled(maximumImageSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    } else {
        result = image;
    }
    return result;
}

QStringList AbstractScreenieModel::getReflectionModeItems()
{
    QStringList result;
    for (auto &&string : AbstractScreenieModelPrivate::ReflectionModeItems) {
        result.append(QCoreApplication::instance()->translate("AbstractScreenieModel", string));
    }
    return result;
}
