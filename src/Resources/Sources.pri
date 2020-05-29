DEPENDPATH += $$PWD/../Utils/src \
              $$PWD/GeneratedFiles \
              $$PWD/src

RESOURCES += $$PWD/CommonResources.qrc

if (macx) {
    RESOURCES += $$PWD/Mac/Resources.qrc
    OTHER_FILES += $$PWD/Mac/qt.conf
} else:win32 {
    RESOURCES += $$PWD/Windows/Resources.qrc
    OTHER_FILES += $$PWD/Windows/qt.conf
} else:linux-* {
    RESOURCES += $$PWD/Linux/Resources.qrc
} else {
    RESOURCES += $$PWD/Linux/Resources.qrc
}

