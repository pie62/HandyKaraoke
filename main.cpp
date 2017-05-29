#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("HandyKaraoke");
    QCoreApplication::setOrganizationDomain("github.com/pie62/HandyKaraoke");
    QCoreApplication::setApplicationName("handy-karaoke");

    MainWindow w;
    w.show();

    return a.exec();
}
