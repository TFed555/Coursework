#include "abstractwork.h"

AbstractWork::AbstractWork(int workID, QWidget *parent)
    : QDialog(parent), workID(workID)
{

}

AbstractWork::~AbstractWork(){
    delete db;
    delete query;
}

bool AbstractWork::setupData(int workID){
     query->prepare("Select Title, Description, Deadline, Pay, Status, Users.Name, Users.Surname, Users.Patronymic, Status, Tasks.Responsible "
                    " From Works "
                        "LEFT JOIN Tasks On Tasks.Work = Works.ID "
                            "LEFT JOIN Users On Tasks.Responsible = Users.ID Where Works.ID = :workID" );
     query->bindValue(":workID", workID);
     if(!query->exec()){
         qDebug()<<query->lastError().text();
         return false;
     }
     return true;
}
