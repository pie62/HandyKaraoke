QT -= gui

CONFIG += c++11 #console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp

win32 {
    contains(QT_ARCH, i386) {
        message("32-bit")
        TARGET = VSTChecker_x86

        LIBS += -L$$PWD/3rdParty/bass24/ -lbass
        LIBS += -L$$PWD/3rdParty/bass_vst24/ -lbass_vst

        RC_FILE = resources.rc
        QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01
    } else {
        message("64-bit")
        TARGET = VSTChecker_x64

        LIBS += -L$$PWD/3rdParty/bass24/x64/ -lbass
        LIBS += -L$$PWD/3rdParty/bass_vst24/x64/ -lbass_vst

        RC_FILE = resources_x64.rc
    }
    INCLUDEPATH += $$PWD/3rdParty/bass24
    INCLUDEPATH += $$PWD/3rdParty/bass_vst24
}

DISTFILES += \
    resources.rc \
    resources_x64.rc
