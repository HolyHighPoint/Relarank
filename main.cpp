#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    // create application
    QApplication app(argc, argv);
    app.setOrganizationName("Holy High Point");
    app.setOrganizationDomain("github.com/holyhighpoint");
    app.setApplicationName("Relarank");
    app.setWindowIcon(QIcon(":/icons/QT.png"));
    // create the main window and enter the main execution loop
    MainWindow window;
    window.show();
    int result = app.exec();
    return result;
}
