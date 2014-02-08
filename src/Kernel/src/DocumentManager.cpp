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

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QEvent>
#include <QtCore/QSignalMapper>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QAction>
#include <QtWidgets/QActionGroup>

#include "../../Utils/src/FileUtils.h"
#include "../../Model/src/ScreenieScene.h"
#include "DocumentInfo.h"
#include "DocumentManager.h"

class DocumentManagerPrivate
{
public:
    DocumentManagerPrivate()
        : windowActionGroup(new QActionGroup(0))
    {
        windowActionGroup->setExclusive(true);
    }

    ~DocumentManagerPrivate()
    {
        delete windowActionGroup;
    }

    QList<DocumentInfo *> documentInfos;
    QList<QMainWindow *> activeDialogWindows;
    QActionGroup *windowActionGroup;
    QSignalMapper windowMapper;
    static DocumentManager *instance;
    static int nextWindowId;
    static DocumentManager::CloseRequest closeRequest;
};

DocumentManager *DocumentManagerPrivate::instance = nullptr;
int DocumentManagerPrivate::nextWindowId = 1;
DocumentManager::CloseRequest DocumentManagerPrivate::closeRequest = DocumentManager::CloseCurrent;

// public

DocumentManager &DocumentManager::getInstance()
{
    if (DocumentManagerPrivate::instance == nullptr) {
        DocumentManagerPrivate::instance = new DocumentManager();
    }
    return *DocumentManagerPrivate::instance;
}

void DocumentManager::destroyInstance()
{
    if (DocumentManagerPrivate::instance != nullptr) {
        delete DocumentManagerPrivate::instance;
        DocumentManagerPrivate::instance = nullptr;
    }
}

void DocumentManager::add(DocumentInfo *documentInfo)
{
    QMainWindow *mainWindow = documentInfo->getMainWindow();
    d->documentInfos.append(documentInfo);
    connect(mainWindow, SIGNAL(destroyed(QObject *)),
            this, SLOT(remove(QObject *)));
    // update object name ("window ID")
    mainWindow->setObjectName(mainWindow->objectName() + QString::number(d->nextWindowId));
    mainWindow->installEventFilter(this);
    /*!\todo This gets messy: make DocumentInfo a proper class. */
    documentInfo->setWindowId(d->nextWindowId);
    ++d->nextWindowId;
    documentInfo->setName(tr("New %1", "New document title + ID").arg(documentInfo->getWindowId()));
    QAction *action = new QAction(d->windowActionGroup);
    action->setCheckable(true);
    action->setData(documentInfo->getWindowId());
    action->setText(documentInfo->getName());
    d->windowMapper.setMapping(action, documentInfo->getWindowId());
    connect(action, SIGNAL(triggered()),
            &d->windowMapper, SLOT(map()));
    emit changed();
}

DocumentInfo *DocumentManager::getDocumentInfo(const QMainWindow &mainWindow) const
{
    return getDocumentInfoFromObject(mainWindow);
}

QString DocumentManager::getDocumentFileName(const QMainWindow &mainWindow) const
{
    QString result;
    DocumentInfo *documentInfo = getDocumentInfoFromObject(mainWindow);
    if (documentInfo != nullptr) {
        result = documentInfo->getName();
    }
    return result;
}

QString DocumentManager::getDocumentName(const QMainWindow &mainWindow) const
{
    QString result;
    result = getDocumentFileName(mainWindow);
    if (result.endsWith(FileUtils::SceneExtension)) {
        // chop including the dot (.) -> +1
        result.chop(FileUtils::SceneExtension.length() + 1);
    } else if (result.endsWith(FileUtils::TemplateExtension)) {
        // chop including the dot (.) -> +1
        result.chop(FileUtils::TemplateExtension.length() + 1);
    }
    return result;
}

void DocumentManager::setDocumentFileName(const QString &documentFileName, const QMainWindow &mainWindow)
{
    DocumentInfo *documentInfo = getDocumentInfoFromObject(mainWindow);
    if (documentInfo != nullptr) {
        documentInfo->setName(documentFileName);
        QAction *action = getWindowAction(documentInfo->getWindowId());
        if (action != nullptr) {
            action->setText(documentFileName);
        }
    }
}

QActionGroup &DocumentManager::getActionGroup() const
{
    return *d->windowActionGroup;
}

int DocumentManager::count() const
{
    return d->documentInfos.count();
}

int DocumentManager::getModifiedCount() const
{
    int result = 0;
    foreach (const DocumentInfo *documentInfo, d->documentInfos) {
        if (documentInfo->getScreenieScene()->isModified()) {
            result++;
        }
    }
    return result;
}

DocumentInfo::SaveStrategy DocumentManager::getSaveStrategy(const QMainWindow &mainWindow) const
{
    DocumentInfo::SaveStrategy result;
    const DocumentInfo *documentInfo = getDocumentInfoFromObject(mainWindow);
    if (documentInfo != nullptr) {
        result = documentInfo->getSaveStrategy();
    } else {
        result = DocumentInfo::Discard;
    }
    return result;
}

void DocumentManager::setSaveStrategy(const QMainWindow &mainWindow, DocumentInfo::SaveStrategy saveStrategy)
{
    DocumentInfo *documentInfo = getDocumentInfoFromObject(mainWindow);
    if (documentInfo != nullptr) {
        documentInfo->setSaveStrategy(saveStrategy);
    }
}

void DocumentManager::setSaveStrategyForAll(DocumentInfo::SaveStrategy saveStrategy)
{
    foreach (DocumentInfo *documentInfo, d->documentInfos) {
       documentInfo->setSaveStrategy(saveStrategy);
    }
}

void DocumentManager::addActiveDialogMainWindow(QMainWindow *mainWindow)
{
    d->activeDialogWindows.append(mainWindow);
}

void DocumentManager::removeActiveDialogMainWindow(QMainWindow *mainWindow)
{
    if (d->activeDialogWindows.contains(mainWindow)) {
        d->activeDialogWindows.removeOne(mainWindow);
    }
}

QMainWindow *DocumentManager::getRecentActiveDialogMainWindow() const
{
    QMainWindow *result;
    if (!d->activeDialogWindows.isEmpty()) {
        result = d->activeDialogWindows.last();
    } else {
        result = nullptr;
    }
    return result;
}

DocumentManager::CloseRequest DocumentManager::getCloseRequest()
{
    return DocumentManagerPrivate::closeRequest;
}

void DocumentManager::setCloseRequest(CloseRequest closeRequest)
{
    DocumentManagerPrivate::closeRequest = closeRequest;
}

bool DocumentManager::eventFilter(QObject *object, QEvent *event)
{
    bool result;
    const QMainWindow *mainWindow = qobject_cast<const QMainWindow *>(object);
    if (mainWindow != nullptr) {
        switch (event->type()) {
        case QEvent::ActivationChange:
            updateActionGroup(*mainWindow);
            result = false;
            break;
        default:
            result = QObject::eventFilter(object, event);
            break;
        }
    } else {
        result = QObject::eventFilter(object, event);
    }
    return result;
}

// protected

DocumentManager::~DocumentManager()
{
    delete d;
}

// private

DocumentManager::DocumentManager()
    : d(new DocumentManagerPrivate())
{
    frenchConnection();
}

void DocumentManager::frenchConnection()
{
    connect(&d->windowMapper, SIGNAL(mapped(int)),
            this, SLOT(activate(int)));
}

void DocumentManager::updateActionGroup(const QMainWindow &mainWindow)
{
    const DocumentInfo *documentInfo = getDocumentInfoFromObject(mainWindow);
    if (documentInfo != nullptr) {
        QAction *action = getWindowAction(documentInfo->getWindowId());
        if (action != nullptr) {
            action->setChecked(mainWindow.isActiveWindow());
        }
    }
}

QAction *DocumentManager::getWindowAction(int id) const
{
    QAction *result = nullptr;
    foreach (QAction *action, d->windowActionGroup->actions()) {
        if (action->data().toInt() == id) {
            result = action;
            break;
        }
    }
    return result;
}

DocumentInfo *DocumentManager::getDocumentInfoFromObject(const QObject &object) const
{
    DocumentInfo *result = nullptr;
    foreach (DocumentInfo *documentInfo, d->documentInfos) {
        if (documentInfo->getMainWindow()->objectName() == object.objectName()) {
            result = documentInfo;
            break;
        }
    }
    return result;
}

// private slots

void DocumentManager::remove(QObject *object)
{
    DocumentInfo *documentInfo = getDocumentInfoFromObject(*object);
    if (documentInfo != nullptr) {
        foreach (QAction *action, d->windowActionGroup->actions()) {
            if (action->data().toInt() == documentInfo->getWindowId()) {
                delete action;
                break;
            }
        }
        d->documentInfos.removeOne(documentInfo);
        emit changed();
    }
}

void DocumentManager::activate(int id) const
{
    foreach(DocumentInfo *documentInfo, d->documentInfos) {
        if (documentInfo->getWindowId() == id) {
            documentInfo->getMainWindow()->activateWindow();
            documentInfo->getMainWindow()->raise();
            break;
        }
    }
}





