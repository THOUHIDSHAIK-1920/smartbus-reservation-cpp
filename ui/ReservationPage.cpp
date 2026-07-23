#include "ReservationPage.h"
#include "database/DatabaseManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

ReservationPage::ReservationPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    refreshTable();
}

void ReservationPage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QLabel *title = new QLabel("Reservations", this);
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #1c2541;");
    mainLayout->addWidget(title);

    QHBoxLayout *filterLayout = new QHBoxLayout();
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText("Search by booking ID, ticket number or passenger ID");
    m_statusCombo = new QComboBox(this);
    m_statusCombo->addItems({"All Statuses", "Confirmed", "Pending", "Completed", "Cancelled"});
    QPushButton *searchButton = new QPushButton("Search", this);
    filterLayout->addWidget(m_searchEdit);
    filterLayout->addWidget(m_statusCombo);
    filterLayout->addWidget(searchButton);
    mainLayout->addLayout(filterLayout);

    m_table = new QTableWidget(this);
    m_table->setColumnCount(8);
    m_table->setHorizontalHeaderLabels({"Booking ID", "Ticket", "Passenger", "Bus", "Route", "Seat", "Status", "Travel Date"});
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->verticalHeader()->setVisible(false);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(m_table);

    QHBoxLayout *actionLayout = new QHBoxLayout();
    QPushButton *cancelButton = new QPushButton("Cancel Ticket", this);
    QPushButton *completeButton = new QPushButton("Complete Journey", this);
    actionLayout->addWidget(cancelButton);
    actionLayout->addWidget(completeButton);
    mainLayout->addLayout(actionLayout);

    connect(searchButton, &QPushButton::clicked, this, &ReservationPage::onSearchBooking);
    connect(cancelButton, &QPushButton::clicked, this, &ReservationPage::onCancelBooking);
    connect(completeButton, &QPushButton::clicked, this, &ReservationPage::onCompleteBooking);
}

void ReservationPage::refreshTable()
{
    m_table->setRowCount(0);
    QSqlQuery query(DatabaseManager::instance().database());
    if (query.exec("SELECT booking_id, ticket_number, passenger_id, bus_id, route_id, seat_number, booking_status, travel_date FROM bookings")) {
        while (query.next()) {
            int row = m_table->rowCount();
            m_table->insertRow(row);
            for (int col = 0; col < 8; ++col) {
                m_table->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
            }
        }
    }
}

void ReservationPage::onSearchBooking()
{
    QString criteria = m_searchEdit->text().trimmed();
    QString status = m_statusCombo->currentText();
    m_table->setRowCount(0);
    QSqlQuery query(DatabaseManager::instance().database());
    QString sql = "SELECT booking_id, ticket_number, passenger_id, bus_id, route_id, seat_number, booking_status, travel_date FROM bookings WHERE (booking_id LIKE ? OR ticket_number LIKE ? OR passenger_id LIKE ?)";
    if (status != "All Statuses") {
        sql += " AND booking_status = ?";
    }
    query.prepare(sql);
    QString pattern = "%" + criteria + "%";
    query.addBindValue(pattern);
    query.addBindValue(pattern);
    query.addBindValue(pattern);
    if (status != "All Statuses") {
        query.addBindValue(status);
    }
    if (query.exec()) {
        while (query.next()) {
            int row = m_table->rowCount();
            m_table->insertRow(row);
            for (int col = 0; col < 8; ++col) {
                m_table->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
            }
        }
    }
}

void ReservationPage::onCancelBooking()
{
    auto selected = m_table->selectedItems();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Cancel", "Select a reservation first.");
        return;
    }
    QString bookingId = selected.at(0)->text();
    if (QMessageBox::question(this, "Confirm", "Cancel this booking?") != QMessageBox::Yes) {
        return;
    }
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("UPDATE bookings SET booking_status = 'Cancelled' WHERE booking_id = ?");
    query.addBindValue(bookingId);
    if (!query.exec()) {
        QMessageBox::critical(this, "Error", query.lastError().text());
        return;
    }
    QMessageBox::information(this, "Cancelled", "Booking cancelled and seat released.");
    refreshTable();
}

void ReservationPage::onCompleteBooking()
{
    auto selected = m_table->selectedItems();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Complete", "Select a reservation first.");
        return;
    }
    QString bookingId = selected.at(0)->text();
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("UPDATE bookings SET booking_status = 'Completed' WHERE booking_id = ?");
    query.addBindValue(bookingId);
    if (!query.exec()) {
        QMessageBox::critical(this, "Error", query.lastError().text());
        return;
    }
    QMessageBox::information(this, "Completed", "Booking marked as completed.");
    refreshTable();
}
