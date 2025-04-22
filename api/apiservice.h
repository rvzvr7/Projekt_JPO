#ifndef APISERVICE_H
#define APISERVICE_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QDate>
#include <QString>
#include <vector>
#include <memory>
#include "../data/station.h"
#include "../data/sensor.h"
#include "../data/measurement.h"

class ApiService : public QObject {
    Q_OBJECT

public:
    explicit ApiService(QObject *parent = nullptr);
    ~ApiService();

public slots:
    void fetchStations(const QString& cityFilter = QString());
    void fetchStationDetails(int stationId);
    void fetchSensorMeasurements(int sensorId, const QDate& startDate, const QDate& endDate);
    void fetchAirQualityIndex(int stationId);

signals:
    void stationsLoaded(const std::vector<Station>& stations);
    void stationDetailsLoaded(const Station& station, const std::vector<Sensor>& sensors);
    void sensorDetailsLoaded(const Sensor& sensor, const std::vector<Measurement>& measurements);
    void airQualityIndexLoaded(const AirQualityIndex& index);
    void errorOccurred(const QString& errorMessage);

private:
    void handleStationsReply(QNetworkReply* reply, const QString& cityFilter);
    void handleSensorsReply(QNetworkReply* reply, int stationId);
    void handleMeasurementsReply(QNetworkReply* reply, int sensorId, const QDate& startDate, const QDate& endDate);
    void handleAirQualityIndexReply(QNetworkReply* reply);

    QNetworkAccessManager* networkManager;
    QUrl baseUrl;
};

#endif
