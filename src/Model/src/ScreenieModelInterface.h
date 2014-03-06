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

#ifndef SCREENIEMODELINTERFACE_H
#define SCREENIEMODELINTERFACE_H

#include <QObject>
#include <QPointF>
#include <QImage>

#include "ModelLib.h"

class QString;

/*!
 * The ScreenieScene contains items which implement this model interface.
 *
 * \sa SceneLimits
 */
class ScreenieModelInterface : public QObject
{
    Q_OBJECT

public:

    /*!
     * Defines the reflection mode.
     */
    enum class ReflectionMode {
        None = 0, /*!< No reflection */
        Opaque = 1, /*!< The reflection is blended over the opaque background colour */
        Transparent = 2 /*!< The reflection is blended over the scene background */
    };

    virtual ~ScreenieModelInterface() {}

    /*!
     * Returns the QImage.
     *
     * \sa #setFilePath(const QString &)
     */
    virtual const QImage &getImage() const = 0;

    /*!
     * Returns the size of the image.
     *
     * Implementation note: if #readImage has not yet been called the image is currently
     * read from disk first, so it might be potentially expensive, but only for the first
     * time (the image is stored in memory).
     *
     * \return the QSize of the image
     * \sa #readImage()
     */
    virtual QSize getSize() const = 0;

    virtual QPointF getPosition() const = 0;

    /*!
     * \sa #positionChanged()
     */
    virtual void setPosition(QPointF position) = 0;

    /*!
     * \sa #positionChanged()
     */
    virtual void setPositionX(qreal x) = 0;

    /*!
     * \sa #positionChanged()
     */
    virtual void setPositionY(qreal y) = 0;

    /*!
     * \sa #positionChanged()
     */
    virtual void translate(qreal dx, qreal dy) = 0;

    virtual qreal getDistance() const = 0;
    virtual void setDistance(qreal distance) = 0;
    virtual void addDistance(qreal distance) = 0;

    virtual int getRotation() const = 0;

    /*!
     * Rotates this instance to \p angle degrees, that is the rotation is set to the absolute value \p angle.
     *
     * \param angle
     *        absolute angle to which this ScreenieModel is to be rotated in degrees; [0, 360]
     */
    virtual void setRotation(int angle) = 0;

    /*!
     * Rotates this instance by \p angle degrees, that is \p angle is added to the existing rotation.
     *
     * \param angle
     *        relative angle by which this ScreenieModel is to be rotated in degrees
     */
    virtual void rotate(int angle) = 0;

    /*!
     * Returns whether the reflection is enabled or not. This is a convenience method which
     * returns \c false when the \em reflection mode is set to ReflectionMode#None.
     *
     * \return \c true if reflection is enabled; \c false else
     * \sa #getReflectionMode
     */
    virtual bool isReflectionEnabled() const = 0;

    virtual int getReflectionOffset() const = 0;

    /*!
     * \param reflectionOffset
     *        the reflection offset in percent [1, 100]
     */
    virtual void setReflectionOffset(int reflectionOffset) = 0;

    /*!
     * \sa #reflectionChanged()
     */
    virtual void addReflectionOffset(int reflectionOffset) = 0;

    virtual int getReflectionOpacity() const = 0;

    /*!
     * \sa #reflectionChanged()
     */
    virtual void setReflectionOpacity(int reflectionOpacity) = 0;
    virtual void addReflectionOpacity(int reflectionOpacity) = 0;

    virtual ReflectionMode getReflectionMode() const = 0;

    /*!
     * \sa #reflectionModeChanged()
     */
    virtual void setReflectionMode(ReflectionMode ReflectionMode) = 0;

    /*!\ Emit signals? */
    virtual void convert(ScreenieModelInterface &source) = 0;

    /*!
     * Creates a copy of this instance and all its associated data. The caller
     * is the owner.
     *
     * \return a copy of this instance; must be \c deleted by the caller
     */
    virtual ScreenieModelInterface *copy() const = 0;

    /*!
     * \sa #selectionChanged()
     */
    virtual void setSelected(bool enable) = 0;
    virtual bool isSelected() const = 0;

    virtual bool isTemplate() const = 0;

    /*!
     * \return a QString containing the overlay text to be drawn over the pixmap; a \em null QString if nothing to draw
     */
    virtual QString getOverlayText() const = 0;

signals:
    void reflectionChanged();
    void reflectionModeChanged(ScreenieModelInterface::ReflectionMode reflectionMode);
    void distanceChanged();
    void rotationChanged();
    void positionChanged();
    void changed();
    void imageChanged();
    void filePathChanged(const QString &filePath);
    void selectionChanged();
};

Q_DECLARE_METATYPE(ScreenieModelInterface::ReflectionMode)

#endif // SCREENIEMODELINTERFACE_H
