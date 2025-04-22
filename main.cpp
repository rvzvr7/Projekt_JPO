#include <QApplication>
#include "mainwindow.h"
#include "logger/logger.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

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
