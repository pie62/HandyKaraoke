#include "MainWindow.h"
#include <QApplication>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPixmap *pixmap = new QPixmap(":/Icons/App/splash.png");
    QSplashScreen *splash = new QSplashScreen(*pixmap);
    splash->show();
    splash->showMessage("กำลังเปิดโปรแกรม...", Qt::AlignBottom|Qt::AlignRight);
    qApp->processEvents();

    QCoreApplication::setOrganizationName("HandyKaraoke");
    QCoreApplication::setOrganizationDomain("github.com/pie62/HandyKaraoke");
    QCoreApplication::setApplicationName("handy-karaoke");

    MainWindow w;

    w.show();

    splash->finish(&w);

    delete splash;
    delete pixmap;

    return a.exec();
}
