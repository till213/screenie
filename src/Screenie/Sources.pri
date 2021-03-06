DEPENDPATH += $$PWD/../Utils/src \
              $$PWD/../Resources/src \
              $$PWD/../Model/src \
              $$PWD/../Model/src/Dao \
              $$PWD/../Model/src/Dao/Xml \
              $$PWD/../Kernel/src \
              $$PWD/../Kernel/src/Clipboard \
              $$PWD/../Kernel/src/Dialogs \
              $$PWD/src/PlatformManager \
              $$PWD/src \
              $$PWD/GeneratedFiles

HEADERS += $$PWD/src/Main.h \
           $$PWD/src/MainWindow.h \
           $$PWD/src/ScreenieApplication.h \
           $$PWD/src/RecentFileMenu.h \
           $$PWD/src/PlatformManager/PlatformManager.h \
           $$PWD/src/PlatformManager/PlatformManagerFactory.h \
           $$PWD/src/PlatformManager/AbstractPlatformManager.h

SOURCES += $$PWD/src/Main.cpp \
           $$PWD/src/MainWindow.cpp \
           $$PWD/src/ScreenieApplication.cpp \
           $$PWD/src/RecentFileMenu.cpp \
           $$PWD/src/PlatformManager/PlatformManagerFactory.cpp \
           $$PWD/src/PlatformManager/AbstractPlatformManager.cpp

FORMS += $$PWD/ui/MainWindow.ui

if (macx) {
    HEADERS += $$PWD/src/PlatformManager/MacPlatformManager.h
    SOURCES += $$PWD/src/PlatformManager/PlatformManagerFactory_Mac.cpp \
               $$PWD/src/PlatformManager/PlatformManager_Mac.cpp
    OBJECTIVE_HEADERS += $$PWD/Script/ASOpenCommand.h
    OBJECTIVE_SOURCES += $$PWD/src/PlatformManager/MacPlatformManager.mm \
                         $$PWD/Script/ASOpenCommand.mm

} else:win32 {
    HEADERS += $$PWD/src/PlatformManager/WindowsPlatformManager.h \
               $$PWD/src/PlatformManager/Windows10ProxyStyle.h
    SOURCES += $$PWD/src/PlatformManager/PlatformManagerFactory_Windows.cpp \
               $$PWD/src/PlatformManager/PlatformManager_Windows.cpp \
               $$PWD/src/PlatformManager/WindowsPlatformManager.cpp \
               $$PWD/src/PlatformManager/Windows10ProxyStyle.cpp
} else:linux-* {
    HEADERS += $$PWD/src/PlatformManager/LinuxPlatformManager.h
    SOURCES += $$PWD/src/PlatformManager/PlatformManagerFactory_Linux.cpp \
               $$PWD/src/PlatformManager/PlatformManager_Linux.cpp \
               $$PWD/src/PlatformManager/LinuxPlatformManager.cpp
} else {
    HEADERS += $$PWD/src/PlatformManager/DefaultPlatformManager.h
    SOURCES += $$PWD/src/PlatformManager/PlatformManagerFactory_Default.cpp \
               $$PWD/src/PlatformManager/PlatformManager_Default.cpp \
               $$PWD/src/PlatformManager/DefaultPlatformManager.cpp
}


