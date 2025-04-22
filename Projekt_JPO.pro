QT += core gui network charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
INCLUDEPATH += $$PWD/logger $$PWD/api $$PWD/data $$PWD/analysis $$PWD/charts

SOURCES += \
    analysis/dataanalysis.cpp \
    api/apiservice.cpp \
    charts/chartwidget.cpp \
    data/database.cpp \
    logger/logger.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    analysis/dataanalysis.h \
    api/apiservice.h \
    charts/chartwidget.h \
    data/database.h \
    data/measurement.h \
    data/sensor.h \
    data/station.h \
    logger/logger.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    readme.md
