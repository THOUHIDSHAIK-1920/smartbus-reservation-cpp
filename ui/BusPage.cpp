#include "BusPage.h"
#include "database/DatabaseManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QTableWidget>
#include <QPushButton>
#include <QHeaderView>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QLabel>

BusPage::BusPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    refreshTable();
}

void BusPage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QLabel *title = new QLabel("Bus Management", this);
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #1c2541;");
    mainLayout->addWidget(title);

    QHBoxLayout *topLayout = new QHBoxLayout();
    QFormLayout *formLayout = new QFormLayout();
    m_idEdit = new QLineEdit(this);
    m_idEdit->setReadOnly(true);
    m_numberEdit = new QLineEdit(this);
    m_nameEdit = new QLineEdit(this);
    m_typeCombo = new QComboBox(this);
    m_typeCombo->addItems({"Ordinary", "Express", "Deluxe", "AC Sleeper", "Non-AC Sleeper", "Volvo"});
    m_seatsSpin = new QSpinBox(this);
    m_seatsSpin->setRange(1, 100);
    m_registrationEdit = new QLineEdit(this);
    m_driverNameEdit = new QLineEdit(this);
    m_driverPhoneEdit = new QLineEdit(this);
    m_statusCombo = new QComboBox(this);
    m_statusCombo->addItems({"Available", "On Trip", "Maintenance", "Inactive"});

    formLayout->addRow("Bus ID", m_idEdit);
    formLayout->addRow("Bus Number", m_numberEdit);
    formLayout->addRow("Bus Name", m_nameEdit);
    formLayout->addRow("Bus Type", m_typeCombo);
    formLayout->addRow("Total Seats", m_seatsSpin);
    formLayout->addRow("Registration #", m_registrationEdit);
    formLayout->addRow("Driver Name", m_driverNameEdit);
    formLayout->addRow("Driver Phone", m_driverPhoneEdit);
    formLayout->addRow("Status", m_statusCombo);

    QVBoxLayout *buttonLayout = new QVBoxLayout();
    QPushButton *addButton = new QPushButton("Add Bus", this);
    QPushButton *updateButton = new QPushButton("Update Bus", this);
    QPushButton *deleteButton = new QPushButton("Delete Bus", this);
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(updateButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addStretch();

    topLayout->addLayout(formLayout, 3);
    topLayout->addLayout(buttonLayout, 1);
    mainLayout->addLayout(topLayout);

    QHBoxLayout *searchLayout = new QHBoxLayout();
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText("Search by bus number or name");
    m_filterTypeCombo = new QComboBox(this);
    m_filterTypeCombo->addItem("All Types");
    m_filterTypeCombo->addItems({"Ordinary", "Express", "Deluxe", "AC Sleeper", "Non-AC Sleeper", "Volvo"});
    searchLayout->addWidget(m_searchEdit);
    searchLayout->addWidget(m_filterTypeCombo);
    mainLayout->addLayout(searchLayout);

    m_table = new QTableWidget(this);
    m_table->setColumnCount(9);
    m_table->setHorizontalHeaderLabels({"ID", "Number", "Name", "Type", "Seats", "Registration", "Driver", "Driver Phone", "Status"});
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->verticalHeader()->setVisible(false);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(m_table);

    connect(addButton, &QPushButton::clicked, this, &BusPage::onAddBus);
    connect(updateButton, &QPushButton::clicked, this, &BusPage::onUpdateBus);
    connect(deleteButton, &QPushButton::clicked, this, &BusPage::onDeleteBus);
    connect(m_searchEdit, &QLineEdit::textChanged, this, &BusPage::onSearchTextChanged);
    connect(m_filterTypeCombo, &QComboBox::currentTextChanged, this, &BusPage::onFilterTypeChanged);
    connect(m_table, &QTableWidget::cellClicked, this, &BusPage::onTableCellClicked);
}

void BusPage::onTableCellClicked(int row, int)
{
    m_idEdit->setText(m_table->item(row, 0)->text());
    m_numberEdit->setText(m_table->item(row, 1)->text());
    m_nameEdit->setText(m_table->item(row, 2)->text());
    int typeIndex = m_typeCombo->findText(m_table->item(row, 3)->text());
    if (typeIndex >= 0) {
        m_typeCombo->setCurrentIndex(typeIndex);
    }
    m_seatsSpin->setValue(m_table->item(row, 4)->text().toInt());
    m_registrationEdit->setText(m_table->item(row, 5)->text());
    m_driverNameEdit->setText(m_table->item(row, 6)->text());
    m_driverPhoneEdit->setText(m_table->item(row, 7)->text());
    int statusIndex = m_statusCombo->findText(m_table->item(row, 8)->text());
    if (statusIndex >= 0) {
        m_statusCombo->setCurrentIndex(statusIndex);
    }
}

bool BusPage::validateFields()
{
    if (m_numberEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation", "Bus number cannot be empty.");
        return false;
    }
    if (m_nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation", "Bus name cannot be empty.");
        return false;
    }
    if (m_seatsSpin->value() <= 0) {
        QMessageBox::warning(this, "Validation", "Total seats must be greater than zero.");
        return false;
    }
    return true;
}

void BusPage::refreshTable()
{
    m_table->setRowCount(0);
    QSqlQuery query(DatabaseManager::instance().database());
    if (query.exec("SELECT bus_id, bus_number, bus_name, bus_type, total_seats, registration_number, driver_name, driver_phone, bus_status FROM buses")) {
        while (query.next()) {
            int row = m_table->rowCount();
            m_table->insertRow(row);
            for (int col = 0; col < 9; ++col) {
                m_table->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
            }
        }
    }
}

void BusPage::onAddBus()
{
    if (!validateFields()) {
        return;
    }
    QString number = m_numberEdit->text().trimmed();
    QSqlQuery check(DatabaseManager::instance().database());
    check.prepare("SELECT bus_id FROM buses WHERE bus_number = ?");
    check.addBindValue(number);
    if (check.exec() && check.next()) {
        QMessageBox::warning(this, "Duplicate", "This bus number already exists.");
        return;
    }

    QString id = DatabaseManager::instance().generateBusId();
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("INSERT INTO buses (bus_id, bus_number, bus_name, bus_type, total_seats, registration_number, driver_name, driver_phone, bus_status) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(id);
    query.addBindValue(number);
    query.addBindValue(m_nameEdit->text().trimmed());
    query.addBindValue(m_typeCombo->currentText());
    query.addBindValue(m_seatsSpin->value());
    query.addBindValue(m_registrationEdit->text().trimmed());
    query.addBindValue(m_driverNameEdit->text().trimmed());
    query.addBindValue(m_driverPhoneEdit->text().trimmed());
    query.addBindValue(m_statusCombo->currentText());
    if (!query.exec()) {
        QMessageBox::critical(this, "Error", query.lastError().text());
        return;
    }
    QMessageBox::information(this, "Added", "Bus added successfully.");
    refreshTable();
}

void BusPage::onUpdateBus()
{
    if (!validateFields()) {
        return;
    }
    if (m_idEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Update", "Select a bus from the table first.");
        return;
    }
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("UPDATE buses SET bus_number = ?, bus_name = ?, bus_type = ?, total_seats = ?, registration_number = ?, driver_name = ?, driver_phone = ?, bus_status = ? WHERE bus_id = ?");
    query.addBindValue(m_numberEdit->text().trimmed());
    query.addBindValue(m_nameEdit->text().trimmed());
    query.addBindValue(m_typeCombo->currentText());
    query.addBindValue(m_seatsSpin->value());
    query.addBindValue(m_registrationEdit->text().trimmed());
    query.addBindValue(m_driverNameEdit->text().trimmed());
    query.addBindValue(m_driverPhoneEdit->text().trimmed());
    query.addBindValue(m_statusCombo->currentText());
    query.addBindValue(m_idEdit->text().trimmed());
    if (!query.exec()) {
        QMessageBox::critical(this, "Error", query.lastError().text());
        return;
    }
    QMessageBox::information(this, "Updated", "Bus updated successfully.");
    refreshTable();
}

void BusPage::onDeleteBus()
{
    if (m_idEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Delete", "Select a bus from the table first.");
        return;
    }
    if (QMessageBox::question(this, "Confirm Delete", "Delete selected bus?") != QMessageBox::Yes) {
        return;
    }
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("DELETE FROM buses WHERE bus_id = ?");
    query.addBindValue(m_idEdit->text().trimmed());
    if (!query.exec()) {
        QMessageBox::critical(this, "Error", query.lastError().text());
        return;
    }
    QMessageBox::information(this, "Deleted", "Bus deleted successfully.");
    refreshTable();
}

void BusPage::onSearchTextChanged(const QString &text)
{
    Q_UNUSED(text)
    onFilterTypeChanged(m_filterTypeCombo->currentText());
}

void BusPage::onFilterTypeChanged(const QString &type)
{
    QString filter = m_searchEdit->text().trimmed();
    m_table->setRowCount(0);
    QSqlQuery query(DatabaseManager::instance().database());
    QString queryString = "SELECT bus_id, bus_number, bus_name, bus_type, total_seats, registration_number, driver_name, driver_phone, bus_status FROM buses WHERE (bus_number LIKE ? OR bus_name LIKE ?)";
    if (type != "All Types") {
        queryString += " AND bus_type = ?";
    }
    query.prepare(queryString);
    QString pattern = "%" + filter + "%";
    query.addBindValue(pattern);
    query.addBindValue(pattern);
    if (type != "All Types") {
        query.addBindValue(type);
    }
    if (query.exec()) {
        while (query.next()) {
            int row = m_table->rowCount();
            m_table->insertRow(row);
            for (int col = 0; col < 9; ++col) {
                m_table->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
            }
        }
    }
}
