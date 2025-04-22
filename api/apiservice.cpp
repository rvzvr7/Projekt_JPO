#include "apiservice.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QtNetwork/QNetworkRequest>
#include "../logger/logger.h"

ApiService::ApiService(QObject *parent)
    : QObject(parent),
    networkManager(new QNetworkAccessManager(this))
{
    baseUrl = QUrl("https://api.gios.gov.pl/pjp-api/rest");
}

ApiService::~ApiService() {
    // networkManager jest usuwany automatycznie, gdyż jest dzieckiem this.
}

void ApiService::fetchStations(const QString& cityFilter) {
    QUrl url = baseUrl;
    url.setPath(baseUrl.path() + "/station/findAll");

    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply, cityFilter]() {
        if (reply->error() == QNetworkReply::NoError) {
            handleStationsReply(reply, cityFilter);  // ← przekazujemy filtr
        } else {
            emit errorOccurred(reply->errorString());
        }
        reply->deleteLater();
    });
}

void ApiService::fetchStationDetails(int stationId) {
    QUrl url = baseUrl;
    url.setPath(baseUrl.path() + "/station/sensors/" + QString::number(stationId));

    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply, stationId]() {
        if (reply->error() == QNetworkReply::NoError) {
            handleSensorsReply(reply, stationId);
        } else {
            emit errorOccurred(reply->errorString());
        }
        reply->deleteLater();
    });
}

void ApiService::fetchSensorMeasurements(int sensorId, const QDate& startDate, const QDate& endDate) {
    QUrl url = baseUrl;
    url.setPath(baseUrl.path() + "/data/getData/" + QString::number(sensorId));

    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply, sensorId, startDate, endDate]() {
        if (reply->error() == QNetworkReply::NoError) {
            handleMeasurementsReply(reply, sensorId, startDate, endDate);
        } else {
            emit errorOccurred(reply->errorString());
        }
        reply->deleteLater();
    });
}

void ApiService::fetchAirQualityIndex(int stationId) {
    QUrl url = baseUrl;
    url.setPath(baseUrl.path() + "/aqindex/getIndex/" + QString::number(stationId));

    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            handleAirQualityIndexReply(reply);
        } else {
            emit errorOccurred(reply->errorString());
        }
        reply->deleteLater();
    });
}

void ApiService::handleStationsReply(QNetworkReply* reply, const QString& cityFilter) {
    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (!doc.isArray()) {
        emit errorOccurred("Nieprawidłowy format odpowiedzi API (stations)");
        return;
    }

    const QString filterLower = cityFilter.trimmed().toLower();
    QJsonArray stationsArray = doc.array();
    std::vector<Station> stations;

    for (const QJsonValue& value : stationsArray) {
        QJsonObject obj = value.toObject();

        Station station;
        station.id = obj["id"].toInt();
        station.name = obj["stationName"].toString().toStdString();
        station.gegrLat = obj["gegrLat"].toString().toStdString();
        station.gegrLon = obj["gegrLon"].toString().toStdString();

        QJsonObject cityObj = obj["city"].toObject();
        station.city.id = cityObj["id"].toInt();
        station.city.name = cityObj["name"].toString().toStdString();

        QJsonObject communeObj = cityObj["commune"].toObject();
        station.city.communeName = communeObj["communeName"].toString().toStdString();
        station.city.districtName = communeObj["districtName"].toString().toStdString();
        station.city.provinceName = communeObj["provinceName"].toString().toStdString();

        station.addressStreet = obj["addressStreet"].toString().toStdString();

        // 🔍 filtrujemy po nazwie miasta
        if (!filterLower.isEmpty()) {
            const QString stationCity = QString::fromStdString(station.city.name).toLower();
            if (!stationCity.contains(filterLower))
                continue;
        }

        stations.push_back(station);
    }

    emit stationsLoaded(stations);
}

void ApiService::handleSensorsReply(QNetworkReply* reply, int stationId) {
    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (!doc.isArray()) {
        emit errorOccurred("Nieprawidłowy format odpowiedzi API (sensors)");
        return;
    }

    QJsonArray sensorsArray = doc.array();
    std::vector<Sensor> sensors;

    for (const QJsonValue& value : sensorsArray) {
        QJsonObject obj = value.toObject();

        Sensor sensor;
        sensor.id = obj["id"].toInt();
        sensor.stationId = obj["stationId"].toInt();

        QJsonObject paramObj = obj["param"].toObject();
        sensor.paramName = paramObj["paramName"].toString().toStdString();
        sensor.paramFormula = paramObj["paramFormula"].toString().toStdString();
        sensor.paramCode = paramObj["paramCode"].toString().toStdString();
        sensor.idParam = paramObj["idParam"].toInt();

        sensors.push_back(sensor);
    }

    // Dodatkowe żądanie, aby uzupełnić dane stacji
    QUrl url = baseUrl;
    url.setPath(baseUrl.path() + "/station/findAll");

    QNetworkRequest request(url);
    QNetworkReply* stationReply = networkManager->get(request);

    connect(stationReply, &QNetworkReply::finished, this, [this, stationReply, stationId, sensors]() {
        if (stationReply->error() == QNetworkReply::NoError) {
            QByteArray data = stationReply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(data);

            if (doc.isArray()) {
                QJsonArray stationsArray = doc.array();
                Station foundStation;

                for (const QJsonValue& val : stationsArray) {
                    QJsonObject obj = val.toObject();
                    if (obj["id"].toInt() == stationId) {
                        foundStation.id = obj["id"].toInt();
                        foundStation.name = obj["stationName"].toString().toStdString();
                        foundStation.gegrLat = obj["gegrLat"].toString().toStdString();
                        foundStation.gegrLon = obj["gegrLon"].toString().toStdString();

                        QJsonObject cityObj = obj["city"].toObject();
                        foundStation.city.id = cityObj["id"].toInt();
                        foundStation.city.name = cityObj["name"].toString().toStdString();

                        QJsonObject communeObj = cityObj["commune"].toObject();
                        foundStation.city.communeName = communeObj["communeName"].toString().toStdString();
                        foundStation.city.districtName = communeObj["districtName"].toString().toStdString();
                        foundStation.city.provinceName = communeObj["provinceName"].toString().toStdString();

                        foundStation.addressStreet = obj["addressStreet"].toString().toStdString();
                        break;
                    }
                }
                emit stationDetailsLoaded(foundStation, sensors);
            } else {
                emit errorOccurred("Nieprawidłowy format odpowiedzi API (stationDetails)");
            }
        } else {
            emit errorOccurred(stationReply->errorString());
        }
        stationReply->deleteLater();
    });
}

void ApiService::handleMeasurementsReply(QNetworkReply* reply, int sensorId, const QDate& startDate, const QDate& endDate) {
    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (!doc.isObject()) {
        emit errorOccurred("Nieprawidłowy format odpowiedzi API (measurements)");
        return;
    }

    QJsonObject mainObj = doc.object();
    Sensor sensor;
    sensor.id = sensorId;

    if (mainObj.contains("key") && mainObj["key"].isObject()) {
        QJsonObject keyObj = mainObj["key"].toObject();
        sensor.paramName = keyObj["paramName"].toString().toStdString();
        sensor.paramFormula = keyObj["paramFormula"].toString().toStdString();
        sensor.paramCode = keyObj["paramCode"].toString().toStdString();
        sensor.idParam = keyObj["idParam"].toInt();
    }

    std::vector<Measurement> measurements;
    if (mainObj.contains("values") && mainObj["values"].isArray()) {
        QJsonArray valuesArray = mainObj["values"].toArray();

        for (const QJsonValue& v : valuesArray) {
            QJsonObject obj = v.toObject();
            QString dateStr = obj["date"].toString();
            QDateTime dateTime = QDateTime::fromString(dateStr, Qt::ISODate);

            if (dateTime.date() >= startDate && dateTime.date() <= endDate) {
                Measurement m;
                m.date = dateStr.toStdString();
                if (!obj["value"].isNull())
                    m.value = obj["value"].toDouble();
                measurements.push_back(m);
            }
        }
    }

    emit sensorDetailsLoaded(sensor, measurements);
}

void ApiService::handleAirQualityIndexReply(QNetworkReply* reply) {
    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (!doc.isObject()) {
        emit errorOccurred("Nieprawidłowy format odpowiedzi API (airQualityIndex)");
        return;
    }

    QJsonObject obj = doc.object();
    AirQualityIndex index;
    index.id = obj["id"].toInt();
    index.stCalcDate = obj["stCalcDate"].toString().toStdString();

    QJsonObject indexLevelObj = obj["stIndexLevel"].toObject();
    index.stIndexLevel.id = indexLevelObj["id"].toInt();
    index.stIndexLevel.indexLevelName = indexLevelObj["indexLevelName"].toString().toStdString();

    index.stSourceDataDate = obj["stSourceDataDate"].toString().toStdString();

    emit airQualityIndexLoaded(index);
}
