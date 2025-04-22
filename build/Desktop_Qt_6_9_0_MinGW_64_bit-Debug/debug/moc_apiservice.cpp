/****************************************************************************
** Meta object code from reading C++ file 'apiservice.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../api/apiservice.h"
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'apiservice.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN10ApiServiceE_t {};
} // unnamed namespace

template <> constexpr inline auto ApiService::qt_create_metaobjectdata<qt_meta_tag_ZN10ApiServiceE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ApiService",
        "stationsLoaded",
        "",
        "std::vector<Station>",
        "stations",
        "stationDetailsLoaded",
        "Station",
        "station",
        "std::vector<Sensor>",
        "sensors",
        "sensorDetailsLoaded",
        "Sensor",
        "sensor",
        "std::vector<Measurement>",
        "measurements",
        "airQualityIndexLoaded",
        "AirQualityIndex",
        "index",
        "errorOccurred",
        "errorMessage",
        "fetchStations",
        "cityFilter",
        "fetchStationDetails",
        "stationId",
        "fetchSensorMeasurements",
        "sensorId",
        "startDate",
        "endDate",
        "fetchAirQualityIndex"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'stationsLoaded'
        QtMocHelpers::SignalData<void(const std::vector<Station> &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'stationDetailsLoaded'
        QtMocHelpers::SignalData<void(const Station &, const std::vector<Sensor> &)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 6, 7 }, { 0x80000000 | 8, 9 },
        }}),
        // Signal 'sensorDetailsLoaded'
        QtMocHelpers::SignalData<void(const Sensor &, const std::vector<Measurement> &)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 11, 12 }, { 0x80000000 | 13, 14 },
        }}),
        // Signal 'airQualityIndexLoaded'
        QtMocHelpers::SignalData<void(const AirQualityIndex &)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 16, 17 },
        }}),
        // Signal 'errorOccurred'
        QtMocHelpers::SignalData<void(const QString &)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 19 },
        }}),
        // Slot 'fetchStations'
        QtMocHelpers::SlotData<void(const QString &)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 21 },
        }}),
        // Slot 'fetchStations'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void),
        // Slot 'fetchStationDetails'
        QtMocHelpers::SlotData<void(int)>(22, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 23 },
        }}),
        // Slot 'fetchSensorMeasurements'
        QtMocHelpers::SlotData<void(int, const QDate &, const QDate &)>(24, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 25 }, { QMetaType::QDate, 26 }, { QMetaType::QDate, 27 },
        }}),
        // Slot 'fetchAirQualityIndex'
        QtMocHelpers::SlotData<void(int)>(28, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 23 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ApiService, qt_meta_tag_ZN10ApiServiceE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ApiService::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10ApiServiceE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10ApiServiceE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10ApiServiceE_t>.metaTypes,
    nullptr
} };

void ApiService::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ApiService *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->stationsLoaded((*reinterpret_cast< std::add_pointer_t<std::vector<Station>>>(_a[1]))); break;
        case 1: _t->stationDetailsLoaded((*reinterpret_cast< std::add_pointer_t<Station>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::vector<Sensor>>>(_a[2]))); break;
        case 2: _t->sensorDetailsLoaded((*reinterpret_cast< std::add_pointer_t<Sensor>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::vector<Measurement>>>(_a[2]))); break;
        case 3: _t->airQualityIndexLoaded((*reinterpret_cast< std::add_pointer_t<AirQualityIndex>>(_a[1]))); break;
        case 4: _t->errorOccurred((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->fetchStations((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->fetchStations(); break;
        case 7: _t->fetchStationDetails((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->fetchSensorMeasurements((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QDate>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QDate>>(_a[3]))); break;
        case 9: _t->fetchAirQualityIndex((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ApiService::*)(const std::vector<Station> & )>(_a, &ApiService::stationsLoaded, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ApiService::*)(const Station & , const std::vector<Sensor> & )>(_a, &ApiService::stationDetailsLoaded, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ApiService::*)(const Sensor & , const std::vector<Measurement> & )>(_a, &ApiService::sensorDetailsLoaded, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (ApiService::*)(const AirQualityIndex & )>(_a, &ApiService::airQualityIndexLoaded, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (ApiService::*)(const QString & )>(_a, &ApiService::errorOccurred, 4))
            return;
    }
}

const QMetaObject *ApiService::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ApiService::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10ApiServiceE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ApiService::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void ApiService::stationsLoaded(const std::vector<Station> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void ApiService::stationDetailsLoaded(const Station & _t1, const std::vector<Sensor> & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}

// SIGNAL 2
void ApiService::sensorDetailsLoaded(const Sensor & _t1, const std::vector<Measurement> & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2);
}

// SIGNAL 3
void ApiService::airQualityIndexLoaded(const AirQualityIndex & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void ApiService::errorOccurred(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}
QT_WARNING_POP
