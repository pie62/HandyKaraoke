#include "MainWindow.h"
#include <QApplication>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("HandyKaraoke");
    QCoreApplication::setOrganizationDomain("https://github.com/pie62/HandyKaraoke");
    QCoreApplication::setApplicationName("handy-karaoke");

    QPixmap pixmap(":/Icons/App/splash.png");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage("กำลังเปิดโปรแกรม...", Qt::AlignBottom|Qt::AlignRight);

    qApp->processEvents();

    MainWindow w;

    w.show();

    splash.finish(&w);

    return a.exec();
}
