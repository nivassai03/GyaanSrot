#include <QApplication>
#include "application.h"
#include "gui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mainwindow;
    mainwindow.show();

    Application application;
    application.start();

    return app.exec();
}
