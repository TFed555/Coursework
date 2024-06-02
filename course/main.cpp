#include "autho.h"
//#include "worksmodel.h"
#include "db.h"
#include <QApplication>


inline void initMyResource() { Q_INIT_RESOURCE(); }

int main(int argc, char *argv[])
{
    DataBase *db;
    db = new DataBase();
    db->connectToDataBase();
    QApplication a(argc, argv);
//    a.setQuitOnLastWindowClosed(false);
    Autho w;
    w.show();
    return a.exec();
}
