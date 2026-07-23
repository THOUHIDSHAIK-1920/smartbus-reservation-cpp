#include "DashboardPage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QFrame>

DashboardPage::DashboardPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

QLabel *DashboardPage::createMetricCard(const QString &title, const QString &value, const QString &color)
{
    QFrame *card = new QFrame(this);
    card->setObjectName("dashboardCard");
    card->setStyleSheet("QFrame#dashboardCard { background: white; border-radius: 16px; padding: 16px; }");
    card->setFrameShape(QFrame::NoFrame);

    QLabel *titleLabel = new QLabel(title, card);
    titleLabel->setStyleSheet("font-size: 12px; color: #5a5f7a;");
    QLabel *valueLabel = new QLabel(value, card);
    valueLabel->setStyleSheet(QString("font-size: 24px; font-weight: bold; color: %1;").arg(color));

    QVBoxLayout *layout = new QVBoxLayout(card);
    layout->addWidget(titleLabel);
    layout->addWidget(valueLabel);
    layout->addStretch();
    card->setLayout(layout);
    return valueLabel;
}

void DashboardPage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(20);

    QLabel *title = new QLabel("Dashboard", this);
    title->setStyleSheet("font-size: 24px; font-weight: bold; color: #1c2541;");
    mainLayout->addWidget(title);

    QHBoxLayout *metricsLayout = new QHBoxLayout();
    m_totalBuses = createMetricCard("Total Buses", "5", "#007bff");
    m_totalPassengers = createMetricCard("Total Passengers", "150", "#28a745");
    m_todayBookings = createMetricCard("Today's Bookings", "18", "#ff6f00");
    m_availableSeats = createMetricCard("Available Seats", "121", "#17a2b8");
    m_cancelledTickets = createMetricCard("Cancelled Tickets", "2", "#dc3545");
    m_totalRevenue = createMetricCard("Total Revenue", "₹24,500", "#007bff");
    metricsLayout->addWidget(m_totalBuses->parentWidget());
    metricsLayout->addWidget(m_totalPassengers->parentWidget());
    metricsLayout->addWidget(m_todayBookings->parentWidget());
    metricsLayout->addWidget(m_availableSeats->parentWidget());
    metricsLayout->addWidget(m_cancelledTickets->parentWidget());
    metricsLayout->addWidget(m_totalRevenue->parentWidget());
    mainLayout->addLayout(metricsLayout);

    QHBoxLayout *tablesLayout = new QHBoxLayout();

    QFrame *leftPanel = new QFrame(this);
    leftPanel->setObjectName("dashboardPanel");
    leftPanel->setStyleSheet("QFrame#dashboardPanel { background: #f8f9fc; border-radius: 16px; padding: 16px; }");
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    QLabel *todayTripsLabel = new QLabel("Today's Trips", leftPanel);
    todayTripsLabel->setStyleSheet("font-weight: 600; color: #1c2541;");
    leftLayout->addWidget(todayTripsLabel);
    m_todayTripsTable = new QTableWidget(4, 5, leftPanel);
    m_todayTripsTable->setHorizontalHeaderLabels({"Bus", "Route", "Time", "Seats", "Status"});
    m_todayTripsTable->horizontalHeader()->setStretchLastSection(true);
    m_todayTripsTable->verticalHeader()->setVisible(false);
    leftLayout->addWidget(m_todayTripsTable);
    tablesLayout->addWidget(leftPanel, 3);

    QFrame *rightPanel = new QFrame(this);
    rightPanel->setObjectName("dashboardPanel");
    rightPanel->setStyleSheet("QFrame#dashboardPanel { background: #f8f9fc; border-radius: 16px; padding: 16px; }");
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    QLabel *recentBookingsLabel = new QLabel("Recent Ticket Bookings", rightPanel);
    recentBookingsLabel->setStyleSheet("font-weight: 600; color: #1c2541;");
    rightLayout->addWidget(recentBookingsLabel);
    m_recentBookingsTable = new QTableWidget(4, 4, rightPanel);
    m_recentBookingsTable->setHorizontalHeaderLabels({"Ticket", "Passenger", "Route", "Status"});
    m_recentBookingsTable->horizontalHeader()->setStretchLastSection(true);
    m_recentBookingsTable->verticalHeader()->setVisible(false);
    rightLayout->addWidget(m_recentBookingsTable);
    tablesLayout->addWidget(rightPanel, 2);

    mainLayout->addLayout(tablesLayout);
    setLayout(mainLayout);
}
