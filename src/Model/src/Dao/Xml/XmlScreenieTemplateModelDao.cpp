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

#include <QString>
#include <QIODevice>
#include <QBitArray>
#include <QXmlStreamWriter>
#include <QSize>

#include "../../../../Utils/src/SizeFitter.h"
#include "../../ScreenieTemplateModel.h"
#include "XmlScreenieTemplateModelDao.h"

class XmlScreenieTemplateModelDaoPrivate
{
public:
    const ScreenieTemplateModel *writeModel;
    ScreenieTemplateModel *readModel;
};

//  public

XmlScreenieTemplateModelDao::XmlScreenieTemplateModelDao(QXmlStreamWriter &xmlStreamWriter)
    : AbstractXmlScreenieModelDao(xmlStreamWriter),
      d(new XmlScreenieTemplateModelDaoPrivate())
{
}

XmlScreenieTemplateModelDao::XmlScreenieTemplateModelDao(QXmlStreamReader &xmlStreamReader)
    : AbstractXmlScreenieModelDao(xmlStreamReader),
      d(new XmlScreenieTemplateModelDaoPrivate())
{
}

XmlScreenieTemplateModelDao::XmlScreenieTemplateModelDao(QIODevice &device)
    : AbstractXmlScreenieModelDao(device),
      d(new XmlScreenieTemplateModelDaoPrivate())
{
}

XmlScreenieTemplateModelDao::~XmlScreenieTemplateModelDao()
{
#ifdef DEBUG
    qDebug("XmlScreenieTemplateModelDao::~XmlScreenieTemplateModelDao: called.");
#endif
    delete d;
}

bool XmlScreenieTemplateModelDao::write(const ScreenieTemplateModel &screenieTemplateModel)
{
    bool result;
    d->writeModel = &screenieTemplateModel;
    result = AbstractXmlScreenieModelDao::write(screenieTemplateModel);
    return result;
}

ScreenieTemplateModel *XmlScreenieTemplateModelDao::read()
{
    ScreenieTemplateModel *result = new ScreenieTemplateModel();
    d->readModel = result;
    AbstractXmlScreenieModelDao::read(*result);
    return result;
}

// protected

bool XmlScreenieTemplateModelDao::writeSpecific()
{
    bool result = true;
    QXmlStreamWriter *streamWriter = getStreamWriter();
    streamWriter->writeStartElement("template");
    {
        QXmlStreamAttributes templateAttributes;
        templateAttributes.append("order", QString::number(d->writeModel->getOrder()));
        streamWriter->writeAttributes(templateAttributes);

        result = writeSizeFitter(d->writeModel->getSizeFitter());
    }
    streamWriter->writeEndElement();
    return result;
}

bool XmlScreenieTemplateModelDao::readSpecific()
{
    bool result = true;
    QXmlStreamReader *streamReader = getStreamReader();
    streamReader->readNextStartElement();
    QXmlStreamAttributes templateAttributes = streamReader->attributes();
    int order = templateAttributes.value("order").toString().toInt(&result);
    if (result) {
        d->readModel->setOrder(order);
        result = readSizeFitter(d->readModel->getSizeFitter());
    }
    streamReader->skipCurrentElement();
    return result;
}

// private

bool XmlScreenieTemplateModelDao::writeSizeFitter(const SizeFitter &sizeFitter)
{
    bool result = true;
    QXmlStreamWriter *streamWriter = getStreamWriter();
    streamWriter->writeStartElement("sizefitter");
    {
        QXmlStreamAttributes sizeFitterAttributes;
        sizeFitterAttributes.append("mode", QString::number(static_cast<std::underlying_type<SizeFitter::FitMode>::type>(sizeFitter.getFitMode())));
        SizeFitter::FitOptions fitOptions = d->writeModel->getSizeFitter().getFitOptions();
        sizeFitterAttributes.append("options", QString("%1").arg(fitOptions));
        sizeFitterAttributes.append("width", QString::number(d->writeModel->getSizeFitter().getTargetSize().width()));
        sizeFitterAttributes.append("height", QString::number(d->writeModel->getSizeFitter().getTargetSize().height()));
        streamWriter->writeAttributes(sizeFitterAttributes);
    }
    streamWriter->writeEndElement();
    return result;
}

bool XmlScreenieTemplateModelDao::readSizeFitter(SizeFitter &sizeFitter)
{
    bool result = true;
    QXmlStreamReader *streamReader = getStreamReader();
    streamReader->readNextStartElement();
    QXmlStreamAttributes sizeFitterAttributes = streamReader->attributes();
    int mode = sizeFitterAttributes.value("mode").toString().toInt(&result);
    if (result) {
        sizeFitter.setFitMode(static_cast<SizeFitter::FitMode>(mode));
        int fitOptionValue = sizeFitterAttributes.value("options").toInt(&result);
        if (result) {
            SizeFitter::FitOptions fitOptions;
            fitOptions = static_cast<SizeFitter::FitOptions>(fitOptionValue);
            sizeFitter.setFitOptions(fitOptions);
            int width = sizeFitterAttributes.value("width").toString().toInt(&result);
            if (result) {
                int height = sizeFitterAttributes.value("height").toString().toInt(&result);
                if (result) {
                    sizeFitter.setTargetSize(QSize(width, height));
                }
            }
        }
    }
    streamReader->skipCurrentElement();
    return result;
}

