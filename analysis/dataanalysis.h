#ifndef DATAANALYSIS_H
#define DATAANALYSIS_H

#include <string>
#include <vector>
#include "../data/measurement.h"

struct AnalysisResult {
    int count = 0;
    double min = 0.0;
    std::string minDate;
    double max = 0.0;
    std::string maxDate;
    double average = 0.0;
    double trend = 0.0; //
};

class DataAnalysis {
public:
    DataAnalysis();
    ~DataAnalysis();

    AnalysisResult analyze(const std::vector<Measurement>& measurements);

private:
    double calculateTrend(const std::vector<Measurement>& measurements);
};

#endif

