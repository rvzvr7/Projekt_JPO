#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QGroupBox>
#include <QLabel>
#include <QTabWidget>
#include <QTableWidget>
#include <QProgressBar>
#include <QSpinBox>
#include <QDateEdit>
#include <QCheckBox>
#include <QThread>
#include <QMutex>
#include <memory>
#include <vector>
#include "api/apiservice.h"
#include "data/database.h"
#include "data/station.h"
#include "data/sensor.h"
#include "data/measurement.h"
#include "charts/chartwidget.h"
#include "analysis/dataanalysis.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadStations();
    void filterStations();
    void showStationDetails(int stationId);
    void showSensorMeasurements(int sensorId);
    void showHistoricalData();
    void saveDataToDatabase();
    void loadDataFromDatabase();
    void analyzeData();
    void refreshData();
    void searchNearbyStations();
    void showMap();

private:
    void setupUi();
    void createConnections();
    void displayStations(const std::vector<Station>& stations);
    void displaySensors(const std::vector<Sensor>& sensors);
    void displayMeasurements(const std::vector<Measurement>& measurements);
    void displayAnalysisResults(const AnalysisResult& result);
    void updateChart(const std::vector<Measurement>& measurements);
    void handleApiError(const QString& errorMessage);
    void displayMessage(const QString& message);

    // Interfejs użytkownika
    QStackedWidget* stackedWidget;
    QTabWidget* tabWidget;

    // Strona wyboru stacji
    QWidget* stationsPage;
    QLineEdit* searchStationEdit;
    QPushButton* searchButton;
    QTableWidget* stationsTable;
    QProgressBar* loadingBar;

    // Strona szczegółów stacji
    QWidget* stationDetailsPage;
    QLabel* stationNameLabel;
    QLabel* stationAddressLabel;
    QTableWidget* sensorsTable;
    QPushButton* backToStationsButton;

    // Strona pomiarów
    QWidget* measurementsPage;
    QLabel* sensorNameLabel;
    ChartWidget* chartWidget;
    QTableWidget* measurementsTable;
    QGroupBox* dateRangeGroup;
    QDateEdit* startDateEdit;
    QDateEdit* endDateEdit;
    QPushButton* refreshChartButton;
    QPushButton* backToStationButton;
    QPushButton* saveToDbButton;
    QPushButton* loadFromDbButton;
    QPushButton* analyzeButton;

    // Strona analizy
    QWidget* analysisPage;
    QLabel* analysisResultLabel;
    QPushButton* backToMeasurementsButton;

    // Strona mapy
    QWidget* mapPage;
    QLineEdit* locationInput;
    QSpinBox* radiusSpinBox;
    QPushButton* searchLocationButton;
    QLabel* mapLabel;
    QPushButton* backFromMapButton;

    // Dane
    std::unique_ptr<ApiService> apiService;
    std::unique_ptr<Database> database;
    std::unique_ptr<DataAnalysis> dataAnalysis;

    // Aktualnie wybrane dane
    Station currentStation;
    Sensor currentSensor;
    std::vector<Measurement> currentMeasurements;

    // Wątki i synchronizacja
    QThread workerThread;
    QMutex dataMutex;
};

#endif // MAINWINDOW_H
