QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RESOURCES += \
    Resources.qrc

TRANSLATIONS += \
    Translations/HandyKaraoke_en_US.ts

win32 {
    #QT += winextras
    #LIBS += -lwinmm
    RC_FILE = WindowsResources.rc
}

SOURCES += \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    Version.h \
    MainWindow.h

FORMS += \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
