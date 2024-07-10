#include <QApplication>
#include "mainapplication.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainApplication mainApplication;
    mainApplication.show();

    return app.exec();
}
