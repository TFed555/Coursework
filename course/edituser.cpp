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
        confirmStatus(userID);
        ui->checkBox->setChecked(false);
        ui->checkBox_2->setChecked(false);
       // emit UsersWindow();
    });

//    connect(ui->confirmButton, &QPushButton::clicked, this, [this, us])
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
    DataBase conn;
    QSqlQuery* query = new QSqlQuery(conn.db);
     query->prepare("Select Surname, Name, Patronymic, phoneNumber, Post "
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
         ui->post->setText(query->value(4).toString());
         fillCheckbox(userID);
         //confirmStatus(userID);
     }

}

void EditUser::updateUser(int userID){
    DataBase conn;
    if (ui->checkBox->isChecked()){
        QSqlQuery* query = new QSqlQuery(conn.db);
        query->prepare("Select Role_ID from Users "
                       "Join Roles on Roles.Role_ID = Users.Role "
                        "Where Users.ID = :userID");
        query->bindValue(":userID", userID);
        if(!query->exec()){
            qDebug()<< query->lastError().text();
        }
        query->next();
        QVariant role = query->value(0).toInt();
        if (role == 1) role = 2;
        else if (role == 2) role = 1;

        query->prepare("Update Users "
                       "Set Role = :roleID "
                       "Where ID = :userID");
        query->bindValue(":userID", userID);
        query->bindValue(":roleID", role);
        if (!query->exec()){
            qDebug()<< query->lastError().text();
        }
        fillCheckbox(userID);
    }
}

void EditUser::on_confirmButton_clicked()
{
   // this->close();
}

void EditUser::fillCheckbox(int userID){
    DataBase conn;
    QSqlQuery* query = new QSqlQuery(conn.db);
    query->prepare("Select Role_ID from Users "
                   "Join Roles on Roles.Role_ID = Users.Role "
                    "Where Users.ID = :userID");
    query->bindValue(":userID", userID);
    if(!query->exec()){
        qDebug()<< query->lastError().text();
    }
    query->next();
    QVariant role = query->value(0).toInt();
    if (role==3) {
        ui->checkBox->setText("Назначить организатором");
        ui->checkBox->setEnabled(false);
    }
    if (role==2) ui->checkBox->setText("Назначить сотрудником");
    else ui->checkBox->setText("Назначить организатором");
}

void EditUser::confirmStatus(int userID){
    DataBase conn;
    if (ui->checkBox_2->isChecked()){
        QSqlQuery* query = new QSqlQuery(conn.db);
        QString post = ui->comboBox->currentText();
        query->prepare("Update Users "
                       "Set Post = :post "
                       "Where ID = :userID ");
        query->bindValue(":userID", userID);
        query->bindValue(":post", post);
        if(!query->exec()){
            qDebug()<<query->lastError().text();
        }
        ui->post->setText(post);
    }
}
