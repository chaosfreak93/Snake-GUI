#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowIcon(QIcon("snake.ico"));
    w.setFocus();
    w.setFixedSize(600, 415);
    w.setWindowTitle("Snake");
    w.show();

    return a.exec();
}
