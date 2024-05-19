#include "descwork.h"
#include "ui_descwork.h"

DescWork::DescWork(int workID, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DescWork)
{
    ui->setupUi(this);
    this->setupData(workID);
    connect(ui->acceptButton, &QPushButton::clicked, this, [this, workID]() {
        this->confirmChange(workID);
    });
//    model = new WorksModel();
}

DescWork::~DescWork()
{
    delete ui;
}


void DescWork::setupData(int workID){
    DataBase conn;
    QSqlQuery* query = new QSqlQuery(conn.db);
     query->prepare("Select Title, Description, Deadline, Pay, Users.Name, Users.Surname "
                    " From Works join Users On Users.ID = Works.Responsible "
                    "Where Works.ID = :workID" );
     query->bindValue(":workID", workID);
     if(!query->exec()){
         qDebug()<<query->lastError().text();
     }
     else{
         query->next();
         if (query->value(4).toInt()==3){
             ui->acceptButton->setDisabled(true);
         }
         //форматирование textbrowser
         ui->textBrowser->setFont(QFont("Times", 9));
         QString title = "<b>Название</b> " + QString("%1").arg(query->value(0).toString());
         QString desc = "<b>Описание</b> " + QString("%1").arg(query->value(1).toString());
         QString deadline = "<b>Срок</b> " + QString("%1").arg(query->value(2).toString());
         QString pay = "<b>Оплата</b> " + QString("%1").arg(query->value(3).toString());
         QString resp = "<b>Ответственный</b> " + QString("%1 %2").arg(query->value(4).toString(), query->value(5).toString());
         ui->textBrowser->append(title);
         ui->textBrowser->append(desc);
         ui->textBrowser->append(deadline);
         ui->textBrowser->append(pay);
         ui->textBrowser->append(resp);
     }
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

