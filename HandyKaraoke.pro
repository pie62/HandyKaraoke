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
    Dialogs/SettingVuDialog.cpp \
    Widgets/SongDetail.cpp \
    Widgets/Detail.cpp \
    Dialogs/AboutDialog.cpp \
    Utils.cpp \
    Widgets/PlaybackButton.cpp \
    Widgets/FaderSlider.cpp \
    Widgets/VSTLabel.cpp \
    Midi/HNKFile.cpp \
    BASSFX/FX.cpp \
    Widgets/CustomFXList.cpp \
    Dialogs/BusDialog.cpp \
    Dialogs/SynthMixerDialog.cpp \
    Dialogs/SecondMonitorDialog.cpp \
    Midi/MidiSequencer.cpp \
    Midi/MidiPlayer.cpp \
    Dialogs/MapChannelDialog.cpp \
    Widgets/ChMxComboBox.cpp \
    BASSFX/AutoWahFX.cpp \
    BASSFX/CompressorFX.cpp \
    BASSFX/DistortionFX.cpp \
    BASSFX/EchoFX.cpp \
    FXDialogs/EQ31BandDialog.cpp \
    FXDialogs/EQ15BandDialog.cpp \
    FXDialogs/ChorusFXDialog.cpp \
    FXDialogs/ReverbFXDialog.cpp \
    FXDialogs/AutoWahFXDialog.cpp \
    FXDialogs/CompressorFXDialog.cpp \
    FXDialogs/DistortionFXDialog.cpp \
    FXDialogs/EchoFXDialog.cpp \
    Widgets/PlaylistWidget.cpp \
    Dialogs/DialogHelper.cpp \
    Dialogs/SpeakerDialog.cpp \
    Dialogs/Chorus2Dialog.cpp \
    BASSFX/Chorus2FX.cpp \
    BASSFX/Reverb2FX.cpp \
    Dialogs/Reverb2Dialog.cpp \
    Dialogs/DeleteSongDialog.cpp

HEADERS  += MainWindow.h \
    SettingsDialog.h \
    SongDatabase.h \
    Song.h \
    Midi/MidiFile.h \
    Midi/MidiEvent.h \
    Midi/MidiOut.h \
    Midi/Channel.h \
    Midi/MidiSynthesizer.h \
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
    Dialogs/AboutDialog.h \
    Utils.h \
    Widgets/PlaybackButton.h \
    Widgets/FaderSlider.h \
    Widgets/VSTLabel.h \
    Midi/HNKFile.h \
    BASSFX/FX.h \
    Widgets/CustomFXList.h \
    Dialogs/BusDialog.h \
    Dialogs/SecondMonitorDialog.h \
    Midi/MidiSequencer.h \
    Midi/MidiPlayer.h \
    DrumPadsKey.h \
    version.h \
    Dialogs/MapChannelDialog.h \
    Widgets/ChMxComboBox.h \
    BASSFX/AutoWahFX.h \
    BASSFX/CompressorFX.h \
    BASSFX/DistortionFX.h \
    BASSFX/EchoFX.h \
    FXDialogs/EQ31BandDialog.h \
    FXDialogs/EQ15BandDialog.h \
    FXDialogs/ChorusFXDialog.h \
    FXDialogs/ReverbFXDialog.h \
    FXDialogs/AutoWahFXDialog.h \
    FXDialogs/CompressorFXDialog.h \
    FXDialogs/DistortionFXDialog.h \
    FXDialogs/EchoFXDialog.h \
    Config.h \
    Widgets/PlaylistWidget.h \
    Dialogs/DialogHelper.h \
    Dialogs/SpeakerDialog.h \
    Dialogs/Chorus2Dialog.h \
    BASSFX/Chorus2FX.h \
    BASSFX/Reverb2FX.h \
    Dialogs/Reverb2Dialog.h \
    Midi/HNKFileComp.h \
    Dialogs/DeleteSongDialog.h

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
    Dialogs/AboutDialog.ui \
    Widgets/VSTLabel.ui \
    Dialogs/BusDialog.ui \
    Dialogs/SecondMonitorDialog.ui \
    Dialogs/MapChannelDialog.ui \
    FXDialogs/EQ31BandDialog.ui \
    FXDialogs/EQ15BandDialog.ui \
    FXDialogs/ChorusFXDialog.ui \
    FXDialogs/ReverbFXDialog.ui \
    FXDialogs/AutoWahFXDialog.ui \
    FXDialogs/CompressorFXDialog.ui \
    FXDialogs/DistortionFXDialog.ui \
    FXDialogs/EchoFXDialog.ui \
    Widgets/PlaylistWidget.ui \
    Dialogs/SpeakerDialog.ui \
    Dialogs/Chorus2Dialog.ui \
    Dialogs/Reverb2Dialog.ui \
    Dialogs/DeleteSongDialog.ui


INCLUDEPATH += $$PWD/Widgets

TRANSLATIONS = languages/en.ts


win32 {
    QT += winextras
    LIBS += -lwinmm
    RC_FILE = resources.rc

    SOURCES += Midi/rtmidi/RtMidi.cpp \
        Dialogs/VSTDirsDialog.cpp \
        Dialogs/VSTDialog.cpp \
        BASSFX/VSTFX.cpp

    HEADERS  += Midi/rtmidi/RtMidi.h \
        Dialogs/VSTDirsDialog.h \
        Dialogs/VSTDialog.h \
        BASSFX/VSTFX.h

    FORMS += Dialogs/VSTDirsDialog.ui

    INCLUDEPATH += $$PWD/Midi/rtmidi \
        $$PWD/3rdParty/WinSparkle/include

    contains(QT_ARCH, i386) {
        message("32-bit")
        LIBS += -L$$PWD/BASS/bass24/ -lbass
        LIBS += -L$$PWD/BASS/bassmidi24/ -lbassmidi
        LIBS += -L$$PWD/BASS/bass_fx24/ -lbass_fx
        LIBS += -L$$PWD/BASS/bassmix24/ -lbassmix
        LIBS += -L$$PWD/BASS/bass_vst24/ -lbass_vst
        LIBS += -L$$PWD/3rdParty/WinSparkle/x86/ -lWinSparkle

        #DEFINES += _ATL_XP_TARGETING
        #DEFINES += PSAPI_VERSION=1
        QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01
    } else {
        message("64-bit")
        LIBS += -L$$PWD/BASS/bass24/x64/ -lbass
        LIBS += -L$$PWD/BASS/bassmidi24/x64/ -lbassmidi
        LIBS += -L$$PWD/BASS/bass_fx24/x64/ -lbass_fx
        LIBS += -L$$PWD/BASS/bassmix24/x64/ -lbassmix
        LIBS += -L$$PWD/BASS/bass_vst24/x64/ -lbass_vst
        LIBS += -L$$PWD/3rdParty/WinSparkle/x64/ -lWinSparkle
    }
    INCLUDEPATH += $$PWD/BASS/bass24
    INCLUDEPATH += $$PWD/BASS/bassmidi24
    INCLUDEPATH += $$PWD/BASS/bass_fx24
    INCLUDEPATH += $$PWD/BASS/bassmix24
    INCLUDEPATH += $$PWD/BASS/bass_vst24

    #RC_ICONS = icon.ico
}

unix:!macx {
    LIBS +=  -lrtmidi
    RESOURCES += \
        fonts.qrc

    contains(QT_ARCH, i386) {
        message("32-bit")
        LIBS += -L$$PWD/BASS/bass24-linux/ -lbass
        LIBS += -L$$PWD/BASS/bassmidi24-linux/ -lbassmidi
        LIBS += -L$$PWD/BASS/bass_fx24-linux/ -lbass_fx
        LIBS += -L$$PWD/BASS/bassmix24-linux/ -lbassmix
    } else {
        message("64-bit")
        LIBS += -L$$PWD/BASS/bass24-linux/x64/ -lbass
        LIBS += -L$$PWD/BASS/bassmidi24-linux/x64/ -lbassmidi
        LIBS += -L$$PWD/BASS/bass_fx24-linux/x64/ -lbass_fx
        LIBS += -L$$PWD/BASS/bassmix24-linux/x64/ -lbassmix
    }
    INCLUDEPATH += $$PWD/BASS/bass24-linux
    INCLUDEPATH += $$PWD/BASS/bassmidi24-linux
    INCLUDEPATH += $$PWD/BASS/bass_fx24-linux
    INCLUDEPATH += $$PWD/BASS/bassmix24-linux
}

macx {
    #LIBS += -lmidi2
}

RESOURCES += \
    icons.qrc \
    app.qrc

#DEFINES += _ATL_XP_TARGETING
#DEFINES += PSAPI_VERSION=1
#QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01

DISTFILES += \
    resources.rc
