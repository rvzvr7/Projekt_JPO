#include "database.h"
#include <QDir>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDateTime>
#include <stdexcept>
#include "../logger/logger.h"

Database::Database() {
    ensureDirectoryExists(getDataPath());
}

Database::~Database() {
}

void Database::saveMeasurements(const Station& station, const Sensor& sensor,
                                const std::vector<Measurement>& measurements) {
    try {
        QJsonObject mainObject;

        QJsonObject stationObject;
        stationObject["id"] = station.id;
        stationObject["name"] = QString::fromStdString(station.name);
        stationObject["city"] = QString::fromStdString(station.city.name);
        stationObject["address"] = QString::fromStdString(station.addressStreet);
        mainObject["station"] = stationObject;

        QJsonObject sensorObject;
        sensorObject["id"] = sensor.id;
        sensorObject["paramName"] = QString::fromStdString(sensor.paramName);
        sensorObject["paramFormula"] = QString::fromStdString(sensor.paramFormula);
        sensorObject["paramCode"] = QString::fromStdString(sensor.paramCode);
        mainObject["sensor"] = sensorObject;

        QJsonArray measurementsArray;
        for (const auto& measurement : measurements) {
            QJsonObject measurementObject;
            measurementObject["date"] = QString::fromStdString(measurement.date);
            if (measurement.value.has_value()) {
                measurementObject["value"] = measurement.value.value();
            } else {
                measurementObject["value"] = QJsonValue::Null;
            }
            measurementsArray.append(measurementObject);
        }
        mainObject["measurements"] = measurementsArray;

        QJsonDocument document(mainObject);
        QByteArray jsonData = document.toJson(QJsonDocument::Indented);

        std::string filename = createFilename(sensor.id);
        QFile file(QString::fromStdString(filename));

        if (!file.open(QIODevice::WriteOnly)) {
            throw std::runtime_error("Nie można otworzyć pliku do zapisu: " + filename);
        }

        if (file.write(jsonData) == -1) {
            throw std::runtime_error("Błąd podczas zapisu do pliku: " + filename);
        }

        file.close();
        Logger::log("Zapisano dane do pliku: " + filename);
    } catch (const std::exception& e) {
        Logger::logError("Błąd podczas zapisywania danych: " + std::string(e.what()));
        throw;
    }
}

std::vector<Measurement> Database::loadMeasurements(int sensorId, const QDate& startDate, const QDate& endDate) {
    std::vector<Measurement> measurements;

    try {
        std::string filename = createFilename(sensorId);
        QFile file(QString::fromStdString(filename));

        if (!file.exists()) {
            Logger::log("Plik nie istnieje: " + filename);
            return measurements;
        }

        if (!file.open(QIODevice::ReadOnly)) {
            throw std::runtime_error("Nie można otworzyć pliku do odczytu: " + filename);
        }

        QByteArray jsonData = file.readAll();
        file.close();

        QJsonDocument document = QJsonDocument::fromJson(jsonData);
        if (document.isNull() || !document.isObject()) {
            throw std::runtime_error("Nieprawidłowy format pliku JSON: " + filename);
        }

        QJsonObject mainObject = document.object();
        QJsonArray measurementsArray = mainObject["measurements"].toArray();

        for (const auto& value : measurementsArray) {
            QJsonObject obj = value.toObject();

            QString dateStr = obj["date"].toString();
            QDateTime dateTime = QDateTime::fromString(dateStr, Qt::ISODate);

            if (dateTime.date() >= startDate && dateTime.date() <= endDate) {
                Measurement measurement;
                measurement.date = dateStr.toStdString();
                if (!obj["value"].isNull()) {
                    measurement.value = obj["value"].toDouble();
                }
                measurements.push_back(measurement);
            }
        }

        Logger::log("Wczytano dane z pliku: " + filename);
    } catch (const std::exception& e) {
        Logger::logError("Błąd podczas wczytywania danych: " + std::string(e.what()));
        throw;
    }

    return measurements;
}

std::vector<Station> Database::loadStations() {
    return std::vector<Station>();
}

std::vector<Sensor> Database::loadSensors(int stationId) {
    return std::vector<Sensor>();
}

std::string Database::getDataPath() const {
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(appDataPath);
    dir.mkpath("data");
    return (appDataPath + "/data").toStdString();
}

std::string Database::createFilename(int sensorId) const {
    return getDataPath() + "/sensor_" + std::to_string(sensorId) + ".json";
}

void Database::ensureDirectoryExists(const std::string& path) const {
    QDir dir(QString::fromStdString(path));
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            throw std::runtime_error("Nie można utworzyć katalogu: " + path);
        }
    }
}
