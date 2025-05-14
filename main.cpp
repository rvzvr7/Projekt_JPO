#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include "mainwindow.h"
#include "logger/logger.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Projekt_JPO");
    QCoreApplication::setApplicationName("MonitorJakosciPowietrza");

    try {
        Logger::init();
        Logger::log("Uruchamianie aplikacji");

        MainWindow window;
        window.show();

        return app.exec();
    } catch (const std::exception& e) {
        Logger::logError("Błąd krytyczny: " + std::string(e.what()));
        return 1;
    }
}
