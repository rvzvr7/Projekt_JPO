#ifndef STATION_H
#define STATION_H

#include <string>

struct City {
    int id = 0;
    std::string name;
    std::string communeName;
    std::string districtName;
    std::string provinceName;
};

struct Station {
    int id = 0;
    std::string name;
    std::string gegrLat;
    std::string gegrLon;
    City city;
    std::string addressStreet;
};

struct AirQualityIndexLevel {
    int id = 0;
    std::string indexLevelName;
};

struct AirQualityIndex {
    int id = 0;
    std::string stCalcDate;
    AirQualityIndexLevel stIndexLevel;
    std::string stSourceDataDate;
};

#endif
