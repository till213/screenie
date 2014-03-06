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

#ifndef SCREENIECONTROL_H
#define SCREENIECONTROL_H

#include <QObject>
#include <QTimer>
#include <QPointF>
#include <QString>
#include <QStringList>
#include <QList>
#include <QBrush>
#include <QColor>
#include <QImage>

class QImage;
class QString;
class QStringList;
class QMimeData;

class ScreenieModelInterface;
class ScreenieScene;
class ScreenieGraphicsScene;
class ScreeniePixmapItem;
class ScreenieFilePathModel;
class ScreenieTemplateModel;
class Reflection;
class ScreenieControlPrivate;

#include "../../Utils/src/SizeFitter.h"
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
    enum class RenderQuality
    {
        Low, /*!< Antialiasing disabled, fast pixmap transformation */
        Maximum /*!< Antialiasing enabled, smooth pixmap transformation */
    };

    KERNEL_API ScreenieControl(ScreenieScene &screenieScene, ScreenieGraphicsScene &screenieGraphicsScene);
    KERNEL_API virtual ~ScreenieControl();

    KERNEL_API QList<ScreenieModelInterface *> getSelectedScreenieModels() const;
    KERNEL_API QList<ScreenieTemplateModel *> getSelectedTemplateModels() const;
    KERNEL_API QList<ScreenieFilePathModel *> getSelectedFilePathModels() const;

    KERNEL_API DefaultScreenieModel &getDefaultScreenieModel();

    /*!
     * Updates the QGraphicsView with the ScreenieScene. The QGraphicsView is first cleared.
     */
    KERNEL_API void updateScene();
    KERNEL_API void updateModel(const QMimeData *mimeData, ScreenieModelInterface &screenieModel);

    ScreenieScene &getScreenieScene() const;
    ScreenieGraphicsScene &getScreenieGraphicsScene() const;

public slots:
    KERNEL_API void addImage(QString filePath, QPointF centerPosition = QPointF(0.0, 0.0));
    KERNEL_API void addImages(QStringList filePaths, QPointF centerPosition = QPointF(0.0, 0.0));
    KERNEL_API void addImage(QImage image, QPointF centerPosition = QPointF(0.0, 0.0));
    KERNEL_API void addImages(QList<QImage> images, QPointF centerPosition = QPointF(0.0, 0.0));
    KERNEL_API void addTemplate(QPointF centerPosition = QPointF(0.0, 0.0));

    KERNEL_API void removeAll();
    KERNEL_API void selectAll();

    KERNEL_API void setPositionX(qreal x, ScreenieModelInterface *screenieModel = nullptr);
    KERNEL_API void setPositionY(qreal y, ScreenieModelInterface *screenieModel = nullptr);
    KERNEL_API void setPosition(QPointF position, ScreenieModelInterface *screenieModel = nullptr);
    KERNEL_API void translate(qreal dx, qreal dy, ScreenieModelInterface *screenieModel = nullptr);
    KERNEL_API void setRotation(int angle, ScreenieModelInterface *screenieModel = nullptr);
    KERNEL_API void rotate(int angle, ScreenieModelInterface *screenieModel = nullptr);
    KERNEL_API void setDistance(qreal distance, ScreenieModelInterface *screenieModel = nullptr);
    KERNEL_API void addDistance(qreal distance, ScreenieModelInterface *screenieModel = nullptr);

    KERNEL_API void setReflectionOffset(int reflectionOffset, ScreenieModelInterface *screenieModel = nullptr);
    KERNEL_API void addReflectionOffset(int reflectionOffset, ScreenieModelInterface *screenieModel = nullptr);
    KERNEL_API void setReflectionOpacity(int reflectionOpacity, ScreenieModelInterface *screenieModel = nullptr);
    KERNEL_API void addReflectionOpacity(int reflectionOpacity, ScreenieModelInterface *screenieModel = nullptr);
    KERNEL_API void setReflectionMode(ScreenieModelInterface::ReflectionMode reflectionMode, ScreenieModelInterface *screenieModel = nullptr);

    KERNEL_API void setBackgroundEnabled(bool enable);
    KERNEL_API void setBackgroundColor(QColor color);
    KERNEL_API void setRedBackgroundComponent(int red);
    KERNEL_API void setGreenBackgroundComponent(int green);
    KERNEL_API void setBlueBackgroundComponent(int blue);

    KERNEL_API void setFilePath(const QString &filePath, ScreenieFilePathModel *screenieFilePathModel = nullptr);

    KERNEL_API void setTargetWidth(int width, ScreenieTemplateModel *screenieTemplateModel = nullptr);
    KERNEL_API void setTargetHeight(int height, ScreenieTemplateModel *screenieTemplateModel = nullptr);
    KERNEL_API void setFitMode(SizeFitter::FitMode fitMode, ScreenieTemplateModel *screenieTemplateModel = nullptr);
    KERNEL_API void setFitOptionEnabled(SizeFitter::FitOption fitOption, bool enable, ScreenieTemplateModel *screenieTemplateModel = nullptr);

    KERNEL_API void convertItemsToTemplate(ScreenieScene &screenieScene);
    KERNEL_API void setRenderQuality(RenderQuality renderQuality);

private:
    ScreenieControlPrivate *d;

    void frenchConnection();

    QList<ScreeniePixmapItem *> getScreeniePixmapItems() const;
    void updateEditRenderQuality();
    void applyDefaultValues(ScreenieModelInterface &screenieModelInterface);

    void updateImageModel(const QImage &image, ScreenieModelInterface &screenieModel);
    void updateFilePathModel(const QString &filePath, ScreenieModelInterface &screenieModel);

    // returns 'screenieModel' in the list if set to != nullptr, otherwise all selected models are returned in the list
    QList<ScreenieModelInterface *> getEditableModels(ScreenieModelInterface *screenieModel = nullptr);
    // returns 'screenieFilePathModel' in the list if set to != nullptr, otherwise all selected filepath models are returned in the list
    QList<ScreenieFilePathModel *> getEditableFilePathModels(ScreenieFilePathModel *screenieFilePathModel = nullptr);
    // returns 'screenieTemplateModel' in the list if set to != nullptr, otherwise all selected template models are returned in the list
    QList<ScreenieTemplateModel *> getEditableTemplateModels(ScreenieTemplateModel *screenieTemplateModel = nullptr);

private slots:
    void handleFilePathsDrop(QStringList filePaths, QPointF centerPosition);
    void handleImageDrop(QList<QImage> images, QPointF centerPosition);
    void handleDistanceChanged();
    void handleModelAdded(ScreenieModelInterface &screenieModel);
    void handleModelRemoved(ScreenieModelInterface &screenieModel);
    void handleBackgroundChanged();
    void restoreRenderQuality();
};

#endif // SCREENIECONTROL_H
