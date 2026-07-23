#include "Payment.h"

Payment::Payment()
{
}

Payment::Payment(const QString &id, const QString &bookingId, const QString &passengerId, const QString &ticketNumber,
                 double baseFare, double reservationCharge, double tax, double discount, double finalAmount,
                 const QString &paymentMethod, const QString &paymentStatus, const QString &paymentDate)
    : m_id(id), m_bookingId(bookingId), m_passengerId(passengerId), m_ticketNumber(ticketNumber), m_baseFare(baseFare), m_reservationCharge(reservationCharge), m_tax(tax), m_discount(discount), m_finalAmount(finalAmount), m_paymentMethod(paymentMethod), m_paymentStatus(paymentStatus), m_paymentDate(paymentDate)
{
}

QString Payment::id() const
{
    return m_id;
}

QString Payment::bookingId() const
{
    return m_bookingId;
}

QString Payment::passengerId() const
{
    return m_passengerId;
}

QString Payment::ticketNumber() const
{
    return m_ticketNumber;
}

double Payment::baseFare() const
{
    return m_baseFare;
}

double Payment::reservationCharge() const
{
    return m_reservationCharge;
}

double Payment::tax() const
{
    return m_tax;
}

double Payment::discount() const
{
    return m_discount;
}

double Payment::finalAmount() const
{
    return m_finalAmount;
}

QString Payment::paymentMethod() const
{
    return m_paymentMethod;
}

QString Payment::paymentStatus() const
{
    return m_paymentStatus;
}

QString Payment::paymentDate() const
{
    return m_paymentDate;
}
