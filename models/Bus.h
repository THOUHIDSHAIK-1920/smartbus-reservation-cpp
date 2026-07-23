#ifndef BUS_H
#define BUS_H

#include <QString>

class Bus
{
public:
    Bus();
    Bus(const QString &id, const QString &number, const QString &name, const QString &type,
        int totalSeats, const QString &registrationNumber, const QString &driverName,
        const QString &driverPhone, const QString &status);

    QString id() const;
    QString number() const;
    QString name() const;
    QString type() const;
    int totalSeats() const;
    QString registrationNumber() const;
    QString driverName() const;
    QString driverPhone() const;
    QString status() const;

private:
    QString m_id;
    QString m_number;
    QString m_name;
    QString m_type;
    int m_totalSeats = 0;
    QString m_registrationNumber;
    QString m_driverName;
    QString m_driverPhone;
    QString m_status;
};

#endif // BUS_H
