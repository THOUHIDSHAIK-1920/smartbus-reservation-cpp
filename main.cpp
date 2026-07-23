#include <QApplication>
#include "ui/MainWindow.h"
#include "database/DatabaseManager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("SmartBus");
    QApplication::setFont(QFont("Segoe UI", 9));

    if (!DatabaseManager::instance().initialize()) {
        return -1;
    }

    MainWindow mainWindow;
    mainWindow.showMaximized();

    return app.exec();
}
