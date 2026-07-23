#ifndef ROUTE_H
#define ROUTE_H

#include <QString>

class Route
{
public:
    Route();
    Route(const QString &id, const QString &source, const QString &destination, const QString &departureTime,
          const QString &arrivalTime, double distance, double baseFare, const QString &busId, const QString &travelDuration);

    QString id() const;
    QString source() const;
    QString destination() const;
    QString departureTime() const;
    QString arrivalTime() const;
    double distance() const;
    double baseFare() const;
    QString busId() const;
    QString travelDuration() const;

private:
    QString m_id;
    QString m_source;
    QString m_destination;
    QString m_departureTime;
    QString m_arrivalTime;
    double m_distance = 0.0;
    double m_baseFare = 0.0;
    QString m_busId;
    QString m_travelDuration;
};

#endif // ROUTE_H
