#include "Passenger.h"

Passenger::Passenger()
{
}

Passenger::Passenger(const QString &id, const QString &name, int age, const QString &gender, const QString &phone,
                     const QString &email, const QString &address, const QString &identityProofType, const QString &identityProofNumber)
    : Person(id, name, phone, email, address), m_age(age), m_gender(gender), m_identityProofType(identityProofType), m_identityProofNumber(identityProofNumber)
{
}

int Passenger::age() const
{
    return m_age;
}

QString Passenger::gender() const
{
    return m_gender;
}

QString Passenger::identityProofType() const
{
    return m_identityProofType;
}

QString Passenger::identityProofNumber() const
{
    return m_identityProofNumber;
}
