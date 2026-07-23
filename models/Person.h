#ifndef PERSON_H
#define PERSON_H

#include <QString>

class Person
{
public:
    Person() = default;
    Person(const QString &id, const QString &name, const QString &phone, const QString &email, const QString &address);
    QString id() const;
    QString name() const;
    QString phone() const;
    QString email() const;
    QString address() const;

protected:
    QString m_id;
    QString m_name;
    QString m_phone;
    QString m_email;
    QString m_address;
};

#endif // PERSON_H
