#include "MainWindow.h"
#include "DashboardPage.h"
#include "PassengerPage.h"
#include "BusPage.h"
#include "RoutePage.h"
#include "BookingPage.h"
#include "ReservationPage.h"
#include "PaymentPage.h"
#include "ReportsPage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QPushButton>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    applyStyle();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupSidebar()
{
    m_navList = new QListWidget(this);
    m_navList->setObjectName("sidebarNav");
    m_navList->setFixedWidth(240);
    m_navList->setFrameShape(QFrame::NoFrame);
    m_navList->setStyleSheet("QListWidget { background: #0f1b32; color: #cfd8ef; } QListWidget::item { padding: 14px; } QListWidget::item:selected { background: #1d5eff; color: white; border-radius: 8px; }");
    m_navList->addItem("Dashboard");
    m_navList->addItem("Passengers");
    m_navList->addItem("Buses");
    m_navList->addItem("Routes");
    m_navList->addItem("Book Tickets");
    m_navList->addItem("Reservations");
    m_navList->addItem("Payments");
    m_navList->addItem("Reports");
    m_navList->addItem("Exit");
    m_navList->setCurrentRow(0);
    connect(m_navList, &QListWidget::currentRowChanged, this, &MainWindow::onNavigationChanged);
}

void MainWindow::setupUi()
{
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    setWindowTitle("SmartBus - Reservation & Ticket Management System");

    QHBoxLayout *rootLayout = new QHBoxLayout(m_centralWidget);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    setupSidebar();

    QWidget *sidebar = new QWidget(this);
    sidebar->setObjectName("sidebar");
    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(24, 24, 24, 24);
    sidebarLayout->setSpacing(24);
    QLabel *logo = new QLabel("SmartBus", sidebar);
    logo->setStyleSheet("font-size: 22px; font-weight: bold; color: white;");
    QLabel *subtitle = new QLabel("Reservation & Ticket Management System", sidebar);
    subtitle->setWordWrap(true);
    subtitle->setStyleSheet("color: #cbd4f2; font-size: 12px;");
    sidebarLayout->addWidget(logo);
    sidebarLayout->addWidget(subtitle);
    sidebarLayout->addWidget(m_navList);
    sidebarLayout->addStretch();
    QPushButton *exitButton = new QPushButton("Exit", sidebar);
    exitButton->setObjectName("exitButton");
    sidebarLayout->addWidget(exitButton);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::onExitClicked);
    sidebar->setLayout(sidebarLayout);

    m_stack = new QStackedWidget(this);
    m_dashboardPage = new DashboardPage(this);
    m_passengerPage = new PassengerPage(this);
    m_busPage = new BusPage(this);
    m_routePage = new RoutePage(this);
    m_bookingPage = new BookingPage(this);
    m_reservationPage = new ReservationPage(this);
    m_paymentPage = new PaymentPage(this);
    m_reportsPage = new ReportsPage(this);
    m_stack->addWidget(m_dashboardPage);
    m_stack->addWidget(m_passengerPage);
    m_stack->addWidget(m_busPage);
    m_stack->addWidget(m_routePage);
    m_stack->addWidget(m_bookingPage);
    m_stack->addWidget(m_reservationPage);
    m_stack->addWidget(m_paymentPage);
    m_stack->addWidget(m_reportsPage);

    rootLayout->addWidget(sidebar);
    rootLayout->addWidget(m_stack, 1);
    m_centralWidget->setLayout(rootLayout);
}

void MainWindow::applyStyle()
{
    QString stylePath;
    QDir appDir(QCoreApplication::applicationDirPath());
    QString fileName = "bus_dashboard.qss";
    if (QFile::exists(appDir.filePath(fileName))) {
        stylePath = appDir.filePath(fileName);
    } else if (QFile::exists(appDir.absoluteFilePath("../styles/" + fileName))) {
        stylePath = appDir.absoluteFilePath("../styles/" + fileName);
    }
    if (!stylePath.isEmpty()) {
        QFile file(stylePath);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream stream(&file);
            qApp->setStyleSheet(stream.readAll());
            file.close();
        }
    }
}

void MainWindow::onNavigationChanged(int currentRow)
{
    if (currentRow == 8) {
        close();
        return;
    }
    if (m_stack->count() > currentRow) {
        m_stack->setCurrentIndex(currentRow);
    }
}

void MainWindow::onExitClicked()
{
    close();
}
