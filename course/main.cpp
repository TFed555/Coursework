#include "autho.h"
#include <memory>
#include "db.h"
#include <QApplication>



int main(int argc, char *argv[])
{
    std::shared_ptr<DataBase> db;
    db = std::make_shared<DataBase>();
    db->connectToDataBase();
    QApplication a(argc, argv);
    Autho w;
    w.show();
    return a.exec();
}
