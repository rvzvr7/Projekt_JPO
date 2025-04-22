#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <vector>
#include "../data/measurement.h"
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>

class ChartWidget : public QWidget {
    Q_OBJECT

public:
    explicit ChartWidget(QWidget* parent = nullptr);
    ~ChartWidget();

    void updateChart(const std::vector<Measurement>& measurements, const QString& title);

private:
    QVBoxLayout* layout;
    QChart* chart;
    QLineSeries* series;
    QChartView* chartView;
};

#endif
