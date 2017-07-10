#-------------------------------------------------
#
# Project created by QtCreator 2017-04-13T22:21:06
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = HandyKaraoke
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    SettingsDialog.cpp \
    SongDatabase.cpp \
    Song.cpp \
    Midi/MidiFile.cpp \
    Midi/MidiEvent.cpp \
    Midi/MidiOut.cpp \
    Midi/Channel.cpp \
    Midi/MidiSynthesizer.cpp \
    Midi/MidiPlayer.cpp \
    Widgets/ChMx.cpp \
    Widgets/LyricsWidget.cpp \
    Widgets/RhythmWidget.cpp \
    Widgets/ChannelMixer.cpp \
    Midi/MidiHelper.cpp \
    Widgets/LEDVu.cpp \
    Widgets/WidgetDialogs/SettingVuDialog.cpp

HEADERS  += MainWindow.h \
    SettingsDialog.h \
    SongDatabase.h \
    Song.h \
    Midi/MidiFile.h \
    Midi/MidiEvent.h \
    Midi/MidiOut.h \
    Midi/Channel.h \
    Midi/MidiSynthesizer.h \
    Midi/MidiPlayer.h \
    Widgets/ChMx.h \
    Widgets/LyricsWidget.h \
    Widgets/RhythmWidget.h \
    Widgets/ChannelMixer.h \
    Midi/MidiHelper.h \
    Widgets/LEDVu.h \
    Widgets/WidgetDialogs/SettingVuDialog.h

FORMS    += MainWindow.ui \
    SettingsDialog.ui \
    Widgets/ChMx.ui \
    Widgets/RhythmWidget.ui \
    Widgets/ChannelMixer.ui \
    Widgets/WidgetDialogs/SettingVuDialog.ui


INCLUDEPATH += $$PWD/Widgets


win32 {
    LIBS += -lwinmm
    SOURCES += Midi/rtmidi/RtMidi.cpp
    HEADERS  += Midi/rtmidi/RtMidi.h
    INCLUDEPATH += $$PWD/Midi/rtmidi

    contains(QT_ARCH, i386) {
        message("32-bit")
        LIBS += -L$$PWD/BASS/bass24/ -lbass
        LIBS += -L$$PWD/BASS/bassmidi24/ -lbassmidi
    } else {
        message("64-bit")
        LIBS += -L$$PWD/BASS/bass24/x64/ -lbass
        LIBS += -L$$PWD/BASS/bassmidi24/x64/ -lbassmidi
    }
    INCLUDEPATH += $$PWD/BASS/bass24
    INCLUDEPATH += $$PWD/BASS/bassmidi24
}

unix:!macx {
    LIBS +=  -lrtmidi

    contains(QT_ARCH, i386) {
        message("32-bit")
        LIBS += -L$$PWD/BASS/bass24-linux/ -lbass
        LIBS += -L$$PWD/BASS/bassmidi24-linux/ -lbassmidi
    } else {
        message("64-bit")
        LIBS += -L$$PWD/BASS/bass24-linux/x64/ -lbass
        LIBS += -L$$PWD/BASS/bassmidi24-linux/x64/ -lbassmidi
    }
    INCLUDEPATH += $$PWD/BASS/bass24-linux
    INCLUDEPATH += $$PWD/BASS/bassmidi24-linux
}

macx {
    #LIBS += -lmidi2
}

RESOURCES += \
    icons.qrc
