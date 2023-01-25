//
// Created by ligh1 on 1/25/2023.
//

// You may need to build the project (run Qt uic code generator) to get "ui_gaga.h" resolved

#include "gaga.h"
#include "ui_gaga.h"


gaga::gaga(QWidget *parent) :
        QWidget(parent), ui(new Ui::gaga) {
    ui->setupUi(this);
}

gaga::~gaga() {
    delete ui;
}
