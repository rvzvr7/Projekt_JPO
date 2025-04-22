#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <string>
#include <optional>

struct Measurement {
    std::string date;
    std::optional<double> value;
};

#endif

