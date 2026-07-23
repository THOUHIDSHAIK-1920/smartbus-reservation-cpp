#include "Driver.h"

Driver::Driver()
{
}

Driver::Driver(const QString &id, const QString &name, const QString &phone, const QString &email,
               const QString &address, const QString &licenseNumber)
    : Person(id, name, phone, email, address), m_licenseNumber(licenseNumber)
{
}

QString Driver::licenseNumber() const
{
    return m_licenseNumber;
}
