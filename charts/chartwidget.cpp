#include "chartwidget.h"
#include <QDateTime>
#include <QPainter>
#include <limits>
#include "../logger/logger.h"

ChartWidget::ChartWidget(QWidget* parent)
    : QWidget(parent),
    layout(new QVBoxLayout(this)),
    chart(new QChart()),
    series(new QLineSeries()),
    chartView(nullptr)
{
    chart->setTitle("Pomiary");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Dodajemy serię do wykresu
    chart->addSeries(series);

    // Oś X (oś czasu)
    QDateTimeAxis* axisX = new QDateTimeAxis;
    axisX->setFormat("dd.MM HH:mm");
    axisX->setTitleText("Data");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Oś Y (wartości)
    QValueAxis* axisY = new QValueAxis;
    axisY->setTitleText("Wartość");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    layout->addWidget(chartView);
    setLayout(layout);
}

ChartWidget::~ChartWidget() {

}

void ChartWidget::updateChart(const std::vector<Measurement>& measurements, const QString& title)
{
    try {
        chart->setTitle(title);
        series->clear();

        if (measurements.empty()) {
            return;
        }

        QDateTime minDate, maxDate;
        double minValue = std::numeric_limits<double>::max();
        double maxValue = std::numeric_limits<double>::lowest();

        for (const auto& m : measurements) {
            if (!m.value.has_value())
                continue;

            QDateTime dateTime = QDateTime::fromString(
                QString::fromStdString(m.date),
                Qt::ISODate
                );
            double value = m.value.value();
            series->append(dateTime.toMSecsSinceEpoch(), value);

            if (minDate.isNull() || dateTime < minDate)
                minDate = dateTime;
            if (maxDate.isNull() || dateTime > maxDate)
                maxDate = dateTime;

            minValue = std::min(minValue, value);
            maxValue = std::max(maxValue, value);
        }

        // Ustawiamy zakres osi czasu
        if (auto* axisX = qobject_cast<QDateTimeAxis*>(chart->axes(Qt::Horizontal).first())) {
            axisX->setRange(minDate, maxDate);
        }

        // Ustawiamy zakres osi wartości
        if (auto* axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first())) {
            double range = maxValue - minValue;
            axisY->setRange(std::max(0.0, minValue - range * 0.1),
                            maxValue + range * 0.1);
        }

        Logger::log("Zaktualizowano wykres: " + title.toStdString());
    } catch (const std::exception& e) {
        Logger::logError("Błąd aktualizacji wykresu: " + std::string(e.what()));
    }
}
