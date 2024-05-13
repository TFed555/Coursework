#include "autho.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Autho w;
    w.show();
    return a.exec();
}
