#include "descwork.h"
#include "ui_descwork.h"

DescWork::DescWork(QString currentLogin, int workID, QWidget *parent) :
    AbstractWork(workID, parent),
    ui(new Ui::DescWork),
    login(currentLogin)
{
    ui->setupUi(this);
    connect(ui->acceptButton, &QPushButton::clicked, this, [this, workID]() {
        this->confirmChange(workID);
    });
    if(!checkUserID(workID)){
        ui->acceptButton->setDisabled(true);
    }
}

DescWork::~DescWork()
{
    delete ui;
    delete db;
}

void DescWork::initialize(){
    setupData(workID);
}


bool DescWork::setupData(int workID){
         if(!AbstractWork::setupData(workID)){
             return false;
         }
         int status = tasks[0][4].toInt();
         if (status==3){
              ui->acceptButton->setDisabled(true);
         }
         connect(this, &DescWork::rejected, [this, workID, status]{
                      this->cancelChange(workID, status);
                  });
         //формирование textbrowser
          QString title = "<b>Название</b> " + QString("%1").arg(tasks[0][0].toString());
          QString desc = "<b>Описание</b> " + QString("%1").arg(tasks[0][1].toString());
          QString deadline = "<b>Срок</b> " + QString("%1").arg(tasks[0][2].toString());
          QString pay = "<b>Оплата</b> " + QString("%1").arg(tasks[0][3].toString());
          QString resp = "<b>Ответственный</b> ";
          QList <QString> data = {title, desc, deadline, pay, resp};
          if (!tasks[0][5].isNull()){
              resp += QString("%1 %2 %3").arg(tasks[0][6].toString(), tasks[0][5].toString(),
                                  tasks[0][7].toString());
              data[4] = resp;
          }
          else{
                resp += "Ещё не назначен";
                data[4] = resp;
           }
          if (tasks.length()>1){
             QString resp_2 = "<b>Второй ответственный</b> " + QString("%1 %2 %3").arg(tasks[1][6].toString(), tasks[1][5].toString(),
                     tasks[1][7].toString());
               data.append(resp_2);
                   }
          setTextBrowser(data);
    return true;
}


//вынести в абстрактный
void DescWork::setTextBrowser(QList<QString> data){
    ui->textBrowser->setFont(QFont("Times", 9));
    for (const QString& item : data){
        ui->textBrowser->append(item);
    }
}

bool DescWork::checkUserID(int workID){
   int userId = db->checkUserID(login);
   int respId = db->getResponsible(workID);
   if (respId == userId){
        return true;
   }
   return false;
}

bool DescWork::confirmChange(int workID){
    if (!db->updateWorkStatus(workID, 3)){
        return false;
    }
    ui->acceptButton->setEnabled(false);
    emit updatedWorkStatus(workID, 3);
    return true;
}

void DescWork::on_buttonBox_accepted()
{
    accept();
}


void DescWork::on_buttonBox_rejected()
{
    //возникает ошибка при работе с proxymodel
    reject();
}

void DescWork::rejectAction(){

}

void DescWork::cancelChange(int workID, int status){
    if (db->updateWorkStatus(workID, status)){
        ui->acceptButton->setEnabled(true);
        emit updatedWorkStatus(workID, status);
    }
}
