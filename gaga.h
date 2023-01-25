//
// Created by ligh1 on 1/25/2023.
//

#ifndef UNTITLED23_GAGA_H
#define UNTITLED23_GAGA_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class gaga; }
QT_END_NAMESPACE

class gaga : public QWidget {
Q_OBJECT

public:
    explicit gaga(QWidget *parent = nullptr);

    ~gaga() override;

private:
    Ui::gaga *ui;
};


#endif //UNTITLED23_GAGA_H
