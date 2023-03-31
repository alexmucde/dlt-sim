#include "mainwindow.h"
#include "version.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("alexmucde");
    QCoreApplication::setOrganizationDomain("github.com");
    QCoreApplication::setApplicationName("DLTSim");
    QCoreApplication::setApplicationVersion(DLT_SIM_VERSION);

    MainWindow w;
    w.show();
    return a.exec();
}
