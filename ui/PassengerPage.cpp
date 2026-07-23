#include "PassengerPage.h"
#include "database/DatabaseManager.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
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

PassengerPage::PassengerPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    refreshTable();
}

void PassengerPage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QLabel *title = new QLabel("Passenger Management", this);
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #1c2541;");
    mainLayout->addWidget(title);

    QHBoxLayout *topLayout = new QHBoxLayout();
    QFormLayout *formLayout = new QFormLayout();
    m_idEdit = new QLineEdit(this);
    m_idEdit->setReadOnly(true);
    m_nameEdit = new QLineEdit(this);
    m_ageSpin = new QSpinBox(this);
    m_ageSpin->setRange(1, 110);
    m_genderCombo = new QComboBox(this);
    m_genderCombo->addItems({"Male", "Female", "Other"});
    m_phoneEdit = new QLineEdit(this);
    m_emailEdit = new QLineEdit(this);
    m_addressEdit = new QLineEdit(this);
    m_idProofTypeEdit = new QLineEdit(this);
    m_idProofNumberEdit = new QLineEdit(this);

    formLayout->addRow("Passenger ID", m_idEdit);
    formLayout->addRow("Name", m_nameEdit);
    formLayout->addRow("Age", m_ageSpin);
    formLayout->addRow("Gender", m_genderCombo);
    formLayout->addRow("Phone", m_phoneEdit);
    formLayout->addRow("Email", m_emailEdit);
    formLayout->addRow("Address", m_addressEdit);
    formLayout->addRow("ID Proof Type", m_idProofTypeEdit);
    formLayout->addRow("ID Proof Number", m_idProofNumberEdit);

    QVBoxLayout *buttonLayout = new QVBoxLayout();
    QPushButton *addButton = new QPushButton("Add Passenger", this);
    QPushButton *updateButton = new QPushButton("Update Passenger", this);
    QPushButton *deleteButton = new QPushButton("Delete Passenger", this);
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(updateButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addStretch();

    topLayout->addLayout(formLayout, 3);
    topLayout->addLayout(buttonLayout, 1);
    mainLayout->addLayout(topLayout);

    QHBoxLayout *searchLayout = new QHBoxLayout();
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText("Search by ID, name or phone");
    searchLayout->addWidget(m_searchEdit);
    mainLayout->addLayout(searchLayout);

    m_table = new QTableWidget(this);
    m_table->setColumnCount(9);
    m_table->setHorizontalHeaderLabels({"ID", "Name", "Age", "Gender", "Phone", "Email", "Address", "Proof Type", "Proof Number"});
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->verticalHeader()->setVisible(false);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(m_table);

    connect(addButton, &QPushButton::clicked, this, &PassengerPage::onAddPassenger);
    connect(updateButton, &QPushButton::clicked, this, &PassengerPage::onUpdatePassenger);
    connect(deleteButton, &QPushButton::clicked, this, &PassengerPage::onDeletePassenger);
    connect(m_searchEdit, &QLineEdit::textChanged, this, &PassengerPage::onSearchTextChanged);
    connect(m_table, &QTableWidget::cellClicked, this, &PassengerPage::onTableCellClicked);
}

void PassengerPage::onTableCellClicked(int row, int)
{
    m_idEdit->setText(m_table->item(row, 0)->text());
    m_nameEdit->setText(m_table->item(row, 1)->text());
    m_ageSpin->setValue(m_table->item(row, 2)->text().toInt());
    int genderIndex = m_genderCombo->findText(m_table->item(row, 3)->text());
    if (genderIndex >= 0) {
        m_genderCombo->setCurrentIndex(genderIndex);
    }
    m_phoneEdit->setText(m_table->item(row, 4)->text());
    m_emailEdit->setText(m_table->item(row, 5)->text());
    m_addressEdit->setText(m_table->item(row, 6)->text());
    m_idProofTypeEdit->setText(m_table->item(row, 7)->text());
    m_idProofNumberEdit->setText(m_table->item(row, 8)->text());
}

bool PassengerPage::validateFields()
{
    if (m_nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation", "Passenger name cannot be empty.");
        return false;
    }
    if (m_phoneEdit->text().trimmed().isEmpty() || m_phoneEdit->text().trimmed().size() < 10) {
        QMessageBox::warning(this, "Validation", "Phone number must contain at least 10 digits.");
        return false;
    }
    return true;
}

void PassengerPage::refreshTable()
{
    m_table->setRowCount(0);
    QSqlQuery query(DatabaseManager::instance().database());
    if (query.exec("SELECT passenger_id, name, age, gender, phone, email, address, identity_proof_type, identity_proof_number FROM passengers")) {
        while (query.next()) {
            int row = m_table->rowCount();
            m_table->insertRow(row);
            for (int col = 0; col < 9; ++col) {
                m_table->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
            }
        }
    }
}

void PassengerPage::onAddPassenger()
{
    if (!validateFields()) {
        return;
    }

    QString phone = m_phoneEdit->text().trimmed();
    QSqlQuery checkQuery(DatabaseManager::instance().database());
    checkQuery.prepare("SELECT passenger_id FROM passengers WHERE phone = ?");
    checkQuery.addBindValue(phone);
    if (checkQuery.exec() && checkQuery.next()) {
        QMessageBox::warning(this, "Duplicate", "A passenger with this phone number already exists.");
        return;
    }

    QString id = DatabaseManager::instance().generatePassengerId();
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("INSERT INTO passengers (passenger_id, name, age, gender, phone, email, address, identity_proof_type, identity_proof_number) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(id);
    query.addBindValue(m_nameEdit->text().trimmed());
    query.addBindValue(m_ageSpin->value());
    query.addBindValue(m_genderCombo->currentText());
    query.addBindValue(phone);
    query.addBindValue(m_emailEdit->text().trimmed());
    query.addBindValue(m_addressEdit->text().trimmed());
    query.addBindValue(m_idProofTypeEdit->text().trimmed());
    query.addBindValue(m_idProofNumberEdit->text().trimmed());
    if (!query.exec()) {
        QMessageBox::critical(this, "Error", query.lastError().text());
        return;
    }
    QMessageBox::information(this, "Added", "Passenger added successfully.");
    refreshTable();
}

void PassengerPage::onUpdatePassenger()
{
    if (!validateFields()) {
        return;
    }

    if (m_idEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Update", "Select a passenger from the table first.");
        return;
    }

    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("UPDATE passengers SET name = ?, age = ?, gender = ?, phone = ?, email = ?, address = ?, identity_proof_type = ?, identity_proof_number = ? WHERE passenger_id = ?");
    query.addBindValue(m_nameEdit->text().trimmed());
    query.addBindValue(m_ageSpin->value());
    query.addBindValue(m_genderCombo->currentText());
    query.addBindValue(m_phoneEdit->text().trimmed());
    query.addBindValue(m_emailEdit->text().trimmed());
    query.addBindValue(m_addressEdit->text().trimmed());
    query.addBindValue(m_idProofTypeEdit->text().trimmed());
    query.addBindValue(m_idProofNumberEdit->text().trimmed());
    query.addBindValue(m_idEdit->text().trimmed());
    if (!query.exec()) {
        QMessageBox::critical(this, "Error", query.lastError().text());
        return;
    }
    QMessageBox::information(this, "Updated", "Passenger updated successfully.");
    refreshTable();
}

void PassengerPage::onDeletePassenger()
{
    if (m_idEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Delete", "Select a passenger from the table first.");
        return;
    }
    if (QMessageBox::question(this, "Confirm Delete", "Are you sure you want to delete this passenger?") != QMessageBox::Yes) {
        return;
    }

    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("DELETE FROM passengers WHERE passenger_id = ?");
    query.addBindValue(m_idEdit->text().trimmed());
    if (!query.exec()) {
        QMessageBox::critical(this, "Error", query.lastError().text());
        return;
    }
    QMessageBox::information(this, "Deleted", "Passenger deleted successfully.");
    refreshTable();
}

void PassengerPage::onSearchTextChanged(const QString &text)
{
    QString filter = text.trimmed();
    m_table->setRowCount(0);
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT passenger_id, name, age, gender, phone, email, address, identity_proof_type, identity_proof_number FROM passengers WHERE passenger_id LIKE ? OR name LIKE ? OR phone LIKE ?");
    QString pattern = "%" + filter + "%";
    query.addBindValue(pattern);
    query.addBindValue(pattern);
    query.addBindValue(pattern);
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
