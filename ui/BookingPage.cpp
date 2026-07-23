#include "BookingPage.h"
#include "database/DatabaseManager.h"
#include <QDir>
#include "models/Ticket.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QTimeEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QHeaderView>
#include <QGridLayout>
#include <QSet>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDate>

BookingPage::BookingPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    loadRoutes();
}

void BookingPage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QLabel *title = new QLabel("Book Tickets", this);
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #1c2541;");
    mainLayout->addWidget(title);

    QHBoxLayout *searchLayout = new QHBoxLayout();
    m_searchSource = new QComboBox(this);
    m_searchDestination = new QComboBox(this);
    m_searchSource->setEditable(true);
    m_searchDestination->setEditable(true);
    m_travelDateEdit = new QDateEdit(QDate::currentDate(), this);
    m_travelDateEdit->setCalendarPopup(true);
    m_travelDateEdit->setMinimumDate(QDate::currentDate());
    QPushButton *searchButton = new QPushButton("Search Available Buses", this);
    searchLayout->addWidget(new QLabel("Source:"));
    searchLayout->addWidget(m_searchSource);
    searchLayout->addWidget(new QLabel("Destination:"));
    searchLayout->addWidget(m_searchDestination);
    searchLayout->addWidget(new QLabel("Travel Date:"));
    searchLayout->addWidget(m_travelDateEdit);
    searchLayout->addWidget(searchButton);
    mainLayout->addLayout(searchLayout);

    m_routeTable = new QTableWidget(this);
    m_routeTable->setColumnCount(7);
    m_routeTable->setHorizontalHeaderLabels({"Route ID", "Source", "Destination", "Departure", "Arrival", "Fare", "Bus ID"});
    m_routeTable->horizontalHeader()->setStretchLastSection(true);
    m_routeTable->verticalHeader()->setVisible(false);
    m_routeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_routeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(m_routeTable);

    QHBoxLayout *formLayout = new QHBoxLayout();
    QFormLayout *leftForm = new QFormLayout();
    m_passengerIdEdit = new QLineEdit(this);
    m_ticketNumberEdit = new QLineEdit(this);
    m_ticketNumberEdit->setReadOnly(true);
    m_routeIdEdit = new QLineEdit(this);
    m_routeIdEdit->setReadOnly(true);
    m_busIdEdit = new QLineEdit(this);
    m_busIdEdit->setReadOnly(true);
    m_sourceEdit = new QLineEdit(this);
    m_sourceEdit->setReadOnly(true);
    m_destinationEdit = new QLineEdit(this);
    m_destinationEdit->setReadOnly(true);
    m_departureTimeEdit = new QTimeEdit(this);
    m_departureTimeEdit->setReadOnly(true);
    m_seatNumberSpin = new QSpinBox(this);
    m_seatNumberSpin->setRange(1, 100);
    m_passengerCountSpin = new QSpinBox(this);
    m_passengerCountSpin->setRange(1, 10);
    m_busTypeEdit = new QLineEdit(this);
    m_busTypeEdit->setReadOnly(true);
    m_farePerPassengerSpin = new QDoubleSpinBox(this);
    m_farePerPassengerSpin->setRange(0.0, 10000.0);
    m_totalFareSpin = new QDoubleSpinBox(this);
    m_totalFareSpin->setRange(0.0, 100000.0);
    m_totalFareSpin->setReadOnly(true);

    leftForm->addRow("Passenger ID", m_passengerIdEdit);
    leftForm->addRow("Ticket Number", m_ticketNumberEdit);
    leftForm->addRow("Route ID", m_routeIdEdit);
    leftForm->addRow("Bus ID", m_busIdEdit);
    leftForm->addRow("Source", m_sourceEdit);
    leftForm->addRow("Destination", m_destinationEdit);
    leftForm->addRow("Departure Time", m_departureTimeEdit);
    leftForm->addRow("Seat Number", m_seatNumberSpin);
    leftForm->addRow("Passengers", m_passengerCountSpin);
    leftForm->addRow("Bus Type", m_busTypeEdit);
    leftForm->addRow("Fare/Passenger", m_farePerPassengerSpin);
    leftForm->addRow("Total Fare", m_totalFareSpin);

    QWidget *seatPanel = new QWidget(this);
    QVBoxLayout *seatLayout = new QVBoxLayout(seatPanel);
    seatLayout->addWidget(new QLabel("Seat Selection"));
    m_seatWidget = new QWidget(this);
    seatLayout->addWidget(m_seatWidget);
    seatPanel->setLayout(seatLayout);

    formLayout->addLayout(leftForm, 2);
    formLayout->addWidget(seatPanel, 3);
    mainLayout->addLayout(formLayout);

    QHBoxLayout *actionLayout = new QHBoxLayout();
    QPushButton *bookButton = new QPushButton("Confirm Booking", this);
    m_generateTicketButton = new QPushButton("Generate Ticket", this);
    actionLayout->addWidget(bookButton);
    actionLayout->addWidget(m_generateTicketButton);
    mainLayout->addLayout(actionLayout);

    m_bookingTable = new QTableWidget(this);
    m_bookingTable->setColumnCount(6);
    m_bookingTable->setHorizontalHeaderLabels({"Booking ID", "Passenger ID", "Route", "Seat", "Status", "Date"});
    m_bookingTable->horizontalHeader()->setStretchLastSection(true);
    m_bookingTable->verticalHeader()->setVisible(false);
    m_bookingTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_bookingTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(m_bookingTable);

    connect(searchButton, &QPushButton::clicked, this, &BookingPage::onSearchRoutes);
    connect(m_routeTable, &QTableWidget::cellClicked, this, &BookingPage::onRouteSelected);
    connect(m_seatNumberSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &BookingPage::updateFare);
    connect(m_passengerCountSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &BookingPage::updateFare);
    connect(bookButton, &QPushButton::clicked, this, &BookingPage::onBookTicket);
    connect(m_generateTicketButton, &QPushButton::clicked, this, &BookingPage::onGenerateTicket);
}

void BookingPage::loadRoutes()
{
    QSqlQuery query(DatabaseManager::instance().database());
    if (query.exec("SELECT DISTINCT source FROM routes")) {
        m_searchSource->clear();
        while (query.next()) {
            m_searchSource->addItem(query.value(0).toString());
        }
    }
    query.exec("SELECT DISTINCT destination FROM routes");
    if (query.isActive()) {
        m_searchDestination->clear();
        while (query.next()) {
            m_searchDestination->addItem(query.value(0).toString());
        }
    }
}

void BookingPage::onSearchRoutes()
{
    refreshRouteTable();
}

void BookingPage::refreshRouteTable()
{
    m_routeTable->setRowCount(0);
    QString source = m_searchSource->currentText().trimmed();
    QString destination = m_searchDestination->currentText().trimmed();
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT route_id, source, destination, departure_time, arrival_time, base_fare, bus_id FROM routes WHERE source LIKE ? AND destination LIKE ?");
    query.addBindValue(source.isEmpty() ? "%" : source);
    query.addBindValue(destination.isEmpty() ? "%" : destination);
    if (query.exec()) {
        while (query.next()) {
            int row = m_routeTable->rowCount();
            m_routeTable->insertRow(row);
            for (int col = 0; col < 7; ++col) {
                m_routeTable->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
            }
        }
    }
}

void BookingPage::onRouteSelected(int row, int column)
{
    Q_UNUSED(column)
    m_selectedSeat = 0;
    resetSeatSelection();
    if (row < 0) {
        return;
    }
    m_routeIdEdit->setText(m_routeTable->item(row, 0)->text());
    m_sourceEdit->setText(m_routeTable->item(row, 1)->text());
    m_destinationEdit->setText(m_routeTable->item(row, 2)->text());
    m_departureTimeEdit->setTime(QTime::fromString(m_routeTable->item(row, 3)->text(), "HH:mm"));
    m_farePerPassengerSpin->setValue(m_routeTable->item(row, 5)->text().toDouble());
    m_busIdEdit->setText(m_routeTable->item(row, 6)->text());
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT bus_type FROM buses WHERE bus_id = ?");
    query.addBindValue(m_busIdEdit->text());
    if (query.exec() && query.next()) {
        m_busTypeEdit->setText(query.value(0).toString());
        QSqlQuery seatQuery(DatabaseManager::instance().database());
        seatQuery.prepare("SELECT total_seats FROM buses WHERE bus_id = ?");
        seatQuery.addBindValue(m_busIdEdit->text());
        if (seatQuery.exec() && seatQuery.next()) {
            int totalSeats = seatQuery.value(0).toInt();
            m_seatNumberSpin->setRange(1, totalSeats);
            refreshSeatButtons();
        }
    }
    updateFare();
}

void BookingPage::refreshSeatButtons()
{
    QLayout *layout = m_seatWidget->layout();
    if (layout) {
        QLayoutItem *item;
        while ((item = layout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete layout;
    }
    QGridLayout *grid = new QGridLayout(m_seatWidget);
    m_seatButtons.clear();
    int totalSeats = m_seatNumberSpin->maximum();
    QSet<int> bookedSeats;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT seat_number FROM bookings WHERE bus_id = ? AND travel_date = ? AND booking_status != 'Cancelled'");
    query.addBindValue(m_busIdEdit->text());
    query.addBindValue(m_travelDateEdit->date().toString("yyyy-MM-dd"));
    if (query.exec()) {
        while (query.next()) {
            bookedSeats.insert(query.value(0).toInt());
        }
    }
    int columns = 8;
    for (int seat = 1; seat <= totalSeats; ++seat) {
        QPushButton *button = new QPushButton(QString::number(seat), m_seatWidget);
        button->setCheckable(true);
        button->setFixedSize(40, 40);
        if (bookedSeats.contains(seat)) {
            button->setStyleSheet("background: #dc3545; color: white;");
            button->setEnabled(false);
        } else {
            button->setStyleSheet("background: #f1f5ff; color: #1c2541;");
            connect(button, &QPushButton::clicked, this, &BookingPage::onSeatButtonClicked);
        }
        int row = (seat - 1) / columns;
        int col = (seat - 1) % columns;
        grid->addWidget(button, row, col);
        m_seatButtons.append(button);
    }
    m_seatWidget->setLayout(grid);
}

void BookingPage::onSeatButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (!button) {
        return;
    }
    int seat = button->text().toInt();
    for (auto *btn : m_seatButtons) {
        if (btn != button) {
            btn->setChecked(false);
        }
    }
    m_selectedSeat = seat;
    m_seatNumberSpin->setValue(seat);
}

void BookingPage::resetSeatSelection()
{
    m_selectedSeat = 0;
    for (auto *button : m_seatButtons) {
        if (button) {
            button->setChecked(false);
        }
    }
}

void BookingPage::updateFare()
{
    double fare = m_farePerPassengerSpin->value();
    int count = m_passengerCountSpin->value();
    m_totalFareSpin->setValue(fare * count);
}

bool BookingPage::validateBooking()
{
    if (m_passengerIdEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation", "Passenger ID is required.");
        return false;
    }
    if (m_routeIdEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation", "Select a route before booking.");
        return false;
    }
    if (m_travelDateEdit->date() < QDate::currentDate()) {
        QMessageBox::warning(this, "Validation", "Travel date cannot be in the past.");
        return false;
    }
    int seat = m_seatNumberSpin->value();
    if (seat <= 0 || seat > m_seatNumberSpin->maximum()) {
        QMessageBox::warning(this, "Validation", "Seat number must be valid for the selected bus.");
        return false;
    }
    QSqlQuery check(DatabaseManager::instance().database());
    check.prepare("SELECT booking_id FROM bookings WHERE bus_id = ? AND travel_date = ? AND seat_number = ? AND booking_status != 'Cancelled'");
    check.addBindValue(m_busIdEdit->text());
    check.addBindValue(m_travelDateEdit->date().toString("yyyy-MM-dd"));
    check.addBindValue(seat);
    if (check.exec() && check.next()) {
        QMessageBox::warning(this, "Validation", "This seat is already booked for the selected travel date.");
        return false;
    }
    return true;
}

void BookingPage::onBookTicket()
{
    if (!validateBooking()) {
        return;
    }
    QString bookingId = DatabaseManager::instance().generateBookingId();
    QString ticket = DatabaseManager::instance().generateTicketNumber();
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("INSERT INTO bookings (booking_id, ticket_number, passenger_id, bus_id, route_id, source, destination, travel_date, departure_time, seat_number, passenger_count, bus_type, fare_per_passenger, total_fare, booking_status, booking_date) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(bookingId);
    query.addBindValue(ticket);
    query.addBindValue(m_passengerIdEdit->text().trimmed());
    query.addBindValue(m_busIdEdit->text().trimmed());
    query.addBindValue(m_routeIdEdit->text().trimmed());
    query.addBindValue(m_sourceEdit->text().trimmed());
    query.addBindValue(m_destinationEdit->text().trimmed());
    query.addBindValue(m_travelDateEdit->date().toString("yyyy-MM-dd"));
    query.addBindValue(m_departureTimeEdit->time().toString("HH:mm"));
    query.addBindValue(m_seatNumberSpin->value());
    query.addBindValue(m_passengerCountSpin->value());
    query.addBindValue(m_busTypeEdit->text().trimmed());
    query.addBindValue(m_farePerPassengerSpin->value());
    query.addBindValue(m_totalFareSpin->value());
    query.addBindValue("Confirmed");
    query.addBindValue(QDate::currentDate().toString("yyyy-MM-dd"));
    if (!query.exec()) {
        QMessageBox::critical(this, "Error", query.lastError().text());
        return;
    }
    QSqlQuery seatQuery(DatabaseManager::instance().database());
    seatQuery.prepare("INSERT INTO booked_seats (booking_id, bus_id, seat_number) VALUES (?, ?, ?)");
    seatQuery.addBindValue(bookingId);
    seatQuery.addBindValue(m_busIdEdit->text().trimmed());
    seatQuery.addBindValue(m_seatNumberSpin->value());
    seatQuery.exec();

    QSqlQuery paymentQuery(DatabaseManager::instance().database());
    paymentQuery.prepare("INSERT INTO payments (payment_id, booking_id, passenger_id, ticket_number, base_fare, reservation_charge, tax, discount, final_amount, payment_method, payment_status, payment_date) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    paymentQuery.addBindValue(DatabaseManager::instance().generatePaymentId());
    paymentQuery.addBindValue(bookingId);
    paymentQuery.addBindValue(m_passengerIdEdit->text().trimmed());
    paymentQuery.addBindValue(ticket);
    paymentQuery.addBindValue(m_totalFareSpin->value());
    paymentQuery.addBindValue(40.0);
    paymentQuery.addBindValue(60.0);
    paymentQuery.addBindValue(0.0);
    paymentQuery.addBindValue(m_totalFareSpin->value() + 100.0); // approximate
    paymentQuery.addBindValue("Cash");
    paymentQuery.addBindValue("Paid");
    paymentQuery.addBindValue(QDate::currentDate().toString("yyyy-MM-dd"));
    paymentQuery.exec();

    QMessageBox::information(this, "Success", "Ticket booked successfully.");
    m_ticketNumberEdit->setText(ticket);
    refreshSeatButtons();
}

void BookingPage::onGenerateTicket()
{
    if (m_ticketNumberEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Ticket", "Please book a ticket first.");
        return;
    }
    Ticket ticket(m_ticketNumberEdit->text().trimmed(), m_routeIdEdit->text().trimmed(), m_passengerIdEdit->text().trimmed(), m_busIdEdit->text().trimmed(), m_busIdEdit->text().trimmed(), m_busTypeEdit->text().trimmed(), m_sourceEdit->text().trimmed(), m_destinationEdit->text().trimmed(), m_travelDateEdit->date().toString("dd-MM-yyyy"), m_departureTimeEdit->time().toString("HH:mm"), m_seatNumberSpin->value(), m_passengerCountSpin->value(), m_totalFareSpin->value(), "Paid", "Confirmed");
    QString report = ticket.generateReport();
    QString filePath = QDir::homePath() + "/SmartBus_Ticket_" + m_ticketNumberEdit->text().trimmed() + ".txt";
    QFile file(filePath);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&file);
        out << report;
        file.close();
        QMessageBox::information(this, "Exported", "Ticket exported to " + filePath);
    } else {
        QMessageBox::warning(this, "Export", "Unable to write ticket file.");
    }
}
