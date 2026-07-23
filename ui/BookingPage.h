#ifndef BOOKINGPAGE_H
#define BOOKINGPAGE_H

#include <QWidget>
#include <QObject>

class QLineEdit;
class QComboBox;
class QDateEdit;
class QTimeEdit;
class QSpinBox;
class QDoubleSpinBox;
class QTableWidget;
class QPushButton;
class QWidget;

class BookingPage : public QWidget
{
    Q_OBJECT

public:
    BookingPage(QWidget *parent = nullptr);

private slots:
    void onSearchRoutes();
    void onRouteSelected(int row, int column);
    void onSeatButtonClicked();
    void onBookTicket();
    void onGenerateTicket();

private:
    void setupUi();
    void refreshRouteTable();
    void refreshSeatButtons();
    bool validateBooking();
    void updateFare();
    void loadRoutes();
    void resetSeatSelection();

    QLineEdit *m_passengerIdEdit = nullptr;
    QLineEdit *m_ticketNumberEdit = nullptr;
    QComboBox *m_searchSource = nullptr;
    QComboBox *m_searchDestination = nullptr;
    QDateEdit *m_travelDateEdit = nullptr;
    QLineEdit *m_routeIdEdit = nullptr;
    QLineEdit *m_busIdEdit = nullptr;
    QLineEdit *m_sourceEdit = nullptr;
    QLineEdit *m_destinationEdit = nullptr;
    QTimeEdit *m_departureTimeEdit = nullptr;
    QSpinBox *m_seatNumberSpin = nullptr;
    QSpinBox *m_passengerCountSpin = nullptr;
    QLineEdit *m_busTypeEdit = nullptr;
    QDoubleSpinBox *m_farePerPassengerSpin = nullptr;
    QDoubleSpinBox *m_totalFareSpin = nullptr;
    QTableWidget *m_routeTable = nullptr;
    QWidget *m_seatWidget = nullptr;
    QTableWidget *m_bookingTable = nullptr;
    QVector<QPushButton*> m_seatButtons;
    int m_selectedSeat = 0;
    QPushButton *m_generateTicketButton = nullptr;
};

#endif // BOOKINGPAGE_H
