#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QStackedWidget>
#include <QListWidget>
#include <QWidget>

class DashboardPage;
class PassengerPage;
class BusPage;
class RoutePage;
class BookingPage;
class ReservationPage;
class PaymentPage;
class ReportsPage;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onNavigationChanged(int currentRow);
    void onExitClicked();

private:
    void setupUi();
    void setupSidebar();
    void applyStyle();

    QWidget *m_centralWidget = nullptr;
    QListWidget *m_navList = nullptr;
    QStackedWidget *m_stack = nullptr;
    DashboardPage *m_dashboardPage = nullptr;
    PassengerPage *m_passengerPage = nullptr;
    BusPage *m_busPage = nullptr;
    RoutePage *m_routePage = nullptr;
    BookingPage *m_bookingPage = nullptr;
    ReservationPage *m_reservationPage = nullptr;
    PaymentPage *m_paymentPage = nullptr;
    ReportsPage *m_reportsPage = nullptr;
};

#endif // MAINWINDOW_H
