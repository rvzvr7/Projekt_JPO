#ifndef SENSOR_H
#define SENSOR_H

#include <string>

struct Sensor {
    int id = 0;
    int stationId = 0;
    std::string paramName;
    std::string paramFormula;
    std::string paramCode;
    int idParam = 0;
};

#endif
