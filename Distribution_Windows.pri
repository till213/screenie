#include(Common.pri)

QT_PLUGINS_DIR = $$[QT_INSTALL_PLUGINS]

CONFIG(debug, debug|release) {
    DIST_DIR = dist
    QT_CORE = Qt5Cored.dll
    QT_GUI = Qt5Guid.dll
    QT_WIDGETS = Qt5Widgetsd.dll
    QT_IMAGE_JPEG = qjpegd.dll
    QT_IMAGE_TIFF = qtiffd.dll
    QT_IMAGE_GIF = qgifd.dll
    QT_IMAGE_TGA = qtgad.dll
    QT_IMAGE_BMP = qwbmpd.dll
    QT_PLATFORM_WIN = qwindowsd.dll
    message(Installer: DEBUG $${APP_NAME} with $$[QT_INSTALL_BINS] and $$[QT_INSTALL_PLUGINS])
} else {
    DIST_DIR = dist
    QT_CORE = Qt5Core.dll
    QT_GUI = Qt5Gui.dll
    QT_WIDGETS = Qt5Widgets.dll
    QT_IMAGE_JPEG = qjpeg.dll
    QT_IMAGE_TIFF = qtiff.dll
    QT_IMAGE_GIF = qgif.dll
    QT_IMAGE_TGA = qtga.dll
    QT_IMAGE_BMP = qwbmp.dll
    QT_PLATFORM_WIN = qwindows.dll
    message(Installer: RELEASE $${APP_NAME} with $$[QT_INSTALL_BINS] and $$[QT_INSTALL_PLUGINS])
}

#
# INSTALLS
#

qt_lib.files = $$[QT_INSTALL_BINS]/$${QT_CORE} \
               $$[QT_INSTALL_BINS]/$${QT_GUI} \
               $$[QT_INSTALL_BINS]/$${QT_WIDGETS}
qt_lib.path = $${DIST_DIR}

qt_img_plugins.files = $$[QT_INSTALL_PLUGINS]/imageformats/$${QT_IMAGE_JPEG} \
                       $$[QT_INSTALL_PLUGINS]/imageformats/$${QT_IMAGE_TIFF} \
                       $$[QT_INSTALL_PLUGINS]/imageformats/$${QT_IMAGE_GIF} \
                       $$[QT_INSTALL_PLUGINS]/imageformats/$${QT_IMAGE_TGA} \
                       $$[QT_INSTALL_PLUGINS]/imageformats/$${QT_IMAGE_BMP}
qt_img_plugins.path = $${DIST_DIR}/imageformats

qt_platform_plugins.files = $$[QT_INSTALL_PLUGINS]/platforms/$${QT_PLATFORM_WIN}
qt_platform_plugins.path = $${DIST_DIR}/platforms

# libstdc++-6.dll - also refer to QTBUG-16372
support_lib.files = $$[QT_INSTALL_BINS]/libstd~1.dll \
                    $$[QT_INSTALL_BINS]/libgcc_s_dw2-1.dll \
                    $$[QT_INSTALL_BINS]/libwinpthread-1.dll \
                    $$[QT_INSTALL_BINS]/icuin51.dll $$[QT_INSTALL_BINS]/icuuc51.dll \
                    $$[QT_INSTALL_BINS]/icudt51.dll
support_lib.path = $${DIST_DIR}

INSTALLS += qt_lib qt_img_plugins qt_platform_plugins support_lib

#
# Installer
#

# Make sure the WiX installer tools candle.exe and light.exe are in the PATH
# e.g. set PATH="c:\Programme\Windows Installer XML v3.5\bin";%PATH%
CONFIG(debug, debug|release) {
  message("Installer: skipping this build step. Target 'Installer' can only be done in RELEASE mode.")
} else {
  installer.commands = candle.exe -nologo "Installer/Windows/Screenie.wxs" -out "Installer/Windows/Screenie.wixobj"  -ext WixUIExtension &&
  installer.commands += light.exe -nologo "Installer/Windows/Screenie.wixobj" -out "Installer/Windows/Screenie-$${VERSION}.msi"  -ext WixUIExtension
}

OTHER_FILES += Installer/Windows/Screenie.wxs


