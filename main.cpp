#include "MainWindow.h"

#include <QApplication>
#include <QSplashScreen>
#include <QDirIterator>
#include <QMetaType>
#include <QDebug>

#include <bass_vst.h>
#include "BASSFX/FX.h"


void makeVSTList(QSplashScreen *splash, MidiSynthesizer *synth);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); 

    QCoreApplication::setOrganizationName("HandyKaraoke");
    QCoreApplication::setOrganizationDomain("https://github.com/pie62/HandyKaraoke");
    QCoreApplication::setApplicationName("handy-karaoke");

    qRegisterMetaType<InstrumentType>("InstrumentType");

    qRegisterMetaType<QList<int>>("QList<int>");
    qRegisterMetaTypeStreamOperators<QList<int>>("QList<int>");

    qRegisterMetaType<QList<uint>>("QList<uint>");
    qRegisterMetaTypeStreamOperators<QList<uint>>("QList<uint>");

    qRegisterMetaType<QList<float>>("QList<float>");
    qRegisterMetaTypeStreamOperators<QList<float>>("QList<float>");

    qRegisterMetaType<QList<bool>>("QList<bool>");
    qRegisterMetaTypeStreamOperators<QList<bool>>("QList<bool>");

    qRegisterMetaType<QList<QList<float>>>("QList<QList<float>>");
    qRegisterMetaTypeStreamOperators<QList<QList<float>>>("QList<QList<float>>");

    QPixmap pixmap(":/Icons/App/splash.png");
    QSplashScreen splash(pixmap);
    splash.show();

    splash.showMessage("กำลังเริ่มโปรแกรม...", Qt::AlignBottom|Qt::AlignRight);
    qApp->processEvents();

    MainWindow w;

    #ifndef __linux__
    makeVSTList(&splash, w.midiPlayer()->midiSynthesizer());
    w.synthMixerDialog()->setVSTVendorMenu();
    w.synthMixerDialog()->setVSTToSynth();
    #endif

    w.show();

    splash.finish(&w);

    return a.exec();
}

void makeVSTList(QSplashScreen *splash, MidiSynthesizer *synth) {

    QDirIterator it("C:/Users/Noob/Downloads/VST", QStringList() << "*.DLL" << "*.dll",
                    QDir::Files|QDir::NoSymLinks, QDirIterator::Subdirectories);

    QMap<uint, VSTNamePath> vstList;

    while(it.hasNext()) {

        it.next();

        splash->showMessage("Reading : " + it.fileName(), Qt::AlignBottom|Qt::AlignRight);
        qApp->processEvents();

        BASS_VST_INFO info;
        if (!MidiSynthesizer::isVSTFile(it.filePath(), &info))
            continue;

        VSTNamePath v;
        v.uniqueID = info.uniqueID;
        v.vstName = info.effectName;
        v.vstvendor = info.vendorName;
        v.vstPath = it.filePath();

        vstList[info.uniqueID] = v;
    }

     synth->setVSTList(vstList);
}
