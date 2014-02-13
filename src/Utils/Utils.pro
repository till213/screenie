include(../CommonLibrary.pri)
include(../External.pri)
include(Sources.pri)

TEMPLATE = lib
DEFINES += UTILS_EXPORT

macx {
    LIBS += -framework Cocoa
    INCLUDEPATH += /System/Library/Frameworks/AppKit.framework/Headers/
    QT += macextras
}


