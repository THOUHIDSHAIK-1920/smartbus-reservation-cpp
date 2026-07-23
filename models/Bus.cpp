#include "Bus.h"

Bus::Bus()
{
}

Bus::Bus(const QString &id, const QString &number, const QString &name, const QString &type,
         int totalSeats, const QString &registrationNumber, const QString &driverName,
         const QString &driverPhone, const QString &status)
    : m_id(id), m_number(number), m_name(name), m_type(type), m_totalSeats(totalSeats), m_registrationNumber(registrationNumber), m_driverName(driverName), m_driverPhone(driverPhone), m_status(status)
{
}

QString Bus::id() const
{
    return m_id;
}

QString Bus::number() const
{
    return m_number;
}

QString Bus::name() const
{
    return m_name;
}

QString Bus::type() const
{
    return m_type;
}

int Bus::totalSeats() const
{
    return m_totalSeats;
}

QString Bus::registrationNumber() const
{
    return m_registrationNumber;
}

QString Bus::driverName() const
{
    return m_driverName;
}

QString Bus::driverPhone() const
{
    return m_driverPhone;
}

QString Bus::status() const
{
    return m_status;
}
