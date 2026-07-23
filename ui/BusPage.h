#ifndef BUSPAGE_H
#define BUSPAGE_H

#include <QWidget>
#include <QObject>

class QLineEdit;
class QComboBox;
class QSpinBox;
class QTableWidget;
class QPushButton;

class BusPage : public QWidget
{
    Q_OBJECT

public:
    BusPage(QWidget *parent = nullptr);

private slots:
    void onAddBus();
    void onUpdateBus();
    void onDeleteBus();
    void onSearchTextChanged(const QString &text);
    void onFilterTypeChanged(const QString &type);
    void onTableCellClicked(int row, int column);

private:
    void setupUi();
    void refreshTable();
    bool validateFields();

    QLineEdit *m_idEdit = nullptr;
    QLineEdit *m_numberEdit = nullptr;
    QLineEdit *m_nameEdit = nullptr;
    QComboBox *m_typeCombo = nullptr;
    QSpinBox *m_seatsSpin = nullptr;
    QLineEdit *m_registrationEdit = nullptr;
    QLineEdit *m_driverNameEdit = nullptr;
    QLineEdit *m_driverPhoneEdit = nullptr;
    QComboBox *m_statusCombo = nullptr;
    QLineEdit *m_searchEdit = nullptr;
    QComboBox *m_filterTypeCombo = nullptr;
    QTableWidget *m_table = nullptr;
};

#endif // BUSPAGE_H
