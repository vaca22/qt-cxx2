#include <QApplication>
#include <QPushButton>
#include <iostream>
#include "ui/gaga.h"


using namespace std;
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    gaga button( nullptr);
    button.resize(600, 300);
    button.show();
    return QApplication::exec();
}
