#include "MainWindow.h"

#include <QApplication>
#include <QSplashScreen>
#include <QDirIterator>
#include <QMetaType>
#include <QDebug>

#include <bass_vst.h>
#include "BASSFX/FX.h"


void registerMetaType();
void makeVSTList(QSplashScreen *splash, MidiSynthesizer *synth);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); 

    QCoreApplication::setOrganizationName("HandyKaraoke");
    QCoreApplication::setOrganizationDomain("https://github.com/pie62/HandyKaraoke");
    QCoreApplication::setApplicationName("handy-karaoke");

    registerMetaType();

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

void registerMetaType()
{
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
}

void makeVSTList(QSplashScreen *splash, MidiSynthesizer *synth)
{
    QSettings st;
    QStringList dirs = st.value("VSTDirs", QStringList()).toStringList();

    QStringList vstDirs;
    vstDirs << QDir::currentPath() + "/VST";
    vstDirs += dirs;

    QMap<uint, VSTNamePath> vstList;

    for (const QString &dir : dirs)
    {
        QDirIterator it(dir, QStringList() << "*.DLL" << "*.dll",
                        QDir::Files|QDir::NoSymLinks, QDirIterator::Subdirectories);

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
    }

     synth->setVSTList(vstList);
}
