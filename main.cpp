#include <QApplication>
#include <QPushButton>
#include <iostream>
#include "gaga.h"
#include "chem/chem.h"

using namespace std;
int main(int argc, char *argv[]) {
    ChemEquation *chemEquation = new ChemEquation("H2+O2=H2O");
    string result=chemEquation->myResult();
    delete chemEquation;

//    cout<<result<<endl;


    QApplication a(argc, argv);
    gaga button( nullptr);
    button.resize(200, 100);
    button.show();
    return QApplication::exec();
}
