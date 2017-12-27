#include "MainWindow.h"

#include <QApplication>
#include <QSplashScreen>
#include <QDirIterator>
#include <QMetaType>

#include "BASSFX/FX.h"

void registerMetaType();

#ifndef __linux__
void makeVSTList(QSplashScreen *splash, MidiSynthesizer *synth);
#endif


int main(int argc, char *argv[])
{
    QApplication a(argc, argv); 

    QCoreApplication::setApplicationName("handy-karaoke");
    QCoreApplication::setApplicationVersion("1.3.0");
    QCoreApplication::setOrganizationName("HandyKaraoke");
    QCoreApplication::setOrganizationDomain("https://github.com/pie62/HandyKaraoke");

    registerMetaType();

    QPixmap *pixmap = new QPixmap(":/Icons/App/splash.png");
    QSplashScreen *splash = new QSplashScreen(*pixmap);
    splash->show();
    qApp->processEvents();

    splash->showMessage("กำลังเริ่มโปรแกรม...", Qt::AlignBottom|Qt::AlignRight);
    qApp->processEvents();

    MainWindow w;

    #ifndef __linux__
    makeVSTList(splash, w.midiPlayer()->midiSynthesizer());
    w.synthMixerDialog()->setVSTVendorMenu();
    #endif
    w.synthMixerDialog()->setVSTToSynth();

    w.show();

    splash->finish(&w);

    delete splash;
    delete pixmap;

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


#ifndef __linux__

void makeVSTList(QSplashScreen *splash, MidiSynthesizer *synth)
{
    QSettings st;
    QStringList dirs = st.value("VSTDirs", QStringList()).toStringList();

    QStringList vstDirs;
    vstDirs << QDir::currentPath() + "/VST";
    vstDirs += dirs;

    QMap<uint, VSTNamePath> vstList;

    for (const QString &dir : vstDirs)
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

#endif
