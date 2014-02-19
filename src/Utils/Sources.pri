DEPENDPATH += $$PWD/GeneratedFiles \
              $$PWD/src

HEADERS += $$PWD/src/UtilsLib.h \
           $$PWD/src/PaintTools.h \
           $$PWD/src/Settings.h \
           $$PWD/src/Version.h \
           $$PWD/src/SizeFitter.h \
           $$PWD/src/FileUtils.h \
           $$PWD/src/RecentFile.h \
           $$PWD/src/SecurityToken.h

SOURCES += $$PWD/src/PaintTools.cpp \
           $$PWD/src/Settings.cpp \
           $$PWD/src/Version.cpp \
           $$PWD/src/SizeFitter.cpp \
           $$PWD/src/FileUtils.cpp \
           $$PWD/src/RecentFile.cpp

if (macx) {
    OBJECTIVE_SOURCES += $$PWD/src/SecurityToken_Mac.mm
} else {
    SOURCES += $$PWD/src/SecurityToken_Default.cpp
}

