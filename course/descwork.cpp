#include "descwork.h"
#include "ui_descwork.h"

DescWork::DescWork(int workID, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DescWork)
{
    ui->setupUi(this);
    db = new DataBase();
    db->connectToDataBase();
    this->setupData(workID);
    connect(ui->acceptButton, &QPushButton::clicked, this, [this, workID]() {
        this->confirmChange(workID);
    });
}

DescWork::~DescWork()
{
    delete ui;
}


void DescWork::setupData(int workID){
    DataBase conn;
    QSqlQuery* query = new QSqlQuery(conn.db);
     query->prepare("Select Title, Description, Deadline, Pay "
                    " From Works Where Works.ID = :workID" );
     query->bindValue(":workID", workID);
     if(!query->exec()){
         qDebug()<<"err";
     }
     else{
         query->next();
         ui->textBrowser->append(query->value(0).toString());
         ui->textBrowser->append(query->value(1).toString());
         ui->textBrowser->append(query->value(2).toString());
         ui->textBrowser->append(query->value(3).toString());
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
        qDebug()<<"err";
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

