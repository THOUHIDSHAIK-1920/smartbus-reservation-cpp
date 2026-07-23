#include "DatabaseManager.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QDebug>

DatabaseManager::DatabaseManager()
{
}

DatabaseManager &DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

QString DatabaseManager::databasePath() const
{
    QDir appDir(QCoreApplication::applicationDirPath());
    return appDir.filePath("bus_reservation.db");
}

bool DatabaseManager::openDatabase()
{
    if (QSqlDatabase::contains("SmartBusConnection")) {
        m_database = QSqlDatabase::database("SmartBusConnection");
    } else {
        m_database = QSqlDatabase::addDatabase("QSQLITE", "SmartBusConnection");
        m_database.setDatabaseName(databasePath());
    }
    if (!m_database.open()) {
        qWarning() << "Failed to open database:" << m_database.lastError().text();
        return false;
    }
    m_database.exec("PRAGMA foreign_keys = ON;");
    return true;
}

bool DatabaseManager::initialize()
{
    if (!openDatabase()) {
        return false;
    }
    if (!createTables()) {
        return false;
    }
    return insertSampleData();
}

bool DatabaseManager::createTables()
{
    QSqlQuery query(m_database);
    bool result = query.exec(
        "CREATE TABLE IF NOT EXISTS passengers ("
        "passenger_id TEXT PRIMARY KEY,"
        "name TEXT NOT NULL,"
        "age INTEGER NOT NULL,"
        "gender TEXT NOT NULL,"
        "phone TEXT NOT NULL,"
        "email TEXT,"
        "address TEXT,"
        "identity_proof_type TEXT,"
        "identity_proof_number TEXT"
        ");"
    );
    result &= query.exec(
        "CREATE TABLE IF NOT EXISTS buses ("
        "bus_id TEXT PRIMARY KEY,"
        "bus_number TEXT UNIQUE NOT NULL,"
        "bus_name TEXT NOT NULL,"
        "bus_type TEXT NOT NULL,"
        "total_seats INTEGER NOT NULL,"
        "registration_number TEXT NOT NULL,"
        "driver_name TEXT NOT NULL,"
        "driver_phone TEXT NOT NULL,"
        "bus_status TEXT NOT NULL"
        ");"
    );
    result &= query.exec(
        "CREATE TABLE IF NOT EXISTS routes ("
        "route_id TEXT PRIMARY KEY,"
        "source TEXT NOT NULL,"
        "destination TEXT NOT NULL,"
        "departure_time TEXT NOT NULL,"
        "arrival_time TEXT NOT NULL,"
        "distance REAL NOT NULL,"
        "base_fare REAL NOT NULL,"
        "bus_id TEXT NOT NULL,"
        "travel_duration TEXT NOT NULL,"
        "FOREIGN KEY(bus_id) REFERENCES buses(bus_id) ON DELETE CASCADE"
        ");"
    );
    result &= query.exec(
        "CREATE TABLE IF NOT EXISTS bookings ("
        "booking_id TEXT PRIMARY KEY,"
        "ticket_number TEXT UNIQUE NOT NULL,"
        "passenger_id TEXT NOT NULL,"
        "bus_id TEXT NOT NULL,"
        "route_id TEXT NOT NULL,"
        "source TEXT NOT NULL,"
        "destination TEXT NOT NULL,"
        "travel_date TEXT NOT NULL,"
        "departure_time TEXT NOT NULL,"
        "seat_number INTEGER NOT NULL,"
        "passenger_count INTEGER NOT NULL,"
        "bus_type TEXT NOT NULL,"
        "fare_per_passenger REAL NOT NULL,"
        "total_fare REAL NOT NULL,"
        "booking_status TEXT NOT NULL,"
        "booking_date TEXT NOT NULL,"
        "FOREIGN KEY(passenger_id) REFERENCES passengers(passenger_id) ON DELETE CASCADE,"
        "FOREIGN KEY(bus_id) REFERENCES buses(bus_id) ON DELETE CASCADE,"
        "FOREIGN KEY(route_id) REFERENCES routes(route_id) ON DELETE CASCADE"
        ");"
    );
    result &= query.exec(
        "CREATE TABLE IF NOT EXISTS booked_seats ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "booking_id TEXT NOT NULL,"
        "bus_id TEXT NOT NULL,"
        "seat_number INTEGER NOT NULL,"
        "FOREIGN KEY(booking_id) REFERENCES bookings(booking_id) ON DELETE CASCADE"
        ");"
    );
    result &= query.exec(
        "CREATE TABLE IF NOT EXISTS payments ("
        "payment_id TEXT PRIMARY KEY,"
        "booking_id TEXT NOT NULL,"
        "passenger_id TEXT NOT NULL,"
        "ticket_number TEXT NOT NULL,"
        "base_fare REAL NOT NULL,"
        "reservation_charge REAL NOT NULL,"
        "tax REAL NOT NULL,"
        "discount REAL NOT NULL,"
        "final_amount REAL NOT NULL,"
        "payment_method TEXT NOT NULL,"
        "payment_status TEXT NOT NULL,"
        "payment_date TEXT NOT NULL,"
        "FOREIGN KEY(booking_id) REFERENCES bookings(booking_id) ON DELETE CASCADE"
        ");"
    );
    return result;
}

static QString paddedId(const QString &prefix, int number)
{
    return QString("%1%2").arg(prefix).arg(number, 3, 10, QChar('0'));
}

static QString generateId(QSqlDatabase &db, const QString &table, const QString &column, const QString &prefix)
{
    QSqlQuery query(db);
    query.exec(QString("SELECT COUNT(*) FROM %1").arg(table));
    int count = 0;
    if (query.next()) {
        count = query.value(0).toInt();
    }
    return paddedId(prefix, count + 1);
}

QString DatabaseManager::generatePassengerId()
{
    return generateId(m_database, "passengers", "passenger_id", "P");
}

QString DatabaseManager::generateBusId()
{
    return generateId(m_database, "buses", "bus_id", "B");
}

QString DatabaseManager::generateRouteId()
{
    return generateId(m_database, "routes", "route_id", "R");
}

QString DatabaseManager::generateBookingId()
{
    return generateId(m_database, "bookings", "booking_id", "BK");
}

QString DatabaseManager::generateTicketNumber()
{
    return generateId(m_database, "bookings", "ticket_number", "TKT");
}

QString DatabaseManager::generatePaymentId()
{
    return generateId(m_database, "payments", "payment_id", "PAY");
}

QSqlDatabase DatabaseManager::database() const
{
    return m_database;
}

bool DatabaseManager::insertSampleData()
{
    QSqlQuery query(m_database);
    query.exec("SELECT COUNT(*) FROM passengers");
    int count = 0;
    if (query.next()) {
        count = query.value(0).toInt();
    }
    if (count > 0) {
        return true;
    }

    m_database.transaction();
    query.prepare("INSERT INTO passengers (passenger_id, name, age, gender, phone, email, address, identity_proof_type, identity_proof_number) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
    QList<QList<QVariant>> passengers = {
        {"P001", "Thouhid Shaik", 29, "Male", "9988776655", "thouhid@example.com", "Tirupati", "Aadhaar", "1234 5678 9012"},
        {"P002", "Riya Sharma", 32, "Female", "9876543210", "riya@example.com", "Hyderabad", "Passport", "N1234567"},
        {"P003", "Arun Kumar", 40, "Male", "9123456780", "arun@example.com", "Bangalore", "Aadhaar", "2345 6789 0123"},
        {"P004", "Priya Reddy", 27, "Female", "9001234567", "priya@example.com", "Chennai", "Voter ID", "AP123456"},
        {"P005", "Suresh Babu", 50, "Male", "9980012345", "suresh@example.com", "Vijayawada", "Driving License", "DL12345"}
    };
    for (const auto &row : passengers) {
        query.addBindValue(row[0]);
        query.addBindValue(row[1]);
        query.addBindValue(row[2]);
        query.addBindValue(row[3]);
        query.addBindValue(row[4]);
        query.addBindValue(row[5]);
        query.addBindValue(row[6]);
        query.addBindValue(row[7]);
        query.addBindValue(row[8]);
        if (!query.exec()) {
            m_database.rollback();
            return false;
        }
    }

    query.prepare("INSERT INTO buses (bus_id, bus_number, bus_name, bus_type, total_seats, registration_number, driver_name, driver_phone, bus_status) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
    QList<QList<QVariant>> buses = {
        {"B001", "TS01AB001", "Express Rider", "Express", 40, "AP03AB1234", "Ramesh", "9848012345", "Available"},
        {"B002", "TS02CD002", "City Cruiser", "Ordinary", 30, "AP04CD5678", "Suresh", "9848012346", "Available"},
        {"B003", "TS03EF003", "Elite Volvo", "Volvo", 45, "AP05EF9012", "Manoj", "9848012347", "On Trip"},
        {"B004", "TS04GH004", "Night Sleeper", "AC Sleeper", 28, "AP06GH3456", "Kiran", "9848012348", "Maintenance"},
        {"B005", "TS05IJ005", "Silver Runner", "Deluxe", 36, "AP07IJ7890", "Lakshmi", "9848012349", "Available"}
    };
    for (const auto &row : buses) {
        query.addBindValue(row[0]);
        query.addBindValue(row[1]);
        query.addBindValue(row[2]);
        query.addBindValue(row[3]);
        query.addBindValue(row[4]);
        query.addBindValue(row[5]);
        query.addBindValue(row[6]);
        query.addBindValue(row[7]);
        query.addBindValue(row[8]);
        if (!query.exec()) {
            m_database.rollback();
            return false;
        }
    }

    query.prepare("INSERT INTO routes (route_id, source, destination, departure_time, arrival_time, distance, base_fare, bus_id, travel_duration) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
    QList<QList<QVariant>> routes = {
        {"R001", "Tirupati", "Chennai", "08:30", "14:00", 550.0, 600.0, "B001", "5h 30m"},
        {"R002", "Chennai", "Hyderabad", "10:00", "18:00", 630.0, 800.0, "B002", "8h 0m"},
        {"R003", "Tirupati", "Bangalore", "07:00", "13:00", 430.0, 550.0, "B003", "6h 0m"},
        {"R004", "Hyderabad", "Vijayawada", "09:30", "13:00", 280.0, 350.0, "B004", "3h 30m"},
        {"R005", "Bangalore", "Chennai", "12:00", "18:30", 350.0, 500.0, "B005", "6h 30m"},
        {"R006", "Chennai", "Tirupati", "15:00", "20:30", 550.0, 600.0, "B001", "5h 30m"}
    };
    for (const auto &row : routes) {
        query.addBindValue(row[0]);
        query.addBindValue(row[1]);
        query.addBindValue(row[2]);
        query.addBindValue(row[3]);
        query.addBindValue(row[4]);
        query.addBindValue(row[5]);
        query.addBindValue(row[6]);
        query.addBindValue(row[7]);
        query.addBindValue(row[8]);
        if (!query.exec()) {
            m_database.rollback();
            return false;
        }
    }

    query.prepare("INSERT INTO bookings (booking_id, ticket_number, passenger_id, bus_id, route_id, source, destination, travel_date, departure_time, seat_number, passenger_count, bus_type, fare_per_passenger, total_fare, booking_status, booking_date) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    QList<QList<QVariant>> bookings = {
        {"BK001", "TKT001", "P001", "B001", "R001", "Tirupati", "Chennai", "2026-07-30", "08:30", 12, 1, "Express", 600.0, 600.0, "Confirmed", "2026-07-20"},
        {"BK002", "TKT002", "P002", "B002", "R002", "Chennai", "Hyderabad", "2026-07-28", "10:00", 15, 1, "Ordinary", 800.0, 800.0, "Confirmed", "2026-07-18"},
        {"BK003", "TKT003", "P003", "B003", "R003", "Tirupati", "Bangalore", "2026-07-29", "07:00", 18, 2, "Volvo", 550.0, 1100.0, "Pending", "2026-07-19"},
        {"BK004", "TKT004", "P004", "B004", "R004", "Hyderabad", "Vijayawada", "2026-07-31", "09:30", 5, 1, "AC Sleeper", 350.0, 350.0, "Confirmed", "2026-07-21"},
        {"BK005", "TKT005", "P005", "B005", "R005", "Bangalore", "Chennai", "2026-07-30", "12:00", 22, 1, "Deluxe", 500.0, 500.0, "Cancelled", "2026-07-18"},
        {"BK006", "TKT006", "P001", "B001", "R006", "Chennai", "Tirupati", "2026-07-30", "15:00", 8, 1, "Express", 600.0, 600.0, "Confirmed", "2026-07-20"},
        {"BK007", "TKT007", "P002", "B002", "R002", "Chennai", "Hyderabad", "2026-07-29", "10:00", 16, 2, "Ordinary", 800.0, 1600.0, "Pending", "2026-07-19"},
        {"BK008", "TKT008", "P003", "B003", "R003", "Tirupati", "Bangalore", "2026-07-31", "07:00", 19, 1, "Volvo", 550.0, 550.0, "Confirmed", "2026-07-21"}
    };
    for (const auto &row : bookings) {
        query.addBindValue(row[0]);
        query.addBindValue(row[1]);
        query.addBindValue(row[2]);
        query.addBindValue(row[3]);
        query.addBindValue(row[4]);
        query.addBindValue(row[5]);
        query.addBindValue(row[6]);
        query.addBindValue(row[7]);
        query.addBindValue(row[8]);
        query.addBindValue(row[9]);
        query.addBindValue(row[10]);
        query.addBindValue(row[11]);
        query.addBindValue(row[12]);
        query.addBindValue(row[13]);
        query.addBindValue(row[14]);
        query.addBindValue(row[15]);
        if (!query.exec()) {
            m_database.rollback();
            return false;
        }
    }

    query.prepare("INSERT INTO booked_seats (booking_id, bus_id, seat_number) VALUES (?, ?, ?)");
    QList<QList<QVariant>> seats = {
        {"BK001", "B001", 12},
        {"BK002", "B002", 15},
        {"BK003", "B003", 18},
        {"BK003", "B003", 19},
        {"BK004", "B004", 5},
        {"BK005", "B005", 22},
        {"BK006", "B001", 8},
        {"BK007", "B002", 16}
    };
    for (const auto &row : seats) {
        query.addBindValue(row[0]);
        query.addBindValue(row[1]);
        query.addBindValue(row[2]);
        if (!query.exec()) {
            m_database.rollback();
            return false;
        }
    }

    query.prepare("INSERT INTO payments (payment_id, booking_id, passenger_id, ticket_number, base_fare, reservation_charge, tax, discount, final_amount, payment_method, payment_status, payment_date) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    QList<QList<QVariant>> payments = {
        {"PAY001", "BK001", "P001", "TKT001", 600.0, 40.0, 60.0, 0.0, 700.0, "UPI", "Paid", "2026-07-20"},
        {"PAY002", "BK002", "P002", "TKT002", 800.0, 40.0, 60.0, 0.0, 900.0, "Cash", "Paid", "2026-07-18"},
        {"PAY003", "BK003", "P003", "TKT003", 1100.0, 40.0, 60.0, 0.0, 1200.0, "Credit Card", "Pending", "2026-07-19"},
        {"PAY004", "BK004", "P004", "TKT004", 350.0, 40.0, 60.0, 0.0, 450.0, "Debit Card", "Paid", "2026-07-21"},
        {"PAY005", "BK005", "P005", "TKT005", 500.0, 40.0, 60.0, 50.0, 550.0, "Net Banking", "Refunded", "2026-07-18"}
    };
    for (const auto &row : payments) {
        query.addBindValue(row[0]);
        query.addBindValue(row[1]);
        query.addBindValue(row[2]);
        query.addBindValue(row[3]);
        query.addBindValue(row[4]);
        query.addBindValue(row[5]);
        query.addBindValue(row[6]);
        query.addBindValue(row[7]);
        query.addBindValue(row[8]);
        query.addBindValue(row[9]);
        query.addBindValue(row[10]);
        query.addBindValue(row[11]);
        if (!query.exec()) {
            m_database.rollback();
            return false;
        }
    }

    return m_database.commit();
}
