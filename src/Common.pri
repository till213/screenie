linux|freebsd {
  # Prefer lower case app name on Linux and FreeBSD
  APP_NAME = screenie
} else {  
  APP_NAME = Screenie
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
    # For a reason yet to figure out make on FreeBSD does not seem to like
    # subfolders named "obj": even the mere existence of a subfolder called "obj"
    # makes it claim that "source file not found" (even without specifying any
    # OBJECTS_DIR)
    OBJECTS_DIR    = objects/debug
    MOC_DIR        = GeneratedFiles/debug
    UI_DIR         = GeneratedFiles/debug
    INCLUDEPATH   += GeneratedFiles/debug
    DEFINES += DEBUG
} else {
    OBJECTS_DIR    = objects/release
    MOC_DIR        = GeneratedFiles/release
    UI_DIR         = GeneratedFiles/release
    INCLUDEPATH   += GeneratedFiles/release
}

linux|freebsd {
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

freebsd {
  # http://www.unix.com/bsd/254307-freebsd-usr-bin-ld-not-looking-usr-local-lib.html
  # "/usr/local/lib is a conventional place to add local libraries, but it is not a
  #  default location for any FreeBSD supplied libraries. And, it is not a directory
  # that is searched by default by ld."
  QMAKE_LIBDIR += /usr/local/lib
}

