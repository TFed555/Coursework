#include "edituser.h"
#include "ui_edituser.h"

EditUser::EditUser(int userID, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditUser)
{
    ui->setupUi(this);
    this->setupFields(userID);
    connect(ui->confirmButton, &QPushButton::clicked, this, [this, userID](){
        updateUser(userID);
    });
}

EditUser::~EditUser()
{
    delete ui;
//    db->closeDataBase();
//    delete db;
}

void EditUser::on_backButton_clicked()
{
    this->close();
    emit UsersWindow();
}

void EditUser::setupFields(int userID)
{
    db = new DataBase();
    db->connectToDataBase();
    DataBase conn;
    QSqlQuery* query = new QSqlQuery(conn.db);
     query->prepare("Select Surname, Name, Patronymic, phoneNumber "
                    " From Users Where ID = :userID" );
     query->bindValue(":userID", userID);
     if(!query->exec()){
         qDebug()<<"err";
     }
     else{
         query->next();
         ui->surname->setText(query->value(0).toString());
         ui->name->setText(query->value(1).toString());
         ui->patronymic->setText(query->value(2).toString());
         ui->phoneNumber->setText(query->value(3).toString());
     }

}

void EditUser::updateUser(int userID){
    DataBase conn;
    if (ui->checkBox->isChecked()){
        QSqlQuery* query = new QSqlQuery(conn.db);
        query->prepare("Update Users "
                       "Set Role = 2 "
                       "Where ID = :userID");
        query->bindValue(":userID", userID);
    if (!query->exec()){
        qDebug()<< query->lastError().text();
    }
    }
}

void EditUser::on_confirmButton_clicked()
{
    this->close();
    emit UsersWindow();
}

