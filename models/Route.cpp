#include "Route.h"

Route::Route()
{
}

Route::Route(const QString &id, const QString &source, const QString &destination, const QString &departureTime,
             const QString &arrivalTime, double distance, double baseFare, const QString &busId, const QString &travelDuration)
    : m_id(id), m_source(source), m_destination(destination), m_departureTime(departureTime), m_arrivalTime(arrivalTime), m_distance(distance), m_baseFare(baseFare), m_busId(busId), m_travelDuration(travelDuration)
{
}

QString Route::id() const
{
    return m_id;
}

QString Route::source() const
{
    return m_source;
}

QString Route::destination() const
{
    return m_destination;
}

QString Route::departureTime() const
{
    return m_departureTime;
}

QString Route::arrivalTime() const
{
    return m_arrivalTime;
}

double Route::distance() const
{
    return m_distance;
}

double Route::baseFare() const
{
    return m_baseFare;
}

QString Route::busId() const
{
    return m_busId;
}

QString Route::travelDuration() const
{
    return m_travelDuration;
}
