#include "descwork.h"
#include "ui_descwork.h"

DescWork::DescWork(QString currentLogin, int workID, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DescWork),
    login(currentLogin)
{
    ui->setupUi(this);
    this->setupData(workID);
    connect(ui->acceptButton, &QPushButton::clicked, this, [this, workID]() {
        this->confirmChange(workID);
    });
    if(!checkUserID(workID)){
        ui->acceptButton->setDisabled(true);
    }
//    connect(this, &DescWork::rejected, this, );
//    model = new WorksModel();
}

DescWork::~DescWork()
{
    delete ui;
}


void DescWork::setupData(int workID){
    DataBase conn;
    QSqlQuery* query = new QSqlQuery(conn.db);
     query->prepare("Select Title, Description, Deadline, Pay, Status, Users.Name, Users.Surname, Users.Patronymic "
                    " From Works "
                        "JOIN Tasks On Tasks.Work = Works.ID "
                            "JOIN Users On Tasks.Responsible = Users.ID Where Works.ID = :workID" );
     query->bindValue(":workID", workID);
     if(!query->exec()){
         qDebug()<<query->lastError().text();
     }
     else{
         query->next();
         int status = query->value(4).toInt();
         if (status==3){
             ui->acceptButton->setDisabled(true);
         }
         connect(this, &DescWork::rejected, [this, workID, status]{
             this->cancelChange(workID, status);
         });
         //формирование textbrowser
         ui->textBrowser->setFont(QFont("Times", 9));
         QString title = "<b>Название</b> " + QString("%1").arg(query->value(0).toString());
         QString desc = "<b>Описание</b> " + QString("%1").arg(query->value(1).toString());
         QString deadline = "<b>Срок</b> " + QString("%1").arg(query->value(2).toString());
         QString pay = "<b>Оплата</b> " + QString("%1").arg(query->value(3).toString());
         QString resp = "<b>Ответственный</b> " + QString("%1 %2 %3").arg(query->value(6).toString(), query->value(5).toString(),
                 query->value(7).toString());
         ui->textBrowser->append(title);
         ui->textBrowser->append(desc);
         ui->textBrowser->append(deadline);
         ui->textBrowser->append(pay);
         ui->textBrowser->append(resp);
         if (query->next()){
             QString resp_2 = "<b>Второй ответственный</b> " + QString("%1 %2 %3").arg(query->value(6).toString(), query->value(5).toString(),
                                                                                query->value(7).toString());
             ui->textBrowser->append(resp_2);
         }
     }
}

bool DescWork::checkUserID(int workID){
    int userId;
    DataBase conn;
    QSqlQuery* query = new QSqlQuery(conn.db);
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
    DataBase conn;
    QSqlQuery* query = new QSqlQuery(conn.db);
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
    DataBase conn;
    QSqlQuery* query = new QSqlQuery(conn.db);
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
