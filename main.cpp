#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qInfo() << "Program Start";
    MainWindow w;

    w.show();

    return a.exec();
}
