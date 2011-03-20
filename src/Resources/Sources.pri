DEPENDPATH += $$PWD/../Utils/src \
              $$PWD/GeneratedFiles \
              $$PWD/src

if (macx) {
    RESOURCES += $$PWD/Mac/Resources.qrc
} else:win32 {
    RESOURCES += $$PWD/Windows/Resources.qrc
} else:linux {
    RESOURCES += $$PWD/Linux/Resources.qrc
} else {
    RESOURCES += $$PWD/Linux/Resources.qrc
}
