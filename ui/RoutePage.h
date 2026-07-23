#ifndef ROUTEPAGE_H
#define ROUTEPAGE_H

#include <QWidget>
#include <QObject>

class QLineEdit;
class QTimeEdit;
class QDoubleSpinBox;
class QComboBox;
class QTableWidget;

class RoutePage : public QWidget
{
    Q_OBJECT

public:
    RoutePage(QWidget *parent = nullptr);

private slots:
    void onAddRoute();
    void onUpdateRoute();
    void onDeleteRoute();
    void onSearchTextChanged(const QString &text);
    void onTableCellClicked(int row, int column);

private:
    void setupUi();
    void refreshTable();
    bool validateFields();

    QLineEdit *m_idEdit = nullptr;
    QLineEdit *m_sourceEdit = nullptr;
    QLineEdit *m_destinationEdit = nullptr;
    QTimeEdit *m_departureTimeEdit = nullptr;
    QTimeEdit *m_arrivalTimeEdit = nullptr;
    QDoubleSpinBox *m_distanceSpin = nullptr;
    QDoubleSpinBox *m_baseFareSpin = nullptr;
    QComboBox *m_busCombo = nullptr;
    QLineEdit *m_travelDurationEdit = nullptr;
    QLineEdit *m_searchEdit = nullptr;
    QLineEdit *m_filterSourceEdit = nullptr;
    QLineEdit *m_filterDestinationEdit = nullptr;
    QTableWidget *m_table = nullptr;
};

#endif // ROUTEPAGE_H
