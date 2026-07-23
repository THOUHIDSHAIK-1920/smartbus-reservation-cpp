#include "PaymentPage.h"
#include "database/DatabaseManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QInputDialog>
#include <QSqlQuery>
#include <QSqlError>

PaymentPage::PaymentPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    refreshTable();
}

void PaymentPage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QLabel *title = new QLabel("Payments", this);
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #1c2541;");
    mainLayout->addWidget(title);

    QHBoxLayout *filterLayout = new QHBoxLayout();
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText("Search by booking ID, ticket number or passenger ID");
    m_statusCombo = new QComboBox(this);
    m_statusCombo->addItems({"All", "Paid", "Pending", "Failed", "Refunded", "Partially Paid"});
    m_methodCombo = new QComboBox(this);
    m_methodCombo->addItems({"All", "Cash", "Credit Card", "Debit Card", "UPI", "Net Banking"});
    QPushButton *searchButton = new QPushButton("Search", this);
    QPushButton *updateButton = new QPushButton("Update Status", this);
    filterLayout->addWidget(m_searchEdit);
    filterLayout->addWidget(m_statusCombo);
    filterLayout->addWidget(m_methodCombo);
    filterLayout->addWidget(searchButton);
    filterLayout->addWidget(updateButton);
    mainLayout->addLayout(filterLayout);

    m_table = new QTableWidget(this);
    m_table->setColumnCount(11);
    m_table->setHorizontalHeaderLabels({"Payment ID", "Booking ID", "Passenger ID", "Ticket", "Base Fare", "Reserve", "Tax", "Discount", "Final", "Method", "Status"});
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->verticalHeader()->setVisible(false);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(m_table);

    connect(searchButton, &QPushButton::clicked, this, &PaymentPage::onSearchPayment);
    connect(updateButton, &QPushButton::clicked, this, &PaymentPage::onUpdateStatus);
}

void PaymentPage::refreshTable()
{
    m_table->setRowCount(0);
    QSqlQuery query(DatabaseManager::instance().database());
    if (query.exec("SELECT payment_id, booking_id, passenger_id, ticket_number, base_fare, reservation_charge, tax, discount, final_amount, payment_method, payment_status FROM payments")) {
        while (query.next()) {
            int row = m_table->rowCount();
            m_table->insertRow(row);
            for (int col = 0; col < 11; ++col) {
                m_table->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
            }
        }
    }
}

void PaymentPage::onSearchPayment()
{
    QString term = m_searchEdit->text().trimmed();
    QString status = m_statusCombo->currentText();
    QString method = m_methodCombo->currentText();
    m_table->setRowCount(0);
    QSqlQuery query(DatabaseManager::instance().database());
QString sql =
    "SELECT payment_id, booking_id, passenger_id, ticket_number, "
    "base_fare, reservation_charge, tax, discount, final_amount, "
    "payment_method, payment_status "
    "FROM payments "
    "WHERE booking_id LIKE ? OR ticket_number LIKE ? OR passenger_id LIKE ?";
    if (status != "All") {
        sql += " AND payment_status = ?";
    }
    if (method != "All") {
        sql += " AND payment_method = ?";
    }
    query.prepare(sql);
    QString pattern = "%" + term + "%";
    query.addBindValue(pattern);
    query.addBindValue(pattern);
    query.addBindValue(pattern);
    if (status != "All") {
        query.addBindValue(status);
    }
    if (method != "All") {
        query.addBindValue(method);
    }
    if (query.exec()) {
        while (query.next()) {
            int row = m_table->rowCount();
            m_table->insertRow(row);
            for (int col = 0; col < 11; ++col) {
                m_table->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
            }
        }
    }
}

void PaymentPage::onUpdateStatus()
{
    auto selected = m_table->selectedItems();
   if (selected.isEmpty()) {
    QMessageBox::warning(this, "Selection Required",
                         "Please select a payment first.");
    return;
}

QString paymentId = selected.first()->text();
    bool ok = false;
    QStringList statuses = {"Paid", "Pending", "Failed", "Refunded", "Partially Paid"};
    QString status = QInputDialog::getItem(this, "Update Payment Status", "Payment Status:", statuses, 0, false, &ok);
    if (!ok || status.isEmpty()) {
        return;
    }
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("UPDATE payments SET payment_status = ? WHERE payment_id = ?");
    query.addBindValue(status);
    query.addBindValue(paymentId);
    if (!query.exec()) {
        QMessageBox::critical(this, "Error", query.lastError().text());
        return;
    }
    QMessageBox::information(this, "Updated", "Payment status updated successfully.");
    refreshTable();
}
