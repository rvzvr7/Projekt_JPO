#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QHeaderView>
#include <QDateTime>
#include <QFileDialog>
#include "logger/logger.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Monitor Jakości Powietrza");
    resize(1200, 800);

    apiService = std::make_unique<ApiService>();
    database = std::make_unique<Database>();
    dataAnalysis = std::make_unique<DataAnalysis>();

    setupUi();
    createConnections();

    // Ładowanie stacji przy starcie
    QMetaObject::invokeMethod(this, "loadStations", Qt::QueuedConnection);
}

MainWindow::~MainWindow() {
    if (workerThread.isRunning()) {
        workerThread.quit();
        workerThread.wait();
    }
}

void MainWindow::setupUi() {
    // Główny układ
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    stackedWidget = new QStackedWidget();
    mainLayout->addWidget(stackedWidget);

    // Strona wyboru stacji
    stationsPage = new QWidget();
    QVBoxLayout* stationsLayout = new QVBoxLayout(stationsPage);

    QHBoxLayout* searchLayout = new QHBoxLayout();
    searchStationEdit = new QLineEdit();
    searchStationEdit->setPlaceholderText("Wyszukaj stację po nazwie miasta...");
    searchButton = new QPushButton("Szukaj");
    QPushButton* showMapButton = new QPushButton("Szukaj stacji w pobliżu");
    searchLayout->addWidget(searchStationEdit);
    searchLayout->addWidget(searchButton);
    searchLayout->addWidget(showMapButton);

    stationsTable = new QTableWidget();
    stationsTable->setColumnCount(4);
    QStringList headers;
    headers << "ID" << "Nazwa stacji" << "Miasto" << "Adres";
    stationsTable->setHorizontalHeaderLabels(headers);
    stationsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    stationsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    stationsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    loadingBar = new QProgressBar();
    loadingBar->setVisible(false);

    stationsLayout->addLayout(searchLayout);
    stationsLayout->addWidget(stationsTable);
    stationsLayout->addWidget(loadingBar);

    // Strona szczegółów stacji
    stationDetailsPage = new QWidget();
    QVBoxLayout* detailsLayout = new QVBoxLayout(stationDetailsPage);

    stationNameLabel = new QLabel("Stacja: ");
    stationNameLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
    stationAddressLabel = new QLabel("Adres: ");

    sensorsTable = new QTableWidget();
    sensorsTable->setColumnCount(3);
    QStringList sensorHeaders;
    sensorHeaders << "ID" << "Parametr" << "Kod";
    sensorsTable->setHorizontalHeaderLabels(sensorHeaders);
    sensorsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    sensorsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    sensorsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    backToStationsButton = new QPushButton("Powrót do listy stacji");

    detailsLayout->addWidget(stationNameLabel);
    detailsLayout->addWidget(stationAddressLabel);
    detailsLayout->addWidget(new QLabel("Dostępne pomiary:"));
    detailsLayout->addWidget(sensorsTable);
    detailsLayout->addWidget(backToStationsButton);

    // Strona pomiarów
    measurementsPage = new QWidget();
    QVBoxLayout* measurementsLayout = new QVBoxLayout(measurementsPage);

    sensorNameLabel = new QLabel("Pomiar: ");
    sensorNameLabel->setStyleSheet("font-weight: bold; font-size: 16px;");

    // Kontrolki zakresu dat
    dateRangeGroup = new QGroupBox("Zakres dat");
    QHBoxLayout* dateLayout = new QHBoxLayout(dateRangeGroup);
    QLabel* startLabel = new QLabel("Od:");
    startDateEdit = new QDateEdit(QDate::currentDate().addDays(-7));
    startDateEdit->setCalendarPopup(true);
    QLabel* endLabel = new QLabel("Do:");
    endDateEdit = new QDateEdit(QDate::currentDate());
    endDateEdit->setCalendarPopup(true);
    refreshChartButton = new QPushButton("Odśwież wykres");

    dateLayout->addWidget(startLabel);
    dateLayout->addWidget(startDateEdit);
    dateLayout->addWidget(endLabel);
    dateLayout->addWidget(endDateEdit);
    dateLayout->addWidget(refreshChartButton);

    // Wykres
    chartWidget = new ChartWidget();
    chartWidget->setMinimumHeight(300);

    // Tabela pomiarów
    measurementsTable = new QTableWidget();
    measurementsTable->setColumnCount(2);
    QStringList measurementHeaders;
    measurementHeaders << "Data i czas" << "Wartość";
    measurementsTable->setHorizontalHeaderLabels(measurementHeaders);
    measurementsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    measurementsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Przyciski akcji
    QHBoxLayout* actionButtonsLayout = new QHBoxLayout();
    backToStationButton = new QPushButton("Powrót do stacji");
    saveToDbButton = new QPushButton("Zapisz dane do bazy");
    loadFromDbButton = new QPushButton("Wczytaj dane z bazy");
    analyzeButton = new QPushButton("Analizuj dane");

    actionButtonsLayout->addWidget(backToStationButton);
    actionButtonsLayout->addWidget(saveToDbButton);
    actionButtonsLayout->addWidget(loadFromDbButton);
    actionButtonsLayout->addWidget(analyzeButton);

    measurementsLayout->addWidget(sensorNameLabel);
    measurementsLayout->addWidget(dateRangeGroup);
    measurementsLayout->addWidget(chartWidget);
    measurementsLayout->addWidget(measurementsTable);
    measurementsLayout->addLayout(actionButtonsLayout);

    // Strona analizy
    analysisPage = new QWidget();
    QVBoxLayout* analysisLayout = new QVBoxLayout(analysisPage);

    QLabel* analysisTitle = new QLabel("Wyniki analizy danych");
    analysisTitle->setStyleSheet("font-weight: bold; font-size: 16px;");

    analysisResultLabel = new QLabel();
    analysisResultLabel->setTextFormat(Qt::RichText);
    analysisResultLabel->setWordWrap(true);

    backToMeasurementsButton = new QPushButton("Powrót do pomiarów");

    analysisLayout->addWidget(analysisTitle);
    analysisLayout->addWidget(analysisResultLabel);
    analysisLayout->addStretch();
    analysisLayout->addWidget(backToMeasurementsButton);

    // Strona mapy
    mapPage = new QWidget();
    QVBoxLayout* mapLayout = new QVBoxLayout(mapPage);

    QLabel* mapTitle = new QLabel("Wyszukiwanie stacji w pobliżu");
    mapTitle->setStyleSheet("font-weight: bold; font-size: 16px;");

    QHBoxLayout* locationSearchLayout = new QHBoxLayout();
    locationInput = new QLineEdit();
    locationInput->setPlaceholderText("Podaj adres (np. Polanka 3, Poznań)");
    QLabel* radiusLabel = new QLabel("Promień (km):");
    radiusSpinBox = new QSpinBox();
    radiusSpinBox->setRange(1, 100);
    radiusSpinBox->setValue(10);
    searchLocationButton = new QPushButton("Szukaj");

    locationSearchLayout->addWidget(new QLabel("Lokalizacja:"));
    locationSearchLayout->addWidget(locationInput);
    locationSearchLayout->addWidget(radiusLabel);
    locationSearchLayout->addWidget(radiusSpinBox);
    locationSearchLayout->addWidget(searchLocationButton);

    mapLabel = new QLabel("Wybierz lokalizację, aby wyświetlić mapę");
    mapLabel->setAlignment(Qt::AlignCenter);
    mapLabel->setMinimumHeight(400);
    mapLabel->setStyleSheet("background-color: #f0f0f0; border: 1px solid #ccc;");

    backFromMapButton = new QPushButton("Powrót do listy stacji");

    mapLayout->addWidget(mapTitle);
    mapLayout->addLayout(locationSearchLayout);
    mapLayout->addWidget(mapLabel);
    mapLayout->addWidget(backFromMapButton);

    // Dodaj strony do stackedWidget
    stackedWidget->addWidget(stationsPage);
    stackedWidget->addWidget(stationDetailsPage);
    stackedWidget->addWidget(measurementsPage);
    stackedWidget->addWidget(analysisPage);
    stackedWidget->addWidget(mapPage);

    // Inicjalizacja - pokazujemy stronę ze stacjami
    stackedWidget->setCurrentWidget(stationsPage);
}

void MainWindow::createConnections() {
    // Strona stacji
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::filterStations);
    connect(stationsTable, &QTableWidget::cellDoubleClicked, [this](int row, int) {
        int stationId = stationsTable->item(row, 0)->text().toInt();
        showStationDetails(stationId);
    });

    // Strona szczegółów stacji
    connect(backToStationsButton, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentWidget(stationsPage);
    });
    connect(sensorsTable, &QTableWidget::cellDoubleClicked, [this](int row, int) {
        int sensorId = sensorsTable->item(row, 0)->text().toInt();
        showSensorMeasurements(sensorId);
    });

    // Strona pomiarów
    connect(refreshChartButton, &QPushButton::clicked, this, &MainWindow::refreshData);
    connect(backToStationButton, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentWidget(stationDetailsPage);
    });
    connect(saveToDbButton, &QPushButton::clicked, this, &MainWindow::saveDataToDatabase);
    connect(loadFromDbButton, &QPushButton::clicked, this, &MainWindow::loadDataFromDatabase);
    connect(analyzeButton, &QPushButton::clicked, this, &MainWindow::analyzeData);

    // Strona analizy
    connect(backToMeasurementsButton, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentWidget(measurementsPage);
    });

    // Strona mapy
    connect(searchLocationButton, &QPushButton::clicked, this, &MainWindow::searchNearbyStations);
    connect(backFromMapButton, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentWidget(stationsPage);
    });
}

void MainWindow::showHistoricalData()
{
    loadDataFromDatabase();
}

void MainWindow::loadStations() {
    loadingBar->setVisible(true);
    loadingBar->setValue(0);

    // Tworzymy nowy wątek roboczy oraz obiekt ApiService
    QThread* worker = new QThread();
    ApiService* apiWorker = new ApiService();
    apiWorker->moveToThread(worker);

    connect(worker, &QThread::started, [apiWorker]() {
        apiWorker->fetchStations();
    });

    connect(apiWorker, &ApiService::stationsLoaded, this, [this, worker](const std::vector<Station>& stations) {
        displayStations(stations);
        loadingBar->setValue(100);
        loadingBar->setVisible(false);
        worker->quit();
    });

    connect(apiWorker, &ApiService::errorOccurred, this, [this, worker](const QString& error) {
        handleApiError(error);
        loadingBar->setVisible(false);
        worker->quit();
    });

    connect(worker, &QThread::finished, worker, &QThread::deleteLater);
    connect(worker, &QThread::finished, apiWorker, &ApiService::deleteLater);

    worker->start();
}

void MainWindow::filterStations() {
    QString cityName = searchStationEdit->text().trimmed();
    loadingBar->setVisible(true);
    loadingBar->setValue(0);

    QThread* worker = new QThread();
    ApiService* apiWorker = new ApiService();
    apiWorker->moveToThread(worker);

    connect(worker, &QThread::started, [apiWorker, cityName]() {
        apiWorker->fetchStations(cityName);
    });
    connect(apiWorker, &ApiService::stationsLoaded, this, [this](const std::vector<Station>& stations) {
        displayStations(stations);
        loadingBar->setValue(100);
        loadingBar->setVisible(false);
        QThread* t = qobject_cast<QThread*>(sender()->thread());
        if(t) t->quit();
    });
    connect(apiWorker, &ApiService::errorOccurred, this, [this](const QString& error) {
        handleApiError(error);
        loadingBar->setVisible(false);
        QThread* t = qobject_cast<QThread*>(sender()->thread());
        if(t) t->quit();
    });
    connect(worker, &QThread::finished, worker, &QThread::deleteLater);
    connect(worker, &QThread::finished, apiWorker, &ApiService::deleteLater);

    worker->start();
}

void MainWindow::showStationDetails(int stationId) {
    loadingBar->setVisible(true);
    loadingBar->setValue(0);

    QThread* worker = new QThread();
    ApiService* apiWorker = new ApiService();
    apiWorker->moveToThread(worker);

    connect(worker, &QThread::started, [apiWorker, stationId]() {
        apiWorker->fetchStationDetails(stationId);
    });
    connect(apiWorker, &ApiService::stationDetailsLoaded, this, [this](const Station& station, const std::vector<Sensor>& sensors) {
        currentStation = station;
        stationNameLabel->setText("Stacja: " + QString::fromStdString(station.name));
        QString address = QString::fromStdString(station.city.name);
        if (!station.addressStreet.empty()) {
            address += ", " + QString::fromStdString(station.addressStreet);
        }
        stationAddressLabel->setText("Adres: " + address);

        displaySensors(sensors);
        loadingBar->setValue(100);
        loadingBar->setVisible(false);
        stackedWidget->setCurrentWidget(stationDetailsPage);
        QThread* t = qobject_cast<QThread*>(sender()->thread());
        if(t) t->quit();
    });
    connect(apiWorker, &ApiService::errorOccurred, this, [this](const QString& error) {
        handleApiError(error);
        loadingBar->setVisible(false);
        QThread* t = qobject_cast<QThread*>(sender()->thread());
        if(t) t->quit();
    });
    connect(worker, &QThread::finished, worker, &QThread::deleteLater);
    connect(worker, &QThread::finished, apiWorker, &ApiService::deleteLater);

    worker->start();
}

void MainWindow::showSensorMeasurements(int sensorId) {
    loadingBar->setVisible(true);
    loadingBar->setValue(0);

    QDate startDate = startDateEdit->date();
    QDate endDate = endDateEdit->date();

    QThread* worker = new QThread();
    ApiService* apiWorker = new ApiService();
    apiWorker->moveToThread(worker);

    connect(worker, &QThread::started, [apiWorker, sensorId, startDate, endDate]() {
        apiWorker->fetchSensorMeasurements(sensorId, startDate, endDate);
    });
    connect(apiWorker, &ApiService::sensorDetailsLoaded, this, [this](const Sensor& sensor, const std::vector<Measurement>& measurements) {
        currentSensor = sensor;
        currentMeasurements = measurements;

        sensorNameLabel->setText("Pomiar: " + QString::fromStdString(sensor.paramName) +
                                 " (" + QString::fromStdString(sensor.paramFormula) + ")");

        displayMeasurements(measurements);
        updateChart(measurements);

        loadingBar->setValue(100);
        loadingBar->setVisible(false);
        stackedWidget->setCurrentWidget(measurementsPage);
        QThread* t = qobject_cast<QThread*>(sender()->thread());
        if(t) t->quit();
    });
    connect(apiWorker, &ApiService::errorOccurred, this, [this](const QString& error) {
        handleApiError(error);
        loadingBar->setVisible(false);
        QThread* t = qobject_cast<QThread*>(sender()->thread());
        if(t) t->quit();
    });
    connect(worker, &QThread::finished, worker, &QThread::deleteLater);
    connect(worker, &QThread::finished, apiWorker, &ApiService::deleteLater);

    worker->start();
}

void MainWindow::refreshData() {
    if (currentSensor.id == 0) {
        displayMessage("Brak wybranego czujnika");
        return;
    }

    showSensorMeasurements(currentSensor.id);
}

void MainWindow::saveDataToDatabase() {
    try {
        QMutexLocker locker(&dataMutex);

        if (currentMeasurements.empty()) {
            displayMessage("Brak danych do zapisania");
            return;
        }

        database->saveMeasurements(currentStation, currentSensor, currentMeasurements);
        displayMessage("Dane zostały zapisane do bazy");
    } catch (const std::exception& e) {
        Logger::logError("Błąd podczas zapisywania danych: " + std::string(e.what()));
        displayMessage("Błąd podczas zapisywania danych: " + QString(e.what()));
    }
}

void MainWindow::loadDataFromDatabase() {
    if (currentSensor.id == 0) {
        displayMessage("Wybierz najpierw czujnik");
        return;
    }

    try {
        QMutexLocker locker(&dataMutex);

        QDate startDate = startDateEdit->date();
        QDate endDate = endDateEdit->date();

        std::vector<Measurement> measurements =
            database->loadMeasurements(currentSensor.id, startDate, endDate);

        if (measurements.empty()) {
            displayMessage("Brak danych historycznych dla wybranego zakresu dat");
            return;
        }

        currentMeasurements = measurements;
        displayMeasurements(measurements);
        updateChart(measurements);
        displayMessage("Dane historyczne zostały wczytane");
    } catch (const std::exception& e) {
        Logger::logError("Błąd podczas wczytywania danych: " + std::string(e.what()));
        displayMessage("Błąd podczas wczytywania danych: " + QString(e.what()));
    }
}

void MainWindow::analyzeData() {
    if (currentMeasurements.empty()) {
        displayMessage("Brak danych do analizy");
        return;
    }

    try {
        QMutexLocker locker(&dataMutex);

        AnalysisResult result = dataAnalysis->analyze(currentMeasurements);
        displayAnalysisResults(result);
        stackedWidget->setCurrentWidget(analysisPage);
    } catch (const std::exception& e) {
        Logger::logError("Błąd podczas analizy danych: " + std::string(e.what()));
        displayMessage("Błąd podczas analizy danych: " + QString(e.what()));
    }
}

void MainWindow::searchNearbyStations() {
    QString location = locationInput->text().trimmed();
    int radius = radiusSpinBox->value();

    if (location.isEmpty()) {
        displayMessage("Podaj adres lokalizacji");
        return;
    }

    // Symulacja wyszukiwania – do zaimplementowania geokodowania
    mapLabel->setText("Mapa zostanie wyświetlona po zaimplementowaniu usługi geokodowania.\n\n"
                      "Szukamy stacji w promieniu " + QString::number(radius) +
                      " km od lokalizacji: " + location);

    stackedWidget->setCurrentWidget(mapPage);
}

void MainWindow::showMap() {
    stackedWidget->setCurrentWidget(mapPage);
}

void MainWindow::displayStations(const std::vector<Station>& stations) {
    stationsTable->clearContents();
    stationsTable->setRowCount(stations.size());

    for (size_t i = 0; i < stations.size(); ++i) {
        const Station& station = stations[i];

        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(station.id));
        QTableWidgetItem* nameItem = new QTableWidgetItem(QString::fromStdString(station.name));
        QTableWidgetItem* cityItem = new QTableWidgetItem(QString::fromStdString(station.city.name));
        QTableWidgetItem* addressItem = new QTableWidgetItem(QString::fromStdString(station.addressStreet));

        stationsTable->setItem(i, 0, idItem);
        stationsTable->setItem(i, 1, nameItem);
        stationsTable->setItem(i, 2, cityItem);
        stationsTable->setItem(i, 3, addressItem);
    }
}

void MainWindow::displaySensors(const std::vector<Sensor>& sensors) {
    sensorsTable->clearContents();
    sensorsTable->setRowCount(sensors.size());

    for (size_t i = 0; i < sensors.size(); ++i) {
        const Sensor& sensor = sensors[i];

        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(sensor.id));
        QTableWidgetItem* nameItem = new QTableWidgetItem(QString::fromStdString(sensor.paramName));
        QTableWidgetItem* codeItem = new QTableWidgetItem(QString::fromStdString(sensor.paramCode));

        sensorsTable->setItem(i, 0, idItem);
        sensorsTable->setItem(i, 1, nameItem);
        sensorsTable->setItem(i, 2, codeItem);
    }
}

void MainWindow::displayMeasurements(const std::vector<Measurement>& measurements) {
    measurementsTable->clearContents();
    measurementsTable->setRowCount(measurements.size());

    for (size_t i = 0; i < measurements.size(); ++i) {
        const Measurement& measurement = measurements[i];

        QTableWidgetItem* dateItem = new QTableWidgetItem(QString::fromStdString(measurement.date));
        QTableWidgetItem* valueItem;
        if (measurement.value.has_value()) {
            valueItem = new QTableWidgetItem(QString::number(measurement.value.value()));
        } else {
            valueItem = new QTableWidgetItem("Brak danych");
        }

        measurementsTable->setItem(i, 0, dateItem);
        measurementsTable->setItem(i, 1, valueItem);
    }
}

void MainWindow::updateChart(const std::vector<Measurement>& measurements) {
    chartWidget->updateChart(measurements, QString::fromStdString(currentSensor.paramName));
}

void MainWindow::displayAnalysisResults(const AnalysisResult& result) {
    QString html = "<h3>Wyniki analizy:</h3>";
    html += "<p>Liczba pomiarów: <b>" + QString::number(result.count) + "</b></p>";

    if (result.count > 0) {
        html += "<p>Wartość minimalna: <b>" + QString::number(result.min) + "</b> (" +
                QString::fromStdString(result.minDate) + ")</p>";
        html += "<p>Wartość maksymalna: <b>" + QString::number(result.max) + "</b> (" +
                QString::fromStdString(result.maxDate) + ")</p>";
        html += "<p>Wartość średnia: <b>" + QString::number(result.average, 'f', 2) + "</b></p>";

        QString trend;
        if (result.trend > 0.1)
            trend = "Wzrost";
        else if (result.trend < -0.1)
            trend = "Spadek";
        else
            trend = "Stabilny";

        html += "<p>Trend: <b>" + trend + "</b></p>";
    }

    analysisResultLabel->setText(html);
}

void MainWindow::handleApiError(const QString& errorMessage) {
    Logger::logError(errorMessage.toStdString());
    displayMessage("Błąd API: " + errorMessage);

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Błąd połączenia",
        "Nie można połączyć się z API. Czy chcesz spróbować wczytać dane z lokalnej bazy?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        loadDataFromDatabase();
    }
}

void MainWindow::displayMessage(const QString& message) {
    QMessageBox::information(this, "Informacja", message);
}
