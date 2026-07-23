#ifndef PAYMENTPAGE_H
#define PAYMENTPAGE_H

#include <QWidget>
#include <QObject>

class QTableWidget;
class QLineEdit;
class QComboBox;
class QDoubleSpinBox;
class QPushButton;

class PaymentPage : public QWidget
{
    Q_OBJECT

public:
    PaymentPage(QWidget *parent = nullptr);

private slots:
    void onSearchPayment();
    void onUpdateStatus();

private:
    void setupUi();
    void refreshTable();

    QLineEdit *m_searchEdit = nullptr;
    QComboBox *m_statusCombo = nullptr;
    QComboBox *m_methodCombo = nullptr;
    QTableWidget *m_table = nullptr;
};

#endif // PAYMENTPAGE_H
