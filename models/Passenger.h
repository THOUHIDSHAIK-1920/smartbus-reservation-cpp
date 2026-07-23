#ifndef PASSENGER_H
#define PASSENGER_H

#include "Person.h"
#include <QString>

class Passenger : public Person
{
public:
    Passenger();
    Passenger(const QString &id, const QString &name, int age, const QString &gender, const QString &phone,
              const QString &email, const QString &address, const QString &identityProofType, const QString &identityProofNumber);
    int age() const;
    QString gender() const;
    QString identityProofType() const;
    QString identityProofNumber() const;

private:
    int m_age = 0;
    QString m_gender;
    QString m_identityProofType;
    QString m_identityProofNumber;
};

#endif // PASSENGER_H
