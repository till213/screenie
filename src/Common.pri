!linux {
  APP_NAME = Screenie
} else {
  # Prefer lower case app name on Linux
  APP_NAME = screenie
}

LANGUAGE = C++
CONFIG += qt warn_on thread c++11

# Should match with Utils/src/Version.h
VERSION=1.0.0

# qmake variable VER_MAJ seems to be empty, so
# applying the following "hack", inspired by:
# http://www.qtcentre.org/threads/17984-qmake-how-to-extract-number-from-variable
VERSIONS = $$split(VERSION, ".")
VERSION_MAJ = $$member(VERSIONS, 0)
VERSION_MIN = $$member(VERSIONS, 1)

macx {
  # On Mac we build 64bit Intel only
  CONFIG += x86_64
  CONFIG -= x86 ppc

  # QMAKE_MAC_SDK = macosx10.11
  QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.8
}

DEFINES += QT_NO_COMPAT

CONFIG(debug, debug|release) {
    OBJECTS_DIR    = obj/debug
    MOC_DIR        = GeneratedFiles/debug
    UI_DIR         = GeneratedFiles/debug
    INCLUDEPATH   += GeneratedFiles/debug
    DEFINES += DEBUG
} else {
    OBJECTS_DIR    = obj/release
    MOC_DIR        = GeneratedFiles/release
    UI_DIR         = GeneratedFiles/release
    INCLUDEPATH   += GeneratedFiles/release
}

!macx {
    target.path = $$PWD/../dist
    INSTALLS += target
}

unix:linux* {
    # Pick up libraries in the same path as the executable
    QMAKE_LFLAGS += -Wl,--enable-new-dtags,--rpath=\'\$\$ORIGIN\'
    #QMAKE_RPATHDIR = =\'\$\$ORIGIN\'

}

win32-g++ {

}

macx {
    # Create debug symbols in release mode
    QMAKE_CXXFLAGS_RELEASE += -gdwarf-2
    QMAKE_CFLAGS_RELEASE += -gdwarf-2
    QMAKE_OBJECTIVE_CFLAGS_RELEASE += -gdwarf-2
    # Enable ARC
    QMAKE_OBJECTIVE_CFLAGS += -fobjc-arc
}
