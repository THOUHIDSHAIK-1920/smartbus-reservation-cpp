#ifndef DRIVER_H
#define DRIVER_H

#include "Person.h"
#include <QString>

class Driver : public Person
{
public:
    Driver();
    Driver(const QString &id, const QString &name, const QString &phone, const QString &email,
           const QString &address, const QString &licenseNumber);
    QString licenseNumber() const;

private:
    QString m_licenseNumber;
};

#endif // DRIVER_H
