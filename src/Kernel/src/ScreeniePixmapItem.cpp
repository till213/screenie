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

#include <QtCore/QPoint>
#include <QtCore/QMimeData>
#include <QtCore/QUrl>
#include <QtCore/QEvent>
#include <QtCore/QRectF>
#include <QtGui/QGraphicsPixmapItem>
#include <QtGui/QGraphicsItem>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QPainter>
#include <QtGui/QBrush>
#include <QtGui/QPixmap>
#include <QtGui/QImage>
#include <QtGui/QFont>
#include <QtGui/QFontMetrics>
#include <QtGui/QDialog>
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>

#include "../../Utils/src/PaintTools.h"
#include "../../Model/src/ScreenieModelInterface.h"
#include "../../Model/src/ScreenieScene.h"
#include "../../Model/src/SceneLimits.h"
#include "Clipboard/MimeHelper.h"
#include "Reflection.h"
#include "ScreenieControl.h"
#include "PropertyDialogFactory.h"
#include "ScreeniePixmapItem.h"

const int ScreeniePixmapItem::ScreeniePixmapType = QGraphicsItem::UserType + 1;

class ScreeniePixmapItemPrivate
{
public:
    ScreeniePixmapItemPrivate(ScreenieModelInterface &theScreenieModel, ScreenieControl &theScreenieControl, const ScreenieScene &scene)
        : screenieModel(theScreenieModel),
          screenieControl(theScreenieControl),
          screenieScene(scene),
          reflection(new Reflection()),
          transformPixmap(true),
          ignoreUpdates(false),
          itemTransformed(false),
          propertyDialogFactory(new PropertyDialogFactory(screenieControl)),
          propertyDialog(0),
          checkerPattern(PaintTools::createCheckerPattern())
    {}

    ~ScreeniePixmapItemPrivate()
    {
        delete reflection;
        delete propertyDialogFactory;
    }

    ScreenieModelInterface &screenieModel;
    ScreenieControl &screenieControl;
    const ScreenieScene &screenieScene;
    Reflection *reflection;
    QSizeF imageSize;
    QRectF reflectionBoundingRect;
    bool transformPixmap;
    bool ignoreUpdates;
    bool itemTransformed;
    PropertyDialogFactory *propertyDialogFactory;
    QPoint initialPoint;
    QDialog *propertyDialog;
    QBrush checkerPattern;
    QImage alphaChannel;

    static const int ContextActionThreshold;
};

// This threshold is supposed to be small enough as not to delay normal
// rotation actions, and large enough to handle normal "random mouse jitter" when clicking
// the right mouse button for context menu
const int ScreeniePixmapItemPrivate::ContextActionThreshold = 2;

// public

ScreeniePixmapItem::ScreeniePixmapItem(ScreenieModelInterface &screenieModel, ScreenieControl &screenieControl, const  ScreenieScene &screenieScene)
    : QGraphicsPixmapItem(),
      d(new ScreeniePixmapItemPrivate(screenieModel, screenieControl, screenieScene))
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setTransformationMode(Qt::SmoothTransformation);
    // we also want to be able to change the reflection also in the fully translucent areas
    // of the reflection
    setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
    updatePixmap(d->screenieModel.getImage());
    setAcceptDrops(true);
    frenchConnection();
}

ScreeniePixmapItem::~ScreeniePixmapItem()
{
#ifdef DEBUG
    qDebug("ScreeniePixmapItem::~ScreeniePixmapItem: called.");
#endif
}

ScreenieModelInterface &ScreeniePixmapItem::getScreenieModel() const
{
    return d->screenieModel;
}

// protected

int ScreeniePixmapItem::type() const
{
    return ScreeniePixmapType;
}

void ScreeniePixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    d->transformPixmap = isInsidePixmap(event->pos());
    d->itemTransformed = false;
    d->initialPoint = event->lastScreenPos();
#ifdef DEBUG
    qDebug("ScreeniePixmapItem::mousePressEvent: initial point: %d, %d",
           d->initialPoint.x(),
           d->initialPoint.y());
#endif
    event->accept();
}

void ScreeniePixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!d->itemTransformed && event->buttons() & Qt::RightButton) {
        if (::fabs(d->initialPoint.x() - event->screenPos().x()) > ScreeniePixmapItemPrivate::ContextActionThreshold ||
            ::fabs(d->initialPoint.y() - event->screenPos().y()) > ScreeniePixmapItemPrivate::ContextActionThreshold) {
            d->itemTransformed = true;
        }
#ifdef DEBUG
        else {
            qDebug("ScreeniePixmapItem::mouseMoveEvent: within threshold point: %d, %d",
                   event->screenPos().x(),
                   event->screenPos().y());
        }
#endif
    } else {
        d->itemTransformed = true;
    }
    if (d->itemTransformed) {
        if (isInsidePixmap(event->pos()) && d->transformPixmap) {
            transformPixmap(event);
        } else if (!d->transformPixmap) {
            changeReflection(event);
        }
    }
}

void ScreeniePixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    switch (event->button()) {
    case Qt::RightButton:
        if (!d->itemTransformed) {
            event->accept();
            // for now we only allow properties of a single item to be modified at the same time
            selectExclusive();
            QWidget *parent;
            if (scene() != 0) {
                parent = scene()->views().first()->viewport();
            } else {
                parent = 0;
            }
            if (d->propertyDialog == 0) {
                d->propertyDialog = d->propertyDialogFactory->createDialog(d->screenieModel);
                if (d->propertyDialog != 0) {
                    connect(d->propertyDialog, SIGNAL(destroyed()),
                            this, SLOT(handlePropertyDialogDestroyed()));
                    QPoint position = this->calculateDialogPosition(event->screenPos());
                    d->propertyDialog->move(position);
                    d->propertyDialog->show();
                }
            }
        }
        break;
    default:
        QGraphicsPixmapItem::mouseReleaseEvent(event);
        break;
    }
}

void ScreeniePixmapItem::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    int value;
    // On Windows 7 pinch gestures are mapped to wheel events with CTRL pressed,
    // also refer to: http://msdn.microsoft.com/en-us/library/dd940543%28v=vs.85%29.aspx
    if (Qt::ControlModifier & event->modifiers()) {
        value = -event->delta() / 12;
    } else {
        value = event->delta() / 12;
    }
    if (isInsidePixmap(event->pos())) {
        addDistance(value);
    } else {
        addReflectionOpacity(value);
    }
    event->accept();
}

void ScreeniePixmapItem::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    bool accept = MimeHelper::accept(event->mimeData(), MimeHelper::Strict);
    event->setAccepted(accept);
}

void ScreeniePixmapItem::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    d->screenieControl.updateModel(event->mimeData(), d->screenieModel);
    event->accept();
}

QVariant ScreeniePixmapItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange) {
        QPointF position = value.toPointF();
        d->screenieModel.setPosition(position);
    } else if (change == ItemSelectedChange) {
        QGraphicsPixmapItem::itemChange(change, value);
        // See comment below in #transformPixmap
        d->ignoreUpdates = true;
        d->screenieModel.setSelected(value.toBool());
    }
    return QGraphicsPixmapItem::itemChange(change, value);
}

void ScreeniePixmapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (d->screenieModel.isReflectionEnabled()) {
        if (d->screenieScene.isBackgroundEnabled()) {
            //painter->
            //if (d->alphaChannel.isNull()) {
                //painter->fillRect(d->reflectionBoundingRect, d->screenieScene.getBackgroundColor());
            //} else {
            //    painter->drawImage(d->reflectionBoundingRect.topLeft(), d->alphaChannel);
            //}
            //QImage alpha = d->screenieModel.get
        } else {
//            QPainter::CompositionMode compositionMode = painter->compositionMode();
//            painter->setCompositionMode(QPainter::CompositionMode_Source);
//            painter->fillRect(d->reflectionBoundingRect, d->checkerPattern);
//            painter->setCompositionMode(compositionMode);
        }
        // todo: Remove this, testing only
        //painter->setPen(Qt::red);
        //painter->drawRect(d->reflectionBoundingRect);
    }
    QGraphicsPixmapItem::paint(painter, option, widget);
    QString overlayText = d->screenieModel.getOverlayText();
    if (!overlayText.isNull()) {
        QRectF boundingRect = this->boundingRect();
        /*!\todo Optimise this; cache the font, re-calculate when overlay text changes (add signal) */
        if (boundingRect.width() > 100 && boundingRect.height() > 48) {
            boundingRect.adjust(10.0, 10.0, -10.0, -10.0);
            int length = overlayText.length();
            QFont font("Arial");
            if (length > 10) {
                font.setPixelSize(16);
            } else {
                font.setPixelSize(24);
            }
            QFontMetrics fontMetrics(font);
            overlayText = fontMetrics.elidedText(overlayText, Qt::ElideMiddle, boundingRect.width());
            painter->setPen(Qt::white);
            painter->setFont(font);
            painter->drawText(boundingRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, overlayText);
        }
    }
}

QRectF ScreeniePixmapItem::boundingRect() const
{
    QRectF result;
    if (d->screenieModel.isReflectionEnabled()) {
        result.setWidth(d->imageSize.width());
        result.setHeight(d->imageSize.height() * 2.0);
    } else {
        result.setWidth(d->imageSize.width());
        result.setHeight(d->imageSize.height());
    }
    return result;
}

// private

void ScreeniePixmapItem::frenchConnection()
{
    connect(&d->screenieModel, SIGNAL(changed()),
            this, SLOT(updateItem()));
    connect(&d->screenieModel, SIGNAL(positionChanged()),
            this, SLOT(updatePosition()));
    connect(&d->screenieModel, SIGNAL(rotationChanged()),
            this, SLOT(updateItemGeometry()));
    connect(&d->screenieModel, SIGNAL(distanceChanged()),
            this, SLOT(updateItemGeometry()));
    connect(&d->screenieModel, SIGNAL(reflectionChanged()),
            this, SLOT(updateReflection()));
    connect(&d->screenieModel, SIGNAL(imageChanged(const QImage &)),
            this, SLOT(updatePixmap(const QImage &)));
    connect(&d->screenieModel, SIGNAL(filePathChanged(const QString &)),
            this, SLOT(updatePixmap()));
    connect(&d->screenieModel, SIGNAL(selectionChanged()),
            this, SLOT(updateSelection()));
}

void ScreeniePixmapItem::moveTo(QPointF scenePosition)
{
    selectExclusive();
    d->screenieModel.setPosition(scenePosition);
}

void ScreeniePixmapItem::rotate(int angle)
{
    selectExclusive();
    d->screenieControl.rotate(angle);
}

void ScreeniePixmapItem::addDistance(int distance)
{
    selectExclusive();
    d->screenieControl.addDistance(distance);
}

bool ScreeniePixmapItem::isInsidePixmap(QPointF itemPosition)
{
    bool result;
    if (d->screenieModel.isReflectionEnabled()) {
        QRectF boundingRect = this->boundingRect();
        result = itemPosition.y() < boundingRect.height() / 2.0;
    } else {
        result = true;
    }
    return result;
}

void ScreeniePixmapItem::transformPixmap(QGraphicsSceneMouseEvent *event)
{
    int deltaX, deltaY;
    switch (event->buttons()) {
    case Qt::RightButton:
        deltaX = event->lastScreenPos().x() - event->screenPos().x();
        rotate(deltaX);
        event->accept();
        break;

     case Qt::MiddleButton:
        deltaY = event->lastScreenPos().y() - event->screenPos().y();
        addDistance(deltaY);
        event->accept();
        break;

    case Qt::LeftButton:
        // Welcome to the Exception To The Rule "Modify model via Controller"!
        // moving items requires quite some logic, and we re-use that logic
        // (the Qt implementation of handling move events, to be specific)
        // So first we update the View, and THEN we update the Model.
        // (see #itemChange above)
        // As we get a signal by the model nevertheless we ignore it my raising
        // a flag, because we already updated the View ourselves
        d->ignoreUpdates = true;
        selectExclusive();
        QGraphicsPixmapItem::mouseMoveEvent(event);
        break;

     default:
        event->ignore();
        break;
    }
}

void ScreeniePixmapItem::changeReflection(QGraphicsSceneMouseEvent *event)
{
    qreal height_2 = boundingRect().height() / 2.0;
    qreal factor  = (event->pos().y() - height_2) / height_2;
    int percent = qRound(factor * 100.0);
    switch (event->buttons()) {
    case Qt::LeftButton:
        if (percent < 1) {
            percent = 1;
        } else if (percent > 100) {
            percent = 100;
        }
        selectExclusive();
        d->screenieControl.setReflectionOffset(percent);
        event->accept();
        break;

    case Qt::RightButton:
        if (percent < 0) {
            percent = 0;
        } else if (percent > 100) {
            percent = 100;
        }
        selectExclusive();
        d->screenieControl.setReflectionOpacity(percent);
        event->accept();
        break;

    default:
        QGraphicsPixmapItem::mouseMoveEvent(event);
        break;
    }
}

void ScreeniePixmapItem::addReflectionOpacity(int reflectionOpacity)
{
    selectExclusive();
    d->screenieControl.addReflectionOpacity(reflectionOpacity);
}

void ScreeniePixmapItem::selectExclusive()
{
    if (!isSelected()) {
        scene()->clearSelection();
        setSelected(true);
    }
}

QPoint ScreeniePixmapItem::calculateDialogPosition(const QPoint &mousePosition)
{
    QPoint result;
    QDesktopWidget *desktopWidget = QApplication::desktop();
    QRect availableGeometry = desktopWidget->availableGeometry(d->propertyDialog);
    d->propertyDialog->adjustSize();
    QSize dialogSize = d->propertyDialog->size();
    if (mousePosition.x() + dialogSize.width() < availableGeometry.right()) {
        result.setX(mousePosition.x());
    } else {
        result.setX(availableGeometry.right() - dialogSize.width());
    }
    if (mousePosition.y() + dialogSize.height() < availableGeometry.bottom()) {
        result.setY(mousePosition.y());
    } else {
        result.setY(availableGeometry.bottom() - dialogSize.height());
    }
    return result;
}


// private slots

void ScreeniePixmapItem::updateReflection()
{
    QImage image = d->screenieModel.getImage();
    QPixmap pixmap;
    QImage reflection;

    if (d->screenieModel.isReflectionEnabled()) {
        reflection = d->reflection->createReflection(image, d->screenieModel.getReflectionOpacity(), d->screenieModel.getReflectionOffset());
        pixmap = QPixmap(image.width(), image.height() << 1);
        QPainter p(&pixmap);
        pixmap.fill(Qt::white);
        p.drawImage(0, 0, image);
        p.setOpacity(d->screenieModel.getReflectionOpacity() / 100.0);
        p.drawImage(0, image.height(), reflection);

    } else {
        pixmap = QPixmap(image.width(), image.height());
        QPainter p(&pixmap);
        pixmap.fill(Qt::white);
        p.drawImage(0, 0, image);
    }

    setPixmap(pixmap);
    updateReflectionBoundingRect();
}

void ScreeniePixmapItem::updateReflectionBoundingRect()
{
    QPointF topLeft;
    QPointF bottomRight;
    if (d->screenieModel.isReflectionEnabled()) {
    QSize size = d->screenieModel.getSize();
        topLeft.setX(1.0);
        topLeft.setY(size.height());
        bottomRight.setX(size.width() - 1.0);
        bottomRight.setY(size.height() + size.height() * d->screenieModel.getReflectionOffset() / 100.0 - 1.0);
        d->reflectionBoundingRect.setTopLeft(topLeft);
        d->reflectionBoundingRect.setBottomRight(bottomRight);
    } else {
        d->reflectionBoundingRect = QRectF();
    }
}

void ScreeniePixmapItem::updatePixmap(const QImage &image)
{
    QPixmap pixmap;

    d->imageSize.setWidth(image.width());
    d->imageSize.setHeight(image.height());
    pixmap.convertFromImage(image);
    if (image.hasAlphaChannel()) {
        d->alphaChannel = image.mirrored().alphaChannel();
    } else {
        d->alphaChannel = QImage();
    }
    setPixmap(pixmap);
    updateReflection();
    updateItemGeometry();
}

void ScreeniePixmapItem::updatePixmap()
{
    updatePixmap(d->screenieModel.getImage());
}

void ScreeniePixmapItem::updateItemGeometry()
{
    QTransform transform;
    QTransform scale;
    QTransform translateBack;

    qreal centerScale = 1.0 - 0.9 * d->screenieModel.getDistance() / SceneLimits::MaxDistance;
    scale = QTransform().scale(centerScale, centerScale);

    qreal dx = d->imageSize.width() / 2.0;
    qreal dy;
    if (d->screenieModel.isReflectionEnabled()) {
        dy =  d->imageSize.height() / 2.0;
    } else {
        dy = d->imageSize.height();
    }
    transform.translate(dx, dy);
    transform.rotate(d->screenieModel.getRotation(), Qt::YAxis);
    translateBack.translate(-dx, -dy);
    transform = translateBack * scale * transform;
    setTransform(transform, false);
}

void ScreeniePixmapItem::updateItem()
{
    update();
}

void ScreeniePixmapItem::updatePosition()
{
    // see comment in #transformPixmap
    if (!d->ignoreUpdates) {
        setPos(d->screenieModel.getPosition());
    }
    d->ignoreUpdates = false;
}

void ScreeniePixmapItem::updateSelection()
{
    // see comment in #transformPixmap
    if (!d->ignoreUpdates) {
        setSelected(d->screenieModel.isSelected());
    }
    d->ignoreUpdates = false;
}

void ScreeniePixmapItem::handlePropertyDialogDestroyed(){
    d->propertyDialog = 0;
}
