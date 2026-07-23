#include "Booking.h"

Booking::Booking()
{
}

Booking::Booking(const QString &id, const QString &ticketNumber, const QString &passengerId,
                 const QString &busId, const QString &routeId, const QString &source, const QString &destination,
                 const QString &travelDate, const QString &departureTime, int seatNumber, int passengerCount,
                 const QString &busType, double farePerPassenger, double totalFare, const QString &bookingStatus,
                 const QString &bookingDate)
    : m_id(id), m_ticketNumber(ticketNumber), m_passengerId(passengerId), m_busId(busId), m_routeId(routeId), m_source(source), m_destination(destination), m_travelDate(travelDate), m_departureTime(departureTime), m_seatNumber(seatNumber), m_passengerCount(passengerCount), m_busType(busType), m_farePerPassenger(farePerPassenger), m_totalFare(totalFare), m_bookingStatus(bookingStatus), m_bookingDate(bookingDate)
{
}

QString Booking::id() const
{
    return m_id;
}

QString Booking::ticketNumber() const
{
    return m_ticketNumber;
}

QString Booking::passengerId() const
{
    return m_passengerId;
}

QString Booking::busId() const
{
    return m_busId;
}

QString Booking::routeId() const
{
    return m_routeId;
}

QString Booking::source() const
{
    return m_source;
}

QString Booking::destination() const
{
    return m_destination;
}

QString Booking::travelDate() const
{
    return m_travelDate;
}

QString Booking::departureTime() const
{
    return m_departureTime;
}

int Booking::seatNumber() const
{
    return m_seatNumber;
}

int Booking::passengerCount() const
{
    return m_passengerCount;
}

QString Booking::busType() const
{
    return m_busType;
}

double Booking::farePerPassenger() const
{
    return m_farePerPassenger;
}

double Booking::totalFare() const
{
    return m_totalFare;
}

QString Booking::bookingStatus() const
{
    return m_bookingStatus;
}

QString Booking::bookingDate() const
{
    return m_bookingDate;
}
