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
}

DescWork::~DescWork()
{
    delete ui;
}


void DescWork::setupData(int workID){
    DataBase conn;
    QSqlQuery* query = new QSqlQuery(conn.db);
     query->prepare("Select Title, Description "
                    " From Works Where Works.ID = :workID" );
     query->bindValue(":workID", workID);
     if(!query->exec()){
         qDebug()<<"err";
     }
     else{
         query->next();
         ui->textBrowser->append(query->value(0).toString());
         ui->textBrowser->append(query->value(1).toString());
     }
}

void DescWork::on_buttonBox_accepted()
{
    emit(WorksWindow());
    this->close();
}

