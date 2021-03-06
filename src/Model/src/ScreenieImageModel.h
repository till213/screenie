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

#ifndef SCREENIEIMAGEMODEL_H
#define SCREENIEIMAGEMODEL_H

#include <QObject>
#include <QSize>
#include <QString>
#include <QImage>

#include "AbstractScreenieModel.h"
#include "ModelLib.h"

class ScreenieImageModelPrivate;

/*!
 * Implementation note: we need do export the whole class here, since
 * we also need to export the QObject::staticMetaObject methods from
 * the QObject base class.
 */
class MODEL_API ScreenieImageModel : public AbstractScreenieModel
{
    Q_OBJECT
public:
    /*!
     * Creates this ScreenieImageModel. Call #readImage() after creation.
     *
     * \sa #readImage()
     */
    explicit ScreenieImageModel(const QImage image = QImage());

    /*!
     * Copy c'tor.
     */
    explicit ScreenieImageModel(const ScreenieImageModel &other);
    virtual ~ScreenieImageModel();

    virtual const QImage &getImage() const override;
    virtual QSize getSize() const override;
    virtual ScreenieModelInterface *copy() const override;
    virtual bool isTemplate() const override;
    virtual bool hasTransparency() const override;
    virtual QString getOverlayText() const override;    

    /*!
     * Sets the \p image data. Scales the image data as not to exceed
     * the maximum image size.
     *
     * \param image
     *        the QImage to be set
     * \sa Settings::getMaximumImageSize()
     */
    void setImage(const QImage image);

private:
    ScreenieImageModelPrivate *d;
};

#endif // SCREENIEIMAGEMODEL_H
