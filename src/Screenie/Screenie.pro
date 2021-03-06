include(../Common.pri)
include(../External.pri)
include(Sources.pri)

TARGET = $${APP_NAME}
TEMPLATE = app

# OpenGL support later
# QT += opengl
QT += widgets

CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/../../bin/debug
    message(Building $$TARGET in debug mode)
} else {
    DESTDIR = $$PWD/../../bin/release
    message(Building $$TARGET in release mode)
}

macx {
    LIBS += -L$${DESTDIR}/$${APP_NAME}.app/Contents/Frameworks -framework Cocoa
    INCLUDEPATH += /System/Library/Frameworks/AppKit.framework/Headers/
} else {
    LIBS += -L$${DESTDIR}
}

win32 {
  LIBS += -lUtils$${VERSION_MAJ} \
          -lModel$${VERSION_MAJ} \
          -lResources$${VERSION_MAJ} \
          -lKernel$${VERSION_MAJ}
} else {
  LIBS += -lUtils \
          -lModel \
          -lResources \
          -lKernel
}

macx {
   ICON = res/ApplicationIcon.icns
   QMAKE_INFO_PLIST = $$PWD/Info.plist
   OTHER_FILES += $$PWD/Info.plist \
                  $$PWD/Screenie.sdef \
                  $$PWD/Screenie-Entitlements.plist
}

win32 {
   RC_FILE = res/Screenie.rc
}

OTHER_FILES += $$PWD/../../misc/LicenseTemplate.txt \
               $$PWD/../../Doxyfile \
               $$PWD/../Translation.pro \
               $$PWD/../translations/screenie_de.ts \
               $$PWD/res/Screenie.rc \
               $$PWD/res/ApplicationIcon.ico \
               $$PWD/res/SceneIcon.ico \
               $$PWD/res/TemplateIcon.ico

DISTFILES += \
    ../../LICENSE.GPL3 \
    ../../LICENSE.GPL2 \
    ../../README.md



