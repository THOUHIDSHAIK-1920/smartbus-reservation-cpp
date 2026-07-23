#ifndef PASSENGERPAGE_H
#define PASSENGERPAGE_H

#include <QWidget>
#include <QObject>

class QLineEdit;
class QComboBox;
class QSpinBox;
class QTableWidget;
class QPushButton;

class PassengerPage : public QWidget
{
    Q_OBJECT

public:
    PassengerPage(QWidget *parent = nullptr);

private slots:
    void onAddPassenger();
    void onUpdatePassenger();
    void onDeletePassenger();
    void onSearchTextChanged(const QString &text);
    void onTableCellClicked(int row, int column);

private:
    void setupUi();
    void refreshTable();
    bool validateFields();

    QLineEdit *m_idEdit = nullptr;
    QLineEdit *m_nameEdit = nullptr;
    QSpinBox *m_ageSpin = nullptr;
    QComboBox *m_genderCombo = nullptr;
    QLineEdit *m_phoneEdit = nullptr;
    QLineEdit *m_emailEdit = nullptr;
    QLineEdit *m_addressEdit = nullptr;
    QLineEdit *m_idProofTypeEdit = nullptr;
    QLineEdit *m_idProofNumberEdit = nullptr;
    QLineEdit *m_searchEdit = nullptr;
    QTableWidget *m_table = nullptr;
};

#endif // PASSENGERPAGE_H
