QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Players/Player.cpp \
    Widgets/Background.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    MainWindow.h \
    Players/Player.h \
    Widgets/Background.h

FORMS += \
    MainWindow.ui

TRANSLATIONS += \
    HandyKaraoke_en_US.ts


win32 {
    contains(QT_ARCH, i386) {
        # Windows 32-bit

        # VLC 32
        INCLUDEPATH += $$PWD/ThirdParty/vlc-3.0.8-win32/sdk/include
        LIBS += -L$$PWD/ThirdParty/vlc-3.0.8-win32/sdk/lib/ -llibvlc -llibvlccore
    } else {
        # Windows 64-bit

        # VLC 64
        INCLUDEPATH += $$PWD/ThirdParty/vlc-3.0.8-win64/sdk/include
        LIBS += -L$$PWD/ThirdParty/vlc-3.0.8-win64/sdk/lib/ -llibvlc -llibvlccore
    }
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
