#include "autho.h"
#include "db.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    DataBase *db;
    db = new DataBase();
    db->connectToDataBase();
    QApplication a(argc, argv);
    Autho w;
    w.show();
    return a.exec();
}
