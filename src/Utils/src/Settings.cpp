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

#include <QPoint>
#include <QSize>
#include <QString>
#include <QStringList>
#include <QSettings>
#include <QDir>
#include <QStandardPaths>
#include <QGuiApplication>
#include <QScreen>

#include "Version.h"
#include "Settings.h"

class SettingsPrivate
{
public:
    static const QSize DefaultMaximumImageSize;
    static const QSize DefaultTemplateSize;
    static const QString DefaultLastImageDirectoryPath;
    static const QString DefaultLastExportDirectoryPath;
    static const QString DefaultLastDocumentDirectoryPath;
    static const qreal DefaultRotationGestureSensitivity;
    static const qreal DefaultDistanceGestureSensitivity;
    static const Settings::EditRenderQuality DefaultEditRenderQuality;
    static const bool DefaultToolBarVisible = true;
    static const bool DefaultSidePanelVisible = true;
    static const bool DefaultFullScreen;
    static const QRect DefaultMainWindowPosition;
    static const QSize DefaultMainWindowSize;

    Version version;
    QSize maximumImageSize;
    QSize templateSize;
    QString lastImageDirectoryPath;
    QString lastExportDirectoryPath;
    QString lastDocumenDirectoryPath;
    qreal rotationGestureSensitivity;
    qreal distanceGestureSensitivity;

    Settings::EditRenderQuality editRenderQuality;
    bool toolBarVisible;
    bool sidePanelVisible;

    QSettings *settings;

    static Settings *instance;

    SettingsPrivate() : maximumImageSize(DefaultMaximumImageSize) {
#ifdef Q_OS_WIN
        // On Windows prefer INI format over Registry (= NativeFormat)
        QSettings::Format format = QSettings::IniFormat;
#else
        QSettings::Format format = QSettings::NativeFormat;
#endif
        settings = new QSettings(format, QSettings::UserScope, "till-art.net", Version::getApplicationName());
    }

    ~SettingsPrivate() {
        delete settings;
    }
};

Settings *SettingsPrivate::instance = nullptr;

const QSize SettingsPrivate::DefaultMaximumImageSize = QSize(1024, 1024);
const QSize SettingsPrivate::DefaultTemplateSize = QSize(400, 300);
const QString SettingsPrivate::DefaultLastImageDirectoryPath = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).first();
const QString SettingsPrivate::DefaultLastExportDirectoryPath = SettingsPrivate::DefaultLastImageDirectoryPath;
const QString SettingsPrivate::DefaultLastDocumentDirectoryPath = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first();
const qreal SettingsPrivate::DefaultRotationGestureSensitivity = 2.0; // these values work well on a MacBook Pro ;)
const qreal SettingsPrivate::DefaultDistanceGestureSensitivity = 10.0;
const Settings::EditRenderQuality SettingsPrivate::DefaultEditRenderQuality = Settings::LowQuality;
const bool SettingsPrivate::DefaultFullScreen = false;
const QSize SettingsPrivate::DefaultMainWindowSize = QSize(800, 600);
const QRect SettingsPrivate::DefaultMainWindowPosition = QRect();

// public

Settings &Settings::getInstance()
{
    if (SettingsPrivate::instance == nullptr) {
        SettingsPrivate::instance = new Settings();
    }
    return *SettingsPrivate::instance;
}

void Settings::destroyInstance()
{
    if (SettingsPrivate::instance != nullptr) {
        delete SettingsPrivate::instance;
        SettingsPrivate::instance = nullptr;
    }
}

QSettings &Settings::getSettings()
{
    return *d->settings;
}

const QSize &Settings::getMaximumImageSize() const
{
    return d->maximumImageSize;
}

void Settings::setMaximumImageSize(const QSize &maximumImageSize)
{
    if (d->maximumImageSize != maximumImageSize) {
        d->maximumImageSize = maximumImageSize;
        emit changed();
    }
}

const QSize &Settings::getTemplateSize() const
{
    return d->templateSize;
}

void Settings::setTemplateSize(const QSize &templateSize)
{
    if (d->templateSize != templateSize) {
        d->templateSize = templateSize;
        emit changed();
    }
}

const QString &Settings::getLastImageDirectoryPath() const
{
    return d->lastImageDirectoryPath;
}

void Settings::setLastImageDirectoryPath(const QString &lastImageDirectoryPath)
{
    QString qtPath = QDir::fromNativeSeparators(lastImageDirectoryPath);
    if (d->lastImageDirectoryPath != qtPath) {
        d->lastImageDirectoryPath = qtPath;
        emit changed();
    }
}

const QString &Settings::getLastExportDirectoryPath() const
{
    return d->lastExportDirectoryPath;
}

void Settings::setLastExportDirectoryPath(const QString &lastExportDirectoryPath)
{
    QString qtPath = QDir::fromNativeSeparators(lastExportDirectoryPath);
    if (d->lastExportDirectoryPath != qtPath) {
        d->lastExportDirectoryPath = qtPath;
        emit changed();
    }
}

const QString &Settings::getLastDocumentDirectoryPath() const
{
    return d->lastDocumenDirectoryPath;
}

void Settings::setLastDocumentDirectoryPath(const QString &lastDocumentDirectoryPath)
{
    QString qtPath = QDir::fromNativeSeparators(lastDocumentDirectoryPath);
    if (d->lastDocumenDirectoryPath != qtPath) {
        d->lastDocumenDirectoryPath = qtPath;
        emit changed();
    }
}

qreal Settings::getRotationGestureSensitivity() const
{
   return d->rotationGestureSensitivity;
}

void Settings::setRotationGestureSensitivity(qreal rotationGestureSensitivity)
{
    if (d->rotationGestureSensitivity != rotationGestureSensitivity) {
        d->rotationGestureSensitivity = rotationGestureSensitivity;
        emit changed();
    }
}

qreal Settings::getDistanceGestureSensitivity() const
{
   return d->distanceGestureSensitivity;
}

void Settings::setDistanceGestureSensitivity(qreal distanceGestureSensitivity)
{
    if (d->distanceGestureSensitivity != distanceGestureSensitivity) {
        d->distanceGestureSensitivity = distanceGestureSensitivity;
        emit changed();
    }
}

Settings::EditRenderQuality Settings::getEditRenderQuality() const
{
    return d->editRenderQuality;
}

void Settings::setEditRenderQuality(EditRenderQuality editRenderQuality)
{
    if (d->editRenderQuality != editRenderQuality) {
        d->editRenderQuality = editRenderQuality;
        emit changed();
    }
}

bool Settings::isToolBarVisible() const
{
    return d->toolBarVisible;
}

void Settings::setToolBarVisible(bool enable)
{
    if (d->toolBarVisible != enable) {
        d->toolBarVisible = enable;
        emit changed();
    }
}

bool Settings::isSidePanelVisible() const
{
    return d->sidePanelVisible;
}

void Settings::setSidePanelVisible(bool enable)
{
    if (d->sidePanelVisible != enable) {
        d->sidePanelVisible = enable;
        emit changed();
    }
}

Settings::WindowGeometry Settings::getWindowGeometry() const
{
    WindowGeometry result;
    d->settings->beginGroup("UI/MainWindow");
    {
        result.fullScreen = d->settings->value("FullScreen", SettingsPrivate::DefaultFullScreen).toBool();
        result.position = d->settings->value("Position", SettingsPrivate::DefaultMainWindowPosition).toRect();
    }
    d->settings->endGroup();
    return result;
}

void Settings::setWindowGeometry(const WindowGeometry windowGeometry)
{
    d->settings->beginGroup("UI/MainWindow");
    {
        d->settings->setValue("FullScreen", windowGeometry.fullScreen);
        d->settings->setValue("Position", windowGeometry.position);
    }
    d->settings->endGroup();
}

QSize Settings::getDefaultWindowSize()
{
    return SettingsPrivate::DefaultMainWindowSize;
}

// public slots

void Settings::store()
{
    d->settings->setValue("Version", d->version.toString());
    d->settings->setValue("Scene/MaximumImageSize", d->maximumImageSize);
    d->settings->setValue("Scene/TemplateSize", d->templateSize);
    d->settings->beginGroup("Paths");
    {
        d->settings->setValue("LastImageDirectoryPath", d->lastImageDirectoryPath);
        d->settings->setValue("LastExportDirectoryPath", d->lastExportDirectoryPath);
        d->settings->setValue("LastDocumentDirectoryPath", d->lastDocumenDirectoryPath);
    }
    d->settings->endGroup();
    d->settings->beginGroup("UI");
    {
        d->settings->setValue("EditRenderQuality", d->editRenderQuality);
        d->settings->setValue("ToolBarVisible", d->toolBarVisible);
        d->settings->setValue("SidePanelVisible", d->sidePanelVisible);
        d->settings->beginGroup("Gestures");
        {
            d->settings->setValue("RotationSensitivity", d->rotationGestureSensitivity);
            d->settings->setValue("DistanceSensitivity", d->distanceGestureSensitivity);
        }
        d->settings->endGroup();
    }
    d->settings->endGroup();
}

void Settings::restore()
{
    QString version;
    Version appVersion;
    version = d->settings->value("Version", appVersion.toString()).toString();
    Version settingsVersion(version);
    if (settingsVersion < appVersion) {
#ifdef DEBUG
        qDebug("Settings::restore: app version: %s, settings version: %s, conversion might be necessary!",
               qPrintable(appVersion.toString()), qPrintable(settingsVersion.toString()));
        /*!\todo Settings conversion as necessary */
#endif
    }
    d->maximumImageSize = d->settings->value("Scene/MaximumImageSize", SettingsPrivate::DefaultMaximumImageSize).toSize();
    d->templateSize = d->settings->value("Scene/TemplateSize", SettingsPrivate::DefaultTemplateSize).toSize();
    d->settings->beginGroup("Paths");
    {
        d->lastImageDirectoryPath = d->settings->value("LastImageDirectoryPath", SettingsPrivate::DefaultLastImageDirectoryPath).toString();
        d->lastExportDirectoryPath = d->settings->value("LastExportDirectoryPath", SettingsPrivate::DefaultLastExportDirectoryPath).toString();
        d->lastDocumenDirectoryPath = d->settings->value("LastDocumentDirectoryPath", SettingsPrivate::DefaultLastDocumentDirectoryPath).toString();
    }
    d->settings->endGroup();
    d->settings->beginGroup("UI");
    {
        d->editRenderQuality = static_cast<EditRenderQuality>(d->settings->value("EditRenderQuality", SettingsPrivate::DefaultEditRenderQuality).toInt());
        d->toolBarVisible = d->settings->value("ToolBarVisible", SettingsPrivate::DefaultToolBarVisible).toBool();
        d->sidePanelVisible = d->settings->value("SidePanelVisible", SettingsPrivate::DefaultSidePanelVisible).toBool();
        d->settings->beginGroup("Gestures");
        {
            d->rotationGestureSensitivity = d->settings->value("RotationSensitivity", SettingsPrivate::DefaultRotationGestureSensitivity).toReal();
            d->distanceGestureSensitivity = d->settings->value("DistanceSensitivity", SettingsPrivate::DefaultDistanceGestureSensitivity).toReal();
        }
        d->settings->endGroup();
    }
    d->settings->endGroup();
}

// protected

Settings::~Settings()
{
    store();
    delete d;
}

// private

Settings::Settings()
{
    d = new SettingsPrivate();
    restore();
}
