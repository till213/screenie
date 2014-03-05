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

#ifndef ABSTRACTSCREENIEMODEL_H
#define ABSTRACTSCREENIEMODEL_H

#include <QPointF>
#include <QImage>

class AbstractScreenieModelPrivate;

#include "ScreenieModelInterface.h"

class AbstractScreenieModel : public ScreenieModelInterface
{
    Q_OBJECT

public:
    AbstractScreenieModel();
    /*!
     * Copy c'tor.
     */
    explicit AbstractScreenieModel(const AbstractScreenieModel &other);
    virtual ~AbstractScreenieModel();

    virtual QPointF getPosition() const override;
    virtual void setPosition(QPointF position) override;
    virtual void setPositionX(qreal x) override;
    virtual void setPositionY(qreal y) override;
    virtual void translate(qreal dx, qreal dy) override;

    virtual qreal getDistance() const override;
    virtual void setDistance(qreal distance) override;
    virtual void addDistance(qreal distance) override;

    virtual int getRotation() const override;
    virtual void setRotation(int angle) override;
    virtual void rotate(int angle) override;

    virtual bool isReflectionEnabled() const override;
    virtual void setReflectionEnabled(bool enable) override;

    virtual int getReflectionOffset() const override;
    virtual void setReflectionOffset(int reflectionOffset) override;
    virtual void addReflectionOffset(int reflectionOffset) override;

    virtual int getReflectionOpacity() const override;
    virtual void setReflectionOpacity(int reflectionOpacity) override;
    virtual void addReflectionOpacity(int reflectionOpacity) override;
    virtual ReflectionMode getReflectionMode() const;
    virtual void setReflectionMode(ReflectionMode ReflectionMode);

    virtual void setSelected(bool enable) override;
    virtual bool isSelected() const override;

    virtual void convert(ScreenieModelInterface &source) override;

protected:
    QImage fitToMaximumSize(const QImage &image) const;

private:
    AbstractScreenieModelPrivate *d;
};

#endif // ABSTRACTSCREENIEMODEL_H
