#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.setWindowTitle(QString::fromStdString("科学计算器"));
    w.setWindowIcon(QIcon("://resources/icon.ico"));
    return a.exec();
}
