/* This file is part of the Screenie project.
   Screenie is a fancy screenshot composer.

   Copyright (C) 2008 Ariya Hidayat <ariya.hidayat@gmail.com>

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

#ifndef SCREENIECONTROL_H
#define SCREENIECONTROL_H

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QPointF>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QList>
#include <QtGui/QBrush>
#include <QtGui/QColor>
#include <QtGui/QPixmap>

class QStringList;
class QMimeData;

class ScreenieModelInterface;
class ScreenieScene;
class ScreenieGraphicsScene;
class ScreeniePixmapItem;
class ScreenieTemplateModel;
class Reflection;

#include "../../Model/src/DefaultScreenieModel.h"
#include "KernelLib.h"

/*!
 * This is the controller of the ScreenieScene (model): it modifies the <em>selected</em> ScreenieModelInterface.
 */
class ScreenieControl : public QObject
{
    Q_OBJECT

public:
    /*!
     * Defines the QGraphicsView render quality.
     */
    enum RenderQuality
    {
        Low, /*!< Antialiasing disabled, fast pixmap transformation */
        High /*!< Antialiasing enabled, smooth pixmap transformation */
    };

    KERNEL_API ScreenieControl(ScreenieScene &screenieScene, ScreenieGraphicsScene &screenieGraphicsScene);
    KERNEL_API virtual ~ScreenieControl();

    KERNEL_API QList<ScreenieModelInterface *> getSelectedScreenieModels() const;

    KERNEL_API DefaultScreenieModel &getDefaultScreenieModel();

    KERNEL_API void updateModel(const QMimeData *mimeData, ScreenieModelInterface &screenieModel);

    ScreenieScene &getScreenieScene() const;
    ScreenieGraphicsScene &getScreenieGraphicsScene() const;

public slots:
    KERNEL_API void addImage(QString filePath, QPointF centerPosition = QPointF(0.0, 0.0));
    KERNEL_API void addImages(QStringList filePaths, QPointF centerPosition = QPointF(0.0, 0.0));
    KERNEL_API void addImage(QPixmap pixmap, QPointF centerPosition = QPointF(0.0, 0.0));
    KERNEL_API void addImages(QList<QPixmap> pixmaps, QPointF centerPosition = QPointF(0.0, 0.0));
    KERNEL_API void addTemplate(QPointF centerPosition = QPointF(0.0, 0.0));

    KERNEL_API void removeAll();
    KERNEL_API void selectAll();

    KERNEL_API void translate(qreal dx, qreal dy);
    KERNEL_API void setRotation(int angle);
    KERNEL_API void rotate(int angle);
    KERNEL_API void setDistance(int distance);
    KERNEL_API void addDistance(int distance);

    KERNEL_API void setReflectionEnabled(bool enable);
    KERNEL_API void setReflectionOffset(int reflectionOffset);
    KERNEL_API void addReflectionOffset(int reflectionOffset);
    KERNEL_API void setReflectionOpacity(int reflectionOpacity);
    KERNEL_API void addReflectionOpacity(int reflectionOpacity);

    KERNEL_API void setBackgroundEnabled(bool enable);
    KERNEL_API void setBackgroundColor(QColor color);
    KERNEL_API void setRedBackgroundComponent(int red);
    KERNEL_API void setGreenBackgroundComponent(int green);
    KERNEL_API void setBlueBackgroundComponent(int blue);

private:
    ScreenieScene &m_screenieScene;
    ScreenieGraphicsScene &m_screenieGraphicsScene;
    QBrush m_checkerBoardBrush;
    QTimer m_qualityTimer;
    Reflection *m_reflection; /*! \todo The Reflection effect does not belong here. Add an "FX Manager" which keeps track of effects instead */
    DefaultScreenieModel m_defaultScreenieModel;

    void frenchConnection();

    QList<ScreeniePixmapItem *> getScreeniePixmapItems() const;
    void setRenderQuality(RenderQuality renderQuality);
    void applyDefaultValues(ScreenieModelInterface &screenieModelInterface);
    /*!\todo Put these methods in some Kernel "Geometry" class or somewhere */
    QPointF calculateItemPosition(const ScreenieModelInterface &screenieModel, const QPointF &centerPosition);
    QPixmap scaleToTemplate(const ScreenieTemplateModel &templateModel, const QPixmap &pixmap);
    QPointF calculateItemPosition(const QPointF &sourcePosition, const QSize &sourceSize, const QSize &targetSize);

    void updatePixmapModel(const QMimeData *mimeData, ScreenieModelInterface &screenieModel);
    void updateFilePathModel(const QMimeData *mimeData, ScreenieModelInterface &screenieModel);

private slots:
    void handleDistanceChanged();
    void handleModelAdded(ScreenieModelInterface &screenieModel);
    void handleModelRemoved(const ScreenieModelInterface &screenieModel);
    void handleBackgroundChanged();
    void restoreRenderQuality();
};

#endif // SCREENIECONTROL_H
