#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <QDate>
#include "station.h"
#include "sensor.h"
#include "measurement.h"

class Database {
public:
    Database();
    ~Database();

    void saveMeasurements(const Station& station, const Sensor& sensor,
                          const std::vector<Measurement>& measurements);

    std::vector<Measurement> loadMeasurements(int sensorId, const QDate& startDate,
                                              const QDate& endDate);

    std::vector<Station> loadStations();
    std::vector<Sensor> loadSensors(int stationId);

private:
    std::string getDataPath() const;
    std::string createFilename(int sensorId) const;
    void ensureDirectoryExists(const std::string& path) const;
};

#endif
