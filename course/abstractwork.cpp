#include "abstractwork.h"

AbstractWork::AbstractWork(int workID, QWidget *parent)
    : QDialog(parent), workID(workID)
{

}

AbstractWork::~AbstractWork(){
    delete db;
}

bool AbstractWork::setupData(int workID){
    tasks = db->selectTasks(workID);
    if (tasks.isEmpty()){
        return false;
    }
    return true;
}

void AbstractWork::rejectAction(){
    int reply = msgbx.showWarningBox("Изменения не были сохранены, продолжить?");
    if (reply==QMessageBox::Ok){
        reject();
    }
}
