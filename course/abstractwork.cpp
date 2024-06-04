#include "abstractwork.h"

Work::Work(int workID, QWidget *parent)
    : QDialog(parent), workID(workID)
{

}

Work::~Work(){
}

bool Work::setupData(int workID){
    tasks = db->selectTasks(workID);
    if (tasks.isEmpty()){
        return false;
    }
    return true;
}

void Work::rejectAction(){
    int reply = msgbx.showWarningBoxWithCancel("Изменения не были сохранены, продолжить?");
    if (reply==QMessageBox::Ok){
        reject();
    }
}

void Work::setTextBrowser(QList<QString> data, QTextBrowser *browser){
    browser->setFont(QFont("Cascadia Code", 8));
    browser->clear();
    for (const QString& item : data){
        browser->append(item);
    }
}
