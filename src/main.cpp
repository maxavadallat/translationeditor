#include <QDebug>
#include <QApplication>
#include <QTextCodec>
#include <QLocale>
#include <QSplashScreen>
#include <QPixmap>

#include "mainwindow.h"
#include "constants.h"

//==============================================================================
// Main
//==============================================================================
int main(int argc, char *argv[])
{
    // Disable QML Caching
    qputenv("QML_DISABLE_DISK_CACHE", "1");

    // Init Application
    QApplication a(argc, argv);

    // Init Pixmap
    QPixmap pixmap(":/resources/images/core/core_splash_generic.jpg");
    // Init Splash Screen
    QSplashScreen splash(pixmap);
    // Show Splash Screen
    splash.show();

    // Process Events
    a.processEvents();

#ifdef Q_OS_WIN
    //qDebug() << "codecs: " << QTextCodec::availableCodecs();

    qDebug() << "Locale: " << QLocale::system().name();

    // Create Codec
    QTextCodec* utfCodec = QTextCodec::codecForName("UTF-8");
    // Set Codec for Locale
    QTextCodec::setCodecForLocale(utfCodec);

#endif

    // Set Organization Name
    a.setOrganizationName(ORGANIZATION_NAME);
    // Set Organization Domain
    a.setOrganizationDomain(ORGANIZATION_DOMAIN);
    // Set Application Name
    a.setApplicationName(APPLICATION_NAME);
    // Set Application Display Name
    a.setApplicationDisplayName(DEFAULT_APPLICATION_DISPLAYNAME);
    // Set Application Version
    a.setApplicationVersion(APPLICATION_VERSION);

    // Init Main Window
    MainWindow w;

    // Get Last Window State
    int lwState = w.lastWindowState();

    // Switch Last Window State
    switch (lwState) {
        default:
        case Qt::WindowNoState:     w.show();           break;
        case Qt::WindowMaximized:   w.showMaximized();  break;
        case Qt::WindowFullScreen:  w.showFullScreen(); break;
    }

    // Finish Splash Screen
    splash.finish(&w);

    // Exec App
    return a.exec();
}
