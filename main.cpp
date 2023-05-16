#include <QApplication>
#include <QPushButton>
#include <iostream>
#include "ui/gaga.h"
#include "test_ws.h"
#include "test_juice.h"


using namespace std;
int main(int argc, char *argv[]) {
    test_connectivity();
    start_test_ws();
    QApplication a(argc, argv);
    gaga button( nullptr);
    button.resize(600, 300);
    button.show();
    return QApplication::exec();
}
