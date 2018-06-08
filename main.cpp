#include "MainWindow.h"

#include <QApplication>
#include <QSplashScreen>
#include <QDirIterator>
#include <QMetaType>
#include <QStyleFactory>

#include "BASSFX/VSTFX.h"
#include "version.h"
#include "Config.h"
#include "Utils.h"

#ifdef __linux__
#include <QFontDatabase>
#endif

void registerMetaType();

#ifndef __linux__
void loadVSTi(QSplashScreen *splash, MidiSynthesizer *synth);
void makeVSTList(QSplashScreen *splash, MidiSynthesizer *synth);
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); 

    QCoreApplication::setApplicationName("Handy Karaoke");
    QCoreApplication::setApplicationVersion(VER_FILEVERSION_STR);
    QCoreApplication::setOrganizationName("Handy Karaoke");
    QCoreApplication::setOrganizationDomain("https://github.com/pie62/HandyKaraoke");

    registerMetaType();

    QPixmap *pixmap = new QPixmap(":/Icons/App/splash.png");
    QSplashScreen *splash = new QSplashScreen(*pixmap);
    splash->show();
    qApp->processEvents();

    splash->showMessage("กำลังเริ่มโปรแกรม...", Qt::AlignBottom|Qt::AlignRight);
    qApp->processEvents();

    { // Config Dir
        QDir dir(ALL_DATA_DIR_PATH);
        if (!dir.exists())
            dir.mkpath(ALL_DATA_DIR_PATH);

        dir.setPath(CONFIG_DIR_PATH);
        if (!dir.exists())
            dir.mkpath(CONFIG_DIR_PATH);
    }

    { // set style
        QSettings *s = new QSettings("Style.conf", QSettings::IniFormat);

        #ifdef __linux__
        bool useFusion = s->value("Fusion", true).toBool();
        #else
        bool useFusion = s->value("Fusion", false).toBool();
        #endif

        delete s;

        if (useFusion)
            a.setStyle(QStyleFactory::create("Fusion"));
    }


    // Add font for linux
    #ifdef __linux__
    QFontDatabase::addApplicationFont(":/Fonts/THSarabunNew/THSarabunNew Bold.ttf");
    #endif
    //----------------------------------

    MainWindow w;
    w.setWindowIcon(QIcon(":/Icons/App/icon.png"));

    #ifndef __linux__
    loadVSTi(splash, w.midiPlayer()->midiSynthesizer());
    makeVSTList(splash, w.midiPlayer()->midiSynthesizer());
    w.synthMixerDialog()->setVSTVendorMenu();
    #endif
    w.synthMixerDialog()->setFXToSynth();

    w.show();

    splash->finish(&w);

    delete splash;
    delete pixmap;

    return a.exec();
}

void registerMetaType()
{
    qRegisterMetaType<InstrumentType>("InstrumentType");

    //<QList<int>>("QList<int>");
    qRegisterMetaTypeStreamOperators<QList<int>>("QList<int>");

    //qRegisterMetaType<QList<uint>>("QList<uint>");
    qRegisterMetaTypeStreamOperators<QList<uint>>("QList<uint>");

    //qRegisterMetaType<QList<float>>("QList<float>");
    qRegisterMetaTypeStreamOperators<QList<float>>("QList<float>");

    //qRegisterMetaType<QList<bool>>("QList<bool>");
    qRegisterMetaTypeStreamOperators<QList<bool>>("QList<bool>");

    //qRegisterMetaType<QList<QList<float>>>("QList<QList<float>>");
    qRegisterMetaTypeStreamOperators<QList<QList<float>>>("QList<QList<float>>");
}


#ifndef __linux__

void loadVSTi(QSplashScreen *splash, MidiSynthesizer *synth)
{
    QSettings st(CONFIG_SYNTH_FILE_PATH, QSettings::IniFormat);

    st.beginReadArray("VSTiGroup");
    for (int i=0; i<4; i++)
    {
        st.setArrayIndex(i);

        QString      filePath    = st.value("VstiFilePath", "").toString();
        int          program     = st.value("VstiPrograms", 0).toInt();
        QList<float> params      = st.value("VstiParams").value<QList<float>>();

        if (filePath == "")
            continue;

        splash->showMessage("กำลังโหลด : " + QFileInfo(filePath).fileName(), Qt::AlignBottom|Qt::AlignRight);
        qApp->processEvents();

        DWORD vsti = synth->setVSTiFile(i, filePath);

        if (vsti != 0)
        {
            BASS_VST_SetProgram(vsti, program);
            FX::setVSTParams(vsti, params);
        }
    }
    st.endArray();
}

void makeVSTList(QSplashScreen *splash, MidiSynthesizer *synth)
{
    QSettings st(CONFIG_SYNTH_FILE_PATH, QSettings::IniFormat);
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

            splash->showMessage("กำลังตรวจสอบ : " + it.fileName(), Qt::AlignBottom|Qt::AlignRight);
            qApp->processEvents();

            /*
            BASS_VST_INFO info;
            if (!VSTFX::isVSTFile(it.filePath(), &info))
                continue;

            VSTNamePath v;
            v.uniqueID = info.uniqueID;
            v.vstName = info.effectName;
            v.vstvendor = info.vendorName;
            v.vstPath = it.filePath();
            */



            VSTNamePath info;

            if (!Utils::vstInfo(it.filePath(), &info))
                continue;

            vstList[info.uniqueID] = info;
        }
    }

    synth->setVSTList(vstList);
}

#endif
