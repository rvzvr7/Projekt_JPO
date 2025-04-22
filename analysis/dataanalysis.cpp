#include "dataanalysis.h"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <stdexcept>
#include "../logger/logger.h"

DataAnalysis::DataAnalysis() {
}

DataAnalysis::~DataAnalysis() {
}

AnalysisResult DataAnalysis::analyze(const std::vector<Measurement>& measurements) {
    AnalysisResult result;

    try {
        std::vector<Measurement> validMeasurements;
        for (const auto& m : measurements) {
            if (m.value.has_value()) {
                validMeasurements.push_back(m);
            }
        } //sdgdgsdgsdgsgds

        if (validMeasurements.empty()) {
            return result;
        }

        result.count = static_cast<int>(validMeasurements.size());

        auto minIt = std::min_element(validMeasurements.begin(), validMeasurements.end(),
                                      [](const Measurement& a, const Measurement& b) {
                                          return a.value.value() < b.value.value();
                                      });
        auto maxIt = std::max_element(validMeasurements.begin(), validMeasurements.end(),
                                      [](const Measurement& a, const Measurement& b) {
                                          return a.value.value() < b.value.value();
                                      });

        result.min = minIt->value.value();
        result.minDate = minIt->date;
        result.max = maxIt->value.value();
        result.maxDate = maxIt->date;

        double sum = std::accumulate(validMeasurements.begin(), validMeasurements.end(), 0.0,
                                     [](double acc, const Measurement& m) {
                                         return acc + m.value.value();
                                     });
        result.average = sum / validMeasurements.size();

        result.trend = calculateTrend(validMeasurements);

        Logger::log("Wykonano analizę danych: " + std::to_string(result.count) + " pomiarów");
    } catch (const std::exception& e) {
        Logger::logError("Błąd podczas analizy danych: " + std::string(e.what()));
        throw;
    }

    return result;
}

double DataAnalysis::calculateTrend(const std::vector<Measurement>& measurements) {
    if (measurements.size() < 2)
        return 0.0;

    double sumDiff = 0.0;
    for (size_t i = 1; i < measurements.size(); ++i) {
        sumDiff += measurements[i].value.value() - measurements[i-1].value.value();
    }
    return sumDiff / (measurements.size() - 1);
}
