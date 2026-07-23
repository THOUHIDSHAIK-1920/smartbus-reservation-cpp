#ifndef RESERVATIONPAGE_H
#define RESERVATIONPAGE_H

#include <QWidget>
#include <QObject>

class QTableWidget;
class QPushButton;
class QLineEdit;
class QComboBox;

class ReservationPage : public QWidget
{
    Q_OBJECT

public:
    ReservationPage(QWidget *parent = nullptr);

private slots:
    void onSearchBooking();
    void onCancelBooking();
    void onCompleteBooking();

private:
    void setupUi();
    void refreshTable();
    QTableWidget *m_table = nullptr;
    QLineEdit *m_searchEdit = nullptr;
    QComboBox *m_statusCombo = nullptr;
};

#endif // RESERVATIONPAGE_H
