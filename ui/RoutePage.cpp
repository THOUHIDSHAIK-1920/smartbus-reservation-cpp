#include "RoutePage.h"
#include "database/DatabaseManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QTimeEdit>
#include <QDoubleSpinBox>
#include <QTableWidget>
#include <QPushButton>
#include <QHeaderView>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QLabel>

RoutePage::RoutePage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    refreshTable();
}

void RoutePage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QLabel *title = new QLabel("Route Management", this);
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #1c2541;");
    mainLayout->addWidget(title);

    QHBoxLayout *topLayout = new QHBoxLayout();
    QFormLayout *formLayout = new QFormLayout();
    m_idEdit = new QLineEdit(this);
    m_idEdit->setReadOnly(true);
    m_sourceEdit = new QLineEdit(this);
    m_destinationEdit = new QLineEdit(this);
    m_departureTimeEdit = new QTimeEdit(this);
    m_arrivalTimeEdit = new QTimeEdit(this);
    m_distanceSpin = new QDoubleSpinBox(this);
    m_distanceSpin->setRange(0.0, 2000.0);
    m_distanceSpin->setSuffix(" km");
    m_baseFareSpin = new QDoubleSpinBox(this);
    m_baseFareSpin->setRange(0.0, 10000.0);
    m_baseFareSpin->setPrefix("₹");
    m_busCombo = new QComboBox(this);
    m_travelDurationEdit = new QLineEdit(this);

    formLayout->addRow("Route ID", m_idEdit);
    formLayout->addRow("Source", m_sourceEdit);
    formLayout->addRow("Destination", m_destinationEdit);
    formLayout->addRow("Departure Time", m_departureTimeEdit);
    formLayout->addRow("Arrival Time", m_arrivalTimeEdit);
    formLayout->addRow("Distance", m_distanceSpin);
    formLayout->addRow("Base Fare", m_baseFareSpin);
    formLayout->addRow("Bus ID", m_busCombo);
    formLayout->addRow("Travel Duration", m_travelDurationEdit);

    QVBoxLayout *buttonLayout = new QVBoxLayout();
    QPushButton *addButton = new QPushButton("Add Route", this);
    QPushButton *updateButton = new QPushButton("Update Route", this);
    QPushButton *deleteButton = new QPushButton("Delete Route", this);
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(updateButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addStretch();

    topLayout->addLayout(formLayout, 3);
    topLayout->addLayout(buttonLayout, 1);
    mainLayout->addLayout(topLayout);

    QHBoxLayout *searchLayout = new QHBoxLayout();
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText("Search by source or destination");
    m_filterSourceEdit = new QLineEdit(this);
    m_filterSourceEdit->setPlaceholderText("Filter source");
    m_filterDestinationEdit = new QLineEdit(this);
    m_filterDestinationEdit->setPlaceholderText("Filter destination");
    searchLayout->addWidget(m_searchEdit);
    searchLayout->addWidget(m_filterSourceEdit);
    searchLayout->addWidget(m_filterDestinationEdit);
    mainLayout->addLayout(searchLayout);

    m_table = new QTableWidget(this);
    m_table->setColumnCount(9);
    m_table->setHorizontalHeaderLabels({"ID", "Source", "Destination", "Departure", "Arrival", "Distance", "Fare", "Bus ID", "Duration"});
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->verticalHeader()->setVisible(false);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(m_table);

    connect(addButton, &QPushButton::clicked, this, &RoutePage::onAddRoute);
    connect(updateButton, &QPushButton::clicked, this, &RoutePage::onUpdateRoute);
    connect(deleteButton, &QPushButton::clicked, this, &RoutePage::onDeleteRoute);
    connect(m_searchEdit, &QLineEdit::textChanged, this, &RoutePage::onSearchTextChanged);
    connect(m_filterSourceEdit, &QLineEdit::textChanged, this, &RoutePage::onSearchTextChanged);
    connect(m_filterDestinationEdit, &QLineEdit::textChanged, this, &RoutePage::onSearchTextChanged);
    connect(m_table, &QTableWidget::cellClicked, this, &RoutePage::onTableCellClicked);

    QSqlQuery query(DatabaseManager::instance().database());
    if (query.exec("SELECT bus_id FROM buses")) {
        while (query.next()) {
            m_busCombo->addItem(query.value(0).toString());
        }
    }
}

bool RoutePage::validateFields()
{
    if (m_sourceEdit->text().trimmed().isEmpty() || m_destinationEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation", "Source and destination cannot be empty.");
        return false;
    }
    if (m_sourceEdit->text().trimmed().compare(m_destinationEdit->text().trimmed(), Qt::CaseInsensitive) == 0) {
        QMessageBox::warning(this, "Validation", "Source and destination cannot be identical.");
        return false;
    }
    if (m_baseFareSpin->value() < 0.0) {
        QMessageBox::warning(this, "Validation", "Fare cannot be negative.");
        return false;
    }
    if (m_busCombo->currentText().isEmpty()) {
        QMessageBox::warning(this, "Validation", "Assign a bus to the route.");
        return false;
    }
    return true;
}

void RoutePage::refreshTable()
{
    m_table->setRowCount(0);
    QSqlQuery query(DatabaseManager::instance().database());
    if (query.exec("SELECT route_id, source, destination, departure_time, arrival_time, distance, base_fare, bus_id, travel_duration FROM routes")) {
        while (query.next()) {
            int row = m_table->rowCount();
            m_table->insertRow(row);
            for (int col = 0; col < 9; ++col) {
                m_table->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
            }
        }
    }
}

void RoutePage::onAddRoute()
{
    if (!validateFields()) {
        return;
    }
    QString id = DatabaseManager::instance().generateRouteId();
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("INSERT INTO routes (route_id, source, destination, departure_time, arrival_time, distance, base_fare, bus_id, travel_duration) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(id);
    query.addBindValue(m_sourceEdit->text().trimmed());
    query.addBindValue(m_destinationEdit->text().trimmed());
    query.addBindValue(m_departureTimeEdit->time().toString("HH:mm"));
    query.addBindValue(m_arrivalTimeEdit->time().toString("HH:mm"));
    query.addBindValue(m_distanceSpin->value());
    query.addBindValue(m_baseFareSpin->value());
    query.addBindValue(m_busCombo->currentText());
    query.addBindValue(m_travelDurationEdit->text().trimmed());
    if (!query.exec()) {
        QMessageBox::critical(this, "Error", query.lastError().text());
        return;
    }
    QMessageBox::information(this, "Added", "Route added successfully.");
    refreshTable();
}

void RoutePage::onUpdateRoute()
{
    if (!validateFields()) {
        return;
    }
    if (m_idEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Update", "Select a route first.");
        return;
    }
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("UPDATE routes SET source = ?, destination = ?, departure_time = ?, arrival_time = ?, distance = ?, base_fare = ?, bus_id = ?, travel_duration = ? WHERE route_id = ?");
    query.addBindValue(m_sourceEdit->text().trimmed());
    query.addBindValue(m_destinationEdit->text().trimmed());
    query.addBindValue(m_departureTimeEdit->time().toString("HH:mm"));
    query.addBindValue(m_arrivalTimeEdit->time().toString("HH:mm"));
    query.addBindValue(m_distanceSpin->value());
    query.addBindValue(m_baseFareSpin->value());
    query.addBindValue(m_busCombo->currentText());
    query.addBindValue(m_travelDurationEdit->text().trimmed());
    query.addBindValue(m_idEdit->text().trimmed());
    if (!query.exec()) {
        QMessageBox::critical(this, "Error", query.lastError().text());
        return;
    }
    QMessageBox::information(this, "Updated", "Route updated successfully.");
    refreshTable();
}

void RoutePage::onDeleteRoute()
{
    if (m_idEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Delete", "Select a route first.");
        return;
    }
    if (QMessageBox::question(this, "Confirm Delete", "Delete selected route?") != QMessageBox::Yes) {
        return;
    }
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("DELETE FROM routes WHERE route_id = ?");
    query.addBindValue(m_idEdit->text().trimmed());
    if (!query.exec()) {
        QMessageBox::critical(this, "Error", query.lastError().text());
        return;
    }
    QMessageBox::information(this, "Deleted", "Route deleted successfully.");
    refreshTable();
}

void RoutePage::onSearchTextChanged(const QString &)
{
    QString keyword = m_searchEdit->text().trimmed();
    QString sourceFilter = m_filterSourceEdit->text().trimmed();
    QString destinationFilter = m_filterDestinationEdit->text().trimmed();

    m_table->setRowCount(0);
    QSqlQuery query(DatabaseManager::instance().database());
    QString queryString = "SELECT route_id, source, destination, departure_time, arrival_time, distance, base_fare, bus_id, travel_duration FROM routes WHERE 1=1";
    if (!keyword.isEmpty()) {
        queryString += " AND (source LIKE ? OR destination LIKE ? )";
    }
    if (!sourceFilter.isEmpty()) {
        queryString += " AND source LIKE ?";
    }
    if (!destinationFilter.isEmpty()) {
        queryString += " AND destination LIKE ?";
    }
    query.prepare(queryString);
    if (!keyword.isEmpty()) {
        query.addBindValue("%" + keyword + "%");
        query.addBindValue("%" + keyword + "%");
    }
    if (!sourceFilter.isEmpty()) {
        query.addBindValue("%" + sourceFilter + "%");
    }
    if (!destinationFilter.isEmpty()) {
        query.addBindValue("%" + destinationFilter + "%");
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

void RoutePage::onTableCellClicked(int row, int)
{
    m_idEdit->setText(m_table->item(row, 0)->text());
    m_sourceEdit->setText(m_table->item(row, 1)->text());
    m_destinationEdit->setText(m_table->item(row, 2)->text());
    m_departureTimeEdit->setTime(QTime::fromString(m_table->item(row, 3)->text(), "HH:mm"));
    m_arrivalTimeEdit->setTime(QTime::fromString(m_table->item(row, 4)->text(), "HH:mm"));
    m_distanceSpin->setValue(m_table->item(row, 5)->text().toDouble());
    m_baseFareSpin->setValue(m_table->item(row, 6)->text().toDouble());
    int busIndex = m_busCombo->findText(m_table->item(row, 7)->text());
    if (busIndex >= 0) {
        m_busCombo->setCurrentIndex(busIndex);
    }
    m_travelDurationEdit->setText(m_table->item(row, 8)->text());
}
