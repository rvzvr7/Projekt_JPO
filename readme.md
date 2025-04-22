# Monitor Jakości Powietrza – Projekt JPO 2024/2025
Autor: Piotr Kozłowski

<p align="center">
  <img src="docs/screenshot_main.png" width="600" alt="Zrzut ekranu" />
</p>

## Spis treści
1. [Opis projektu](#opis-projektu)  
2. [Najważniejsze funkcje](#najważniejsze-funkcje)  
3. [Wymagania](#wymagania)  
4. [Instrukcja budowania i uruchamiania](#instrukcja-budowania-i-uruchamiania)  
5. [Struktura katalogów](#struktura-katalogów)  
6. [Testy jednostkowe](#testy-jednostkowe)  
7. [Plany rozwoju](#plany-rozwoju)  
8. [Licencja](#licencja)

---

## Opis projektu
Aplikacja desktopowa w C++ 17 (Qt 6) służąca do monitorowania i analizy jakości powietrza w Polsce.  
Pobiera dane z publicznego REST API GIOŚ, prezentuje je na wykresie, umożliwia zapis do lokalnej bazy
(JSON) oraz podstawową analizę statystyczną.

## Najważniejsze funkcje
- pobieranie listy stacji, czujników i pomiarów (asynchronicznie, w osobnych wątkach),
- filtrowanie stacji po nazwie miasta,
- wykres liniowy pomiarów (Qt Charts),
- zapis/odczyt danych historycznych w formacie JSON,
- analiza: min, max, średnia, trend,
- tryb offline – automatyczne przejście na dane lokalne,
- (prototyp) wyszukiwanie stacji w promieniu od wskazanego adresu.

## Wymagania
| Komponent | Wersja testowana |
|-----------|------------------|
| Qt SDK    | 6.7.x (Widgets, Network, Charts) |
| Kompilator| MSVC 2022 / MinGW 13 lub gcc 11+ |
| System    | Windows 10/11, Linux, macOS |

## Instrukcja budowania i uruchamiania
```bash
# 1. klon repozytorium
git clone https://github.com/użytkownik/Projekt_JPO.git
cd Projekt_JPO

# 2. budowa (qmake + MSVC)
qmake Projekt_JPO.pro
nmake            # lub mingw32-make / make

# 3. uruchomienie
./release/MonitorJakosciPowietrza.exe   # Windows
./MonitorJakosciPowietrza               # Linux/macOS

# 4. (Windows) opcjonalne spakowanie DLL:
windeployqt --release release/MonitorJakosciPowietrza.exe
