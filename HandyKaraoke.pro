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
    Dialogs/MapSoundfontDialog.cpp \
    Widgets/Slider.cpp \
    BASSFX/Equalizer15BandFX.cpp \
    Widgets/SwitchButton.cpp \
    BASSFX/Equalizer31BandFX.cpp \
    Dialogs/Equalizer31BandDialog.cpp \
    BASSFX/ReverbFX.cpp \
    Dialogs/ReverbDialog.cpp \
    BASSFX/ChorusFX.cpp \
    Dialogs/ChorusDialog.cpp \
    Widgets/InstCh.cpp \
    Dialogs/SynthMixerDialog.cpp \
    Dialogs/SettingVuDialog.cpp \
    Widgets/SongDetail.cpp \
    Widgets/Detail.cpp \
    Dialogs/AboutDialog.cpp

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
    Dialogs/MapSoundfontDialog.h \
    Dialogs/ComboBoxItem.h \
    Widgets/Slider.h \
    BASSFX/Equalizer15BandFX.h \
    Widgets/SwitchButton.h \
    BASSFX/Equalizer31BandFX.h \
    Dialogs/Equalizer31BandDialog.h \
    BASSFX/ReverbFX.h \
    Dialogs/ReverbDialog.h \
    BASSFX/ChorusFX.h \
    Dialogs/ChorusDialog.h \
    Widgets/InstCh.h \
    Dialogs/SynthMixerDialog.h \
    Dialogs/SettingVuDialog.h \
    Widgets/SongDetail.h \
    Widgets/Detail.h \
    Dialogs/AboutDialog.h

FORMS    += MainWindow.ui \
    SettingsDialog.ui \
    Widgets/ChMx.ui \
    Widgets/RhythmWidget.ui \
    Widgets/ChannelMixer.ui \
    Dialogs/MapSoundfontDialog.ui \
    Dialogs/Equalizer31BandDialog.ui \
    Dialogs/ReverbDialog.ui \
    Dialogs/ChorusDialog.ui \
    Widgets/InstCh.ui \
    Dialogs/SynthMixerDialog.ui \
    Dialogs/SettingVuDialog.ui \
    Widgets/SongDetail.ui \
    Widgets/Detail.ui \
    Dialogs/AboutDialog.ui


INCLUDEPATH += $$PWD/Widgets


win32 {
    LIBS += -lwinmm
    SOURCES += Midi/rtmidi/RtMidi.cpp
    HEADERS  += Midi/rtmidi/RtMidi.h
    INCLUDEPATH += $$PWD/Midi/rtmidi

    contains(QT_ARCH, i386) {
        message("32-bit")
        LIBS += -LD:/Projects/QtProjects/BASS/bass24/ -lbass
        LIBS += -LD:/Projects/QtProjects/BASS/bassmidi24/ -lbassmidi
    } else {
        message("64-bit")
        LIBS += -LD:/Projects/QtProjects/BASS/bass24/x64/ -lbass
        LIBS += -LD:/Projects/QtProjects/BASS/bassmidi24/x64/ -lbassmidi
    }
    INCLUDEPATH += D:/Projects/QtProjects/BASS/bass24
    INCLUDEPATH += D:/Projects/QtProjects/BASS/bassmidi24

    RC_ICONS = icon.ico
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

#DEFINES += _ATL_XP_TARGETING
#DEFINES += PSAPI_VERSION=1
#QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01
