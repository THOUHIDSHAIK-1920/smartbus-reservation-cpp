#include "Person.h"

Person::Person(const QString &id, const QString &name, const QString &phone, const QString &email, const QString &address)
    : m_id(id), m_name(name), m_phone(phone), m_email(email), m_address(address)
{
}

QString Person::id() const
{
    return m_id;
}

QString Person::name() const
{
    return m_name;
}

QString Person::phone() const
{
    return m_phone;
}

QString Person::email() const
{
    return m_email;
}

QString Person::address() const
{
    return m_address;
}
