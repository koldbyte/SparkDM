#include "mainwindow.h"
#include <QApplication>
#include <QtPlugin>
#include <QFile>
static QString strPidFile;

void writePidFile()
{
    QFile file(strPidFile);
    if (!file.open(QIODevice::WriteOnly))
    {
        // << "Cannot write the pid file\n";
        exit(1);
    }
    file.write(QByteArray::number(100));
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("KoldByte");
    app.setOrganizationDomain("koldbyte.com");
    app.setApplicationName("Spark Download Manager");
    app.addLibraryPath(".");
    app.setApplicationDisplayName("Spark DM");
    app.setApplicationName("Spark DM");
    app.setApplicationVersion("1.0");
    app.setDesktopSettingsAware(true);
    app.setWindowIcon(QIcon(":icons/images/icon.png"));
    //if (!strPidFile.isEmpty())
    //        writePidFile();
    MainWindow w;
    w.show();
    return app.exec();
}



