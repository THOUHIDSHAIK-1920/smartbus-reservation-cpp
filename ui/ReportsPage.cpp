#include "ReportsPage.h"
#include "database/DatabaseManager.h"
#include <QSqlRecord>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QMessageBox>
#include <QSqlQuery>

ReportsPage::ReportsPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    refreshTable();
}

void ReportsPage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QLabel *title = new QLabel("Reports", this);
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #1c2541;");
    mainLayout->addWidget(title);

    QHBoxLayout *filterLayout = new QHBoxLayout();
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText("Search report keyword");
    m_reportCombo = new QComboBox(this);
    m_reportCombo->addItems({"Daily Bookings", "Confirmed Bookings", "Cancelled Tickets", "Passenger Travel History", "Bus-wise Reservations", "Route-wise Bookings", "Available Seat Report", "Payment Status Report", "Refund Report", "Revenue Summary", "Most Popular Routes"});
    QPushButton *searchButton = new QPushButton("Search", this);
    m_exportButton = new QPushButton("Export CSV", this);
    filterLayout->addWidget(m_searchEdit);
    filterLayout->addWidget(m_reportCombo);
    filterLayout->addWidget(searchButton);
    filterLayout->addWidget(m_exportButton);
    mainLayout->addLayout(filterLayout);

    m_table = new QTableWidget(this);
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->verticalHeader()->setVisible(false);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(m_table);

    connect(searchButton, &QPushButton::clicked, this, &ReportsPage::onSearchReports);
    connect(m_exportButton, &QPushButton::clicked, this, &ReportsPage::onExportCsv);
}

void ReportsPage::refreshTable()
{
    onSearchReports();
}

void ReportsPage::onSearchReports()
{
    QString reportType = m_reportCombo->currentText();
    QString keyword = m_searchEdit->text().trimmed();
    QSqlQuery query(DatabaseManager::instance().database());
    QString sql;
    if (reportType == "Daily Bookings") {
        sql = "SELECT booking_id, ticket_number, passenger_id, route_id, travel_date FROM bookings WHERE travel_date = date('now')";
    } else if (reportType == "Confirmed Bookings") {
        sql = "SELECT booking_id, ticket_number, passenger_id, route_id, travel_date FROM bookings WHERE booking_status = 'Confirmed'";
    } else if (reportType == "Cancelled Tickets") {
        sql = "SELECT booking_id, ticket_number, passenger_id, route_id, travel_date FROM bookings WHERE booking_status = 'Cancelled'";
    } else if (reportType == "Passenger Travel History") {
        sql = "SELECT passenger_id, route_id, travel_date, booking_status FROM bookings";
    } else if (reportType == "Bus-wise Reservations") {
        sql = "SELECT bus_id, COUNT(*) as reservations FROM bookings GROUP BY bus_id";
    } else if (reportType == "Route-wise Bookings") {
        sql = "SELECT route_id, COUNT(*) as bookings FROM bookings GROUP BY route_id";
    } else if (reportType == "Available Seat Report") {
        sql = "SELECT bus_id, COUNT(*) as booked_seats FROM bookings WHERE booking_status != 'Cancelled' GROUP BY bus_id";
    } else if (reportType == "Payment Status Report") {
        sql = "SELECT payment_status, COUNT(*) as total FROM payments GROUP BY payment_status";
    } else if (reportType == "Refund Report") {
        sql = "SELECT payment_id, booking_id, final_amount, payment_status, payment_date FROM payments WHERE payment_status = 'Refunded'";
    } else if (reportType == "Revenue Summary") {
        sql = "SELECT SUM(final_amount) as total_revenue, payment_method FROM payments GROUP BY payment_method";
    } else if (reportType == "Most Popular Routes") {
        sql = "SELECT route_id, COUNT(*) as bookings FROM bookings GROUP BY route_id ORDER BY bookings DESC";
    }
    if (!keyword.isEmpty()) {
        if (reportType == "Daily Bookings" || reportType == "Confirmed Bookings" || reportType == "Cancelled Tickets" || reportType == "Passenger Travel History" || reportType == "Refund Report") {
            sql += " AND (booking_id LIKE '%" + keyword + "%' OR passenger_id LIKE '%" + keyword + "%' OR ticket_number LIKE '%" + keyword + "%')";
        }
    }
    if (query.exec(sql)) {
        int columns = query.record().count();
        m_table->setColumnCount(columns);
        QStringList headers;
        for (int i = 0; i < columns; ++i) {
            headers << query.record().fieldName(i);
        }
        m_table->setHorizontalHeaderLabels(headers);
        m_table->setRowCount(0);
        while (query.next()) {
            int row = m_table->rowCount();
            m_table->insertRow(row);
            for (int col = 0; col < columns; ++col) {
                m_table->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
            }
        }
    }
}

void ReportsPage::onExportCsv()
{
    QString reportType = m_reportCombo->currentText().replace(' ', '_');
    QString filePath = QDir::homePath() + "/SmartBus_Report_" + reportType + ".csv";
    QFile file(filePath);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Export", "Unable to write report file.");
        return;
    }
    QTextStream out(&file);
    QStringList headers;
    for (int col = 0; col < m_table->columnCount(); ++col) {
        headers << m_table->horizontalHeaderItem(col)->text();
    }
    out << headers.join(',') << '\n';
    for (int row = 0; row < m_table->rowCount(); ++row) {
        QStringList rowValues;
        for (int col = 0; col < m_table->columnCount(); ++col) {
            rowValues << m_table->item(row, col)->text();
        }
        out << rowValues.join(',') << '\n';
    }
    file.close();
    QMessageBox::information(this, "Exported", "Report exported to " + filePath);
}
