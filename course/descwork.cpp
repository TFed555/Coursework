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
    delete query;
}

void DescWork::initialize(){
    setupData(workID);
}


bool DescWork::setupData(int workID){
         if(!AbstractWork::setupData(workID)){
             return false;
         }
         query->next();
         int status = query->value(4).toInt();
         if (status==3){
             ui->acceptButton->setDisabled(true);
         }
         connect(this, &DescWork::rejected, [this, workID, status]{
             this->cancelChange(workID, status);
         });
         //формирование textbrowser
         QString title = "<b>Название</b> " + QString("%1").arg(query->value(0).toString());
         QString desc = "<b>Описание</b> " + QString("%1").arg(query->value(1).toString());
         QString deadline = "<b>Срок</b> " + QString("%1").arg(query->value(2).toString());
         QString pay = "<b>Оплата</b> " + QString("%1").arg(query->value(3).toString());
         QString resp = "<b>Ответственный</b> ";
         QList <QString> data = {title, desc, deadline, pay, resp};
         if (!query->value(5).isNull()){
             resp += QString("%1 %2 %3").arg(query->value(6).toString(), query->value(5).toString(),
                    query->value(7).toString());
            data[4] = resp;
          }
         else{
             resp += "Ещё не назначен";
             data[4] = resp;
         }
         if (query->next()){
             QString resp_2 = "<b>Второй ответственный</b> " + QString("%1 %2 %3").arg(query->value(6).toString(), query->value(5).toString(),
                                                                                query->value(7).toString());
             data.append(resp_2);
         }
         setTextBrowser(data);
    return true;
}

void DescWork::setTextBrowser(QList<QString> data){
    ui->textBrowser->setFont(QFont("Times", 9));
    for (const QString& item : data){
        ui->textBrowser->append(item);
    }
}

bool DescWork::checkUserID(int workID){
    int userId;
    query->prepare("Select Users.ID "
                    "From Users Where Users.phoneNumber = :login");
    query->bindValue(":login", login);
    query->exec();
    query->next();
    userId = query->value(0).toInt();
    query->prepare("Select Tasks.Responsible "
                   "From Tasks LEFT JOIN Users On Users.ID = Tasks.Responsible "
                                "Where Tasks.Work = :workID");
    query->bindValue(":workID", workID);
    query->exec();
    query->next();
   int respId = query->value(0).toInt();
   if (respId == userId){
        return true;
   }
   return false;
}

void DescWork::confirmChange(int workID){
    query->prepare("Update Works "
                       "Set Status = 3 "
                       "Where Works.ID = :workID" );
    query->bindValue(":workID", workID);
    if(!query->exec()){
            qDebug()<<query->lastError().text();
            return;
    }
    ui->acceptButton->setEnabled(false);
    emit updatedWorkStatus(workID, 3);
}

void DescWork::on_buttonBox_accepted()
{
    accept();
}


void DescWork::on_buttonBox_rejected()
{
    reject();
}

void DescWork::cancelChange(int workID, int status){
    query->prepare("Update Works "
                       "Set Status = :status "
                       "Where Works.ID = :workID" );
    query->bindValue(":status", status);
    query->bindValue(":workID", workID);
    if(!query->exec()){
            qDebug()<<query->lastError().text();
            return;
    }
    ui->acceptButton->setEnabled(true);
    emit updatedWorkStatus(workID, status);
}
