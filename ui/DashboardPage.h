#ifndef DASHBOARDPAGE_H
#define DASHBOARDPAGE_H

#include <QWidget>
#include <QObject>

class QLabel;
class QTableWidget;
class QPushButton;

class DashboardPage : public QWidget
{
    Q_OBJECT

public:
    DashboardPage(QWidget *parent = nullptr);

private:
    void setupUi();
    QLabel *createMetricCard(const QString &title, const QString &value, const QString &color);

    QLabel *m_totalBuses = nullptr;
    QLabel *m_totalPassengers = nullptr;
    QLabel *m_todayBookings = nullptr;
    QLabel *m_availableSeats = nullptr;
    QLabel *m_cancelledTickets = nullptr;
    QLabel *m_totalRevenue = nullptr;
    QTableWidget *m_todayTripsTable = nullptr;
    QTableWidget *m_recentBookingsTable = nullptr;
};

#endif // DASHBOARDPAGE_H
