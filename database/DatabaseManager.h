#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QString>

class DatabaseManager
{
public:
    static DatabaseManager &instance();
    bool openDatabase();
    bool initialize();
    QSqlDatabase database() const;
    QString generatePassengerId();
    QString generateBusId();
    QString generateRouteId();
    QString generateBookingId();
    QString generateTicketNumber();
    QString generatePaymentId();

private:
    DatabaseManager();
    bool createTables();
    bool insertSampleData();
    QString databasePath() const;
    QSqlDatabase m_database;
};

#endif // DATABASEMANAGER_H
