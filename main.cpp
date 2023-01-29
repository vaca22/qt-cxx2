#include <QApplication>
#include <QPushButton>
#include <iostream>
#include "gaga.h"


using namespace std;
int main(int argc, char *argv[]) {


//    cout<<result<<endl;


    QApplication a(argc, argv);
    gaga button( nullptr);
    button.resize(600, 300);
    button.show();
    return QApplication::exec();
}
