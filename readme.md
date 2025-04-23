# Monitor Jakości Powietrza – Qt C++ (GIOŚ API)

Aplikacja desktopowa w Qt służąca do monitorowania jakości powietrza z wykorzystaniem API Głównego Inspektoratu Ochrony Środowiska (GIOŚ).

---

## 📚 Spis treści

- [📦 Funkcje aplikacji](#-funkcje-aplikacji)
- [🧠 Struktura projektu](#-struktura-projektu)
- [💻 Technologie](#-technologie)
- [📡 API](#-api)
- [🧪 Tryb offline](#-tryb-offline)
- [🗂 Dane lokalne](#-dane-lokalne)
- [🚀 Uruchomienie](#-uruchomienie)
- [📂 Kompilacja z terminala](#-kompilacja-z-terminala)
- [👨‍💻 Autor](#-autor)
- [📝 Licencja](#-licencja)

---

## 📦 Funkcje aplikacji

- Pobieranie listy stacji pomiarowych z GIOŚ
- Wyszukiwanie stacji po nazwie miasta
- Wyszukiwanie stacji w promieniu od lokalizacji (mock)
- Wyświetlanie listy czujników (sensorów) dla danej stacji
- Wyświetlanie pomiarów z wykresem (Qt Charts)
- Analiza danych: min / max / średnia / trend
- Zapis danych do plików lokalnych (JSON)
- Odczyt danych z lokalnej bazy
- Obsługa pracy offline (fallback bez internetu)

---

## 🧠 Struktura projektu

Pliki w projekcie są podzielone według odpowiedzialności:

- `main.cpp` – punkt startowy aplikacji
- `mainwindow.h/.cpp` – główne okno i logika aplikacji
- `api/apiservice.h/.cpp` – pobieranie danych z API GIOŚ (Qt Network)
- `data/station.h`, `sensor.h`, `measurement.h` – modele danych
- `data/database.h/.cpp` – zapis i odczyt danych z lokalnej bazy (pliki JSON)
- `analysis/dataanalysis.h/.cpp` – analiza danych (min/max/avg/trend)
- `charts/chartwidget.h/.cpp` – wykresy Qt Charts
- `logger/logger.h/.cpp` – prosty logger do konsoli
- `Projekt_JPO.pro` – plik projektu Qt

---

## 💻 Technologie

- Qt 5 (testowane na 5.15.2)
- C++17
- Qt Widgets
- Qt Charts
- Qt Network (API)
- JSON (QJsonDocument)

---

## 📡 API

Wykorzystywane API GIOŚ (https://api.gios.gov.pl/pjp-api/rest):
- `GET /station/findAll` – pobieranie listy stacji
- `GET /station/sensors/{id}` – sensory w danej stacji
- `GET /data/getData/{sensorId}` – pomiary z czujnika
- `GET /aqindex/getIndex/{stationId}` – indeks jakości powietrza

---

## 🧪 Tryb offline

Jeśli aplikacja nie ma dostępu do internetu:
- wyświetla pytanie o wczytanie danych z lokalnej bazy
- jeśli są zapisane wcześniej pliki `.json`, dane są wczytywane lokalnie
- użytkownik może dalej przeglądać wykresy, analizować dane itp.

---

## 🗂 Dane lokalne

Lokalizacja plików na systemie użytkownika:
- Windows: `%AppData%/Local/TwojaAplikacja/data/`
- Linux/macOS: `~/.local/share/TwojaAplikacja/data/`

Każdy sensor zapisuje dane w osobnym pliku np.:
```
sensor_14.json
sensor_27.json
```

---

## 🚀 Uruchomienie

1. Otwórz `Projekt_JPO.pro` w Qt Creator
2. Skonfiguruj Qt 5 + kompilator (np. MSVC lub MinGW)
3. Kliknij zielony trójkąt (Run)
4. Program uruchomi się automatycznie i pobierze dane

---

## 📂 Kompilacja z terminala

Linux/macOS:
```bash
qmake Projekt_JPO.pro
make
./Projekt_JPO
```

Windows (MinGW):
```bash
qmake Projekt_JPO.pro
mingw32-make
Projekt_JPO.exe
```

---

## 👨‍💻 Autor
**Autor:** *Piotr Kozłowski*  
**Semestr:** *letni 2024/2025*

---

## 📝 Licencja

Projekt edukacyjny – tylko do celów naukowych i zaliczeniowych.
