#include <QApplication>
#include <QPushButton>
#include "gaga.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    gaga button( nullptr);
    button.resize(200, 100);
    button.show();
    return QApplication::exec();
}
