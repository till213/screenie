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
#include <QStringList>
#include <QSlider>
#include <QLineEdit>
#include <QIntValidator>

#include "../../../Utils/src/SizeFitter.h"
#include "../../../Model/src/ScreenieTemplateModel.h"
#include "../../../Model/src/SceneLimits.h"
#include "../ScreenieControl.h"
#include "TemplateModelPropertiesWidget.h"
#include "PropertyValidatorWidget.h"
#include "ui_TemplateModelPropertiesWidget.h"

class TemplateModelPropertiesWidgetPrivate
{
public:
    TemplateModelPropertiesWidgetPrivate(ScreenieTemplateModel &templateModel, ScreenieControl &theScreenieControl)
        : screenieTemplateModel(templateModel),
          screenieControl(theScreenieControl)
    {}

    ScreenieTemplateModel &screenieTemplateModel;
    ScreenieControl &screenieControl;
    bool ignoreUpdateSignals;
};

TemplateModelPropertiesWidget::TemplateModelPropertiesWidget(ScreenieTemplateModel &templateModel, ScreenieControl &screenieControl, QWidget *parent) :
    PropertyValidatorWidget(parent),
    ui(new Ui::TemplateModelPropertiesWidget),
    d(new TemplateModelPropertiesWidgetPrivate(templateModel, screenieControl))
{
    ui->setupUi(this);
    initialiseUi();
    updateUi();
    frenchConnection();
}

TemplateModelPropertiesWidget::~TemplateModelPropertiesWidget()
{
#ifdef DEBUG
    qDebug("TemplateModelPropertiesWidget::~TemplateModelPropertiesWidget(): called.");
#endif
    delete ui;
    delete d;
}

// private

void TemplateModelPropertiesWidget::initialiseUi()
{
    QStringList items;
    items << tr("No Fit") << tr("Fit") << tr("Fit To Width") << tr("Fit To Height") << tr("Exact Fit");
    ui->fitModeComboBox->addItems(items);

    QIntValidator *integerValidator = new QIntValidator(this);
    ui->widthLineEdit->setValidator(integerValidator);
    ui->heightLineEdit->setValidator(integerValidator);
}

void TemplateModelPropertiesWidget::frenchConnection()
{
    connect(&d->screenieTemplateModel, SIGNAL(changed()),
            this, SLOT(updateUi()));
}

// private slots

void TemplateModelPropertiesWidget::updateUi()
{
    int width = d->screenieTemplateModel.getSize().width();
    int height = d->screenieTemplateModel.getSize().height();
    ui->widthLineEdit->setText(QString::number(width));
    validate(*ui->widthLineEdit, true);
    ui->heightLineEdit->setText(QString::number(height));
    validate(*ui->heightLineEdit, true);
    SizeFitter &sizeFitter = d->screenieTemplateModel.getSizeFitter();
    switch (sizeFitter.getFitMode()) {
    case SizeFitter::FitMode::NoFit:
        ui->fitModeComboBox->setCurrentIndex(0);
        ui->respectOrientationCheckBox->setEnabled(false);
        ui->enlargeCheckBox->setEnabled(false);
        break;
    case SizeFitter::FitMode::Fit:
        ui->fitModeComboBox->setCurrentIndex(1);
        ui->respectOrientationCheckBox->setEnabled(true);
        ui->enlargeCheckBox->setEnabled(true);
        break;
    case SizeFitter::FitMode::FitToWidth:
        ui->fitModeComboBox->setCurrentIndex(2);
        ui->respectOrientationCheckBox->setEnabled(false);
        ui->enlargeCheckBox->setEnabled(true);
        break;
    case SizeFitter::FitMode::FitToHeight:
        ui->fitModeComboBox->setCurrentIndex(3);
        ui->respectOrientationCheckBox->setEnabled(false);
        ui->enlargeCheckBox->setEnabled(true);
        break;
    case SizeFitter::FitMode::ExactFit:
        ui->fitModeComboBox->setCurrentIndex(4);
        ui->respectOrientationCheckBox->setEnabled(true);
        ui->enlargeCheckBox->setEnabled(true);
        break;
    default:
#ifdef DEBUG
        qCritical("TemplateModelPropertiesDialog::updateUi: UNSUPPORTED Fit Mode: %d", d->screenieTemplateModel.getSizeFitter().getFitMode());
#endif
        break;
    }
    ui->respectOrientationCheckBox->setChecked(sizeFitter.isFitOptionEnabled(SizeFitter::FitOption::RespectOrientation));
    ui->enlargeCheckBox->setChecked(sizeFitter.isFitOptionEnabled(SizeFitter::FitOption::Enlarge));
    ui->idLineEdit->setText(QString::number(d->screenieTemplateModel.getOrder()));
}

void TemplateModelPropertiesWidget::on_widthLineEdit_editingFinished()
{
    bool ok;
    int width = ui->widthLineEdit->text().toInt(&ok);
    if (ok && width <= SceneLimits::MaxTemplateSize.width()) {
        d->screenieControl.setTargetWidth(width, &d->screenieTemplateModel);
        validate(*ui->widthLineEdit, true);
    } else {
        validate(*ui->widthLineEdit, false);
    }
}

void TemplateModelPropertiesWidget::on_heightLineEdit_editingFinished()
{
    bool ok;
    int height = ui->heightLineEdit->text().toInt(&ok);
    if (ok && height <= SceneLimits::MaxTemplateSize.height()) {
        d->screenieControl.setTargetHeight(height, &d->screenieTemplateModel);
        validate(*ui->heightLineEdit, true);
    } else {
        validate(*ui->heightLineEdit, false);
    }
}

void TemplateModelPropertiesWidget::on_fitModeComboBox_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        d->screenieControl.setFitMode(SizeFitter::FitMode::NoFit, &d->screenieTemplateModel);
        break;
    case 1:
        d->screenieControl.setFitMode(SizeFitter::FitMode::Fit, &d->screenieTemplateModel);
        break;
    case 2:
        d->screenieControl.setFitMode(SizeFitter::FitMode::FitToWidth, &d->screenieTemplateModel);
        break;
    case 3:
        d->screenieControl.setFitMode(SizeFitter::FitMode::FitToHeight, &d->screenieTemplateModel);
        break;
    case 4:
        d->screenieControl.setFitMode(SizeFitter::FitMode::ExactFit, &d->screenieTemplateModel);
        break;
    default:
#ifdef DEBUG
        qCritical("TemplateModelPropertiesDialog::on_fitModeComboBox_currentIndexChanged: UNSUPPORTED Fit Mode: %d", index);
#endif
        break;
    }
}

void TemplateModelPropertiesWidget::on_respectOrientationCheckBox_toggled(bool checked)
{
    d->screenieControl.setFitOptionEnabled(SizeFitter::FitOption::RespectOrientation, checked, &d->screenieTemplateModel);
}

void TemplateModelPropertiesWidget::on_enlargeCheckBox_toggled(bool checked)
{
    d->screenieControl.setFitOptionEnabled(SizeFitter::FitOption::Enlarge, checked, &d->screenieTemplateModel);
}
