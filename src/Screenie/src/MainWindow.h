/* This file is part of the Screenie project.
   Screenie is a fancy screenshot composer.

   Copyright (C) 2014 Oliver Knoll <till.oliver.knoll@gmail.com>

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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QString>
#include <QWidget>
#include <QMainWindow>

class QWidget;
class QCloseEvent;
class QEvent;
class QAction;
class QStringList;

class ScreenieModelInterface;
class ScreenieScene;
class ScreeniePixmapItem;
class ScreenieControl;
class ScreenieGraphicsScene;
class Clipboard;
class PlatformManager;
class RecentFileMenu;
class SecurityToken;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    /*!
      * \brief Creates a new MainWindow.
      *
      * \param parent
      *        the parent QWidget
      */
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

    bool read(const QString &filePath, SecurityToken *securityToken = nullptr);

protected:
    virtual void closeEvent(QCloseEvent *event) override;
    virtual void changeEvent(QEvent *event) override;

private:
    Q_DISABLE_COPY(MainWindow)

    Ui::MainWindow *ui;
    RecentFileMenu *m_recentFileMenu;
    PlatformManager *m_platformManager;
    ScreenieGraphicsScene *m_screenieGraphicsScene;
    ScreenieScene *m_screenieScene;
    ScreenieControl *m_screenieControl;
    bool m_ignoreUpdateSignals;
    Clipboard *m_clipboard;
    QAction *m_minimizeWindowsAction;
    QAction *m_maximizeWindowsAction;
    QString m_errorMessage;

    void frenchConnection();

    void newScene(ScreenieScene &screenieScene);
    bool writeScene(const QString &filePath);
    bool writeTemplate(const QString &filePath);

    void initialiseUi();
    void updateTransformationUi();
    void updateReflectionUi();
    void updateColorUi();
    void updateEditActions();
    void updateViewActions();
    void updateTitle();

    void createScene();
    void updateScene(ScreenieScene &screenieScene);

    void askBeforeClose();
    void handleMultipleModifiedBeforeQuit();
    void saveBeforeClose();
    void saveAsBeforeClose();

    void restoreWindowGeometry();

    void updateDocumentManager();
    MainWindow *createMainWindow();

    inline bool isFilePathRequired() const;

    void storeWindowGeometry();

private slots:
    // File
    void on_newAction_triggered();
    void on_openAction_triggered();
    void on_saveAction_triggered();
    void on_saveAsAction_triggered();
    void on_saveAsTemplateAction_triggered();
    void on_exportAction_triggered();
    void on_closeAction_triggered();
    void on_quitAction_triggered();

    // Edit
    void on_cutAction_triggered();
    void on_copyAction_triggered();
    void on_pasteAction_triggered();
    void on_deleteAction_triggered();
    void on_selectAllAction_triggered();

    // Insert
    void on_addImageAction_triggered();
    void on_addTemplateAction_triggered();

    // View
    void on_showToolBarAction_toggled(bool enable);
    void on_showSidePanelAction_toggled(bool enable);
    void on_toggleFullScreenAction_triggered();

    // About
    void on_aboutAction_triggered();
    void on_aboutQtAction_triggered();

    void on_rotationSlider_valueChanged(int value);
    void on_distanceSlider_valueChanged(int value);
    void on_reflectionOffsetSlider_valueChanged(int value);
    void on_reflectionOpacitySlider_valueChanged(int value);
    void on_reflectionModeComboBox_currentIndexChanged(int index);

    void on_backgroundColorGroupBox_toggled(bool enable);
    void on_redSlider_valueChanged(int value);
    void on_greenSlider_valueChanged(int value);
    void on_blueSlider_valueChanged(int value);
    void on_redSpinBox_valueChanged(int value);
    void on_greenSpinBox_valueChanged(int value);
    void on_blueSpinBox_valueChanged(int value);
    void on_htmlBGColorLineEdit_editingFinished();
    void on_colorSelectorPushButton_clicked();

    void updateUi();
    void updateRecentFileMenu();
    void updateDefaultValues();
    void handleRecentFileSelected(const QString &filePath, SecurityToken *securityToken);
    void updateWindowMenu();

    void handleFileSaveAsSelected(const QString &filePath);
    void handleFileSaveAsTemplateSelected(const QString &filePath);
    void handleFileSaveAsBeforeCloseSelected(const QString &filePath);
    void handleDialogClosed();

    void handleExportFilePathSelected(const QString &filePath);
    void handleImagesSelected(const QStringList &filePaths);

    void handleAskBeforeClose(int answer);
    void handleToolBarVisibilityChanged(bool visible);

    // Shows the error message stored in m_errorMessage; the later is re-initialised
    // to an empty QString again
    void showError();
    void handleErrorClosed();
};

#endif // MAINWINDOW_H
