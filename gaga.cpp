//
// Created by ligh1 on 1/25/2023.
//

// You may need to build the project (run Qt uic code generator) to get "ui_gaga.h" resolved

#include <iostream>
#include "gaga.h"
#include "ui_gaga.h"
#include "chem/chem.h"

using namespace std;

gaga::gaga(QWidget *parent) :
        QWidget(parent), ui(new Ui::gaga) {
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked(bool)),this, SLOT(on_pushButton_clicked()));
}

gaga::~gaga() {
    delete ui;
}

void gaga::on_pushButton_clicked() {
    std::cout<<"fuck"<<endl;
    QString name=ui->plainTextEdit->toPlainText();
    ChemEquation *chemEquation = new ChemEquation(name.toStdString().c_str());
    string result=chemEquation->myResult();
    ui->label_2->setText(QString::fromStdString(result));
    delete chemEquation;
}
