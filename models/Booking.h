#ifndef BOOKING_H
#define BOOKING_H

#include <QString>

class Booking
{
public:
    Booking();
    Booking(const QString &id, const QString &ticketNumber, const QString &passengerId,
            const QString &busId, const QString &routeId, const QString &source, const QString &destination,
            const QString &travelDate, const QString &departureTime, int seatNumber, int passengerCount,
            const QString &busType, double farePerPassenger, double totalFare, const QString &bookingStatus,
            const QString &bookingDate);

    QString id() const;
    QString ticketNumber() const;
    QString passengerId() const;
    QString busId() const;
    QString routeId() const;
    QString source() const;
    QString destination() const;
    QString travelDate() const;
    QString departureTime() const;
    int seatNumber() const;
    int passengerCount() const;
    QString busType() const;
    double farePerPassenger() const;
    double totalFare() const;
    QString bookingStatus() const;
    QString bookingDate() const;

private:
    QString m_id;
    QString m_ticketNumber;
    QString m_passengerId;
    QString m_busId;
    QString m_routeId;
    QString m_source;
    QString m_destination;
    QString m_travelDate;
    QString m_departureTime;
    int m_seatNumber = 0;
    int m_passengerCount = 0;
    QString m_busType;
    double m_farePerPassenger = 0.0;
    double m_totalFare = 0.0;
    QString m_bookingStatus;
    QString m_bookingDate;
};

#endif // BOOKING_H
