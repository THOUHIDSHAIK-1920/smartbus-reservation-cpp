#ifndef REPORTSPAGE_H
#define REPORTSPAGE_H

#include <QWidget>
#include <QObject>

class QTableWidget;
class QLineEdit;
class QComboBox;
class QPushButton;

class ReportsPage : public QWidget
{
    Q_OBJECT

public:
    ReportsPage(QWidget *parent = nullptr);

private slots:
    void onSearchReports();
    void onExportCsv();

private:
    void setupUi();
    void refreshTable();

    QLineEdit *m_searchEdit = nullptr;
    QComboBox *m_reportCombo = nullptr;
    QTableWidget *m_table = nullptr;
    QPushButton *m_exportButton = nullptr;
};

#endif // REPORTSPAGE_H
