#ifndef TICKET_H
#define TICKET_H

#include <QString>

class Ticket
{
public:
    Ticket();
    Ticket(const QString &ticketNumber, const QString &bookingId, const QString &passengerName,
           const QString &busName, const QString &busNumber, const QString &busType,
           const QString &source, const QString &destination, const QString &travelDate,
           const QString &departureTime, int seatNumber, int passengerCount,
           double totalAmount, const QString &paymentStatus, const QString &bookingStatus);
    virtual ~Ticket();
    virtual QString generateReport() const;

    QString ticketNumber() const;
    QString bookingId() const;

private:
    QString m_ticketNumber;
    QString m_bookingId;
    QString m_passengerName;
    QString m_busName;
    QString m_busNumber;
    QString m_busType;
    QString m_source;
    QString m_destination;
    QString m_travelDate;
    QString m_departureTime;
    int m_seatNumber = 0;
    int m_passengerCount = 0;
    double m_totalAmount = 0.0;
    QString m_paymentStatus;
    QString m_bookingStatus;
};

#endif // TICKET_H
