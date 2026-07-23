#include "Ticket.h"

Ticket::Ticket()
{
}

Ticket::Ticket(const QString &ticketNumber, const QString &bookingId, const QString &passengerName,
               const QString &busName, const QString &busNumber, const QString &busType,
               const QString &source, const QString &destination, const QString &travelDate,
               const QString &departureTime, int seatNumber, int passengerCount,
               double totalAmount, const QString &paymentStatus, const QString &bookingStatus)
    : m_ticketNumber(ticketNumber), m_bookingId(bookingId), m_passengerName(passengerName), m_busName(busName), m_busNumber(busNumber), m_busType(busType), m_source(source), m_destination(destination), m_travelDate(travelDate), m_departureTime(departureTime), m_seatNumber(seatNumber), m_passengerCount(passengerCount), m_totalAmount(totalAmount), m_paymentStatus(paymentStatus), m_bookingStatus(bookingStatus)
{
}

Ticket::~Ticket() = default;

QString Ticket::generateReport() const
{
    QString report;
    report += "SMARTBUS RESERVATION TICKET\n\n";
    report += QString("Ticket Number: %1\n").arg(m_ticketNumber);
    report += QString("Booking ID: %1\n").arg(m_bookingId);
    report += QString("Passenger: %1\n").arg(m_passengerName);
    report += QString("Bus: %1\n").arg(m_busName);
    report += QString("Bus Number: %1\n").arg(m_busNumber);
    report += QString("Bus Type: %1\n").arg(m_busType);
    report += QString("Route: %1 to %2\n").arg(m_source).arg(m_destination);
    report += QString("Travel Date: %1\n").arg(m_travelDate);
    report += QString("Departure Time: %1\n").arg(m_departureTime);
    report += QString("Seat Number: %1\n").arg(m_seatNumber);
    report += QString("Passengers: %1\n").arg(m_passengerCount);
    report += QString("Total Fare: ₹%1\n").arg(m_totalAmount);
    report += QString("Payment Status: %1\n").arg(m_paymentStatus);
    report += QString("Booking Status: %1\n").arg(m_bookingStatus);
    return report;
}

QString Ticket::ticketNumber() const
{
    return m_ticketNumber;
}

QString Ticket::bookingId() const
{
    return m_bookingId;
}
