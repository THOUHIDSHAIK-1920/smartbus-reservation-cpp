#ifndef PAYMENT_H
#define PAYMENT_H

#include <QString>

class Payment
{
public:
    Payment();
    Payment(const QString &id, const QString &bookingId, const QString &passengerId, const QString &ticketNumber,
            double baseFare, double reservationCharge, double tax, double discount, double finalAmount,
            const QString &paymentMethod, const QString &paymentStatus, const QString &paymentDate);

    QString id() const;
    QString bookingId() const;
    QString passengerId() const;
    QString ticketNumber() const;
    double baseFare() const;
    double reservationCharge() const;
    double tax() const;
    double discount() const;
    double finalAmount() const;
    QString paymentMethod() const;
    QString paymentStatus() const;
    QString paymentDate() const;

private:
    QString m_id;
    QString m_bookingId;
    QString m_passengerId;
    QString m_ticketNumber;
    double m_baseFare = 0.0;
    double m_reservationCharge = 0.0;
    double m_tax = 0.0;
    double m_discount = 0.0;
    double m_finalAmount = 0.0;
    QString m_paymentMethod;
    QString m_paymentStatus;
    QString m_paymentDate;
};

#endif // PAYMENT_H
