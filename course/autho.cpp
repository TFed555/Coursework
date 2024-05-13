#include "autho.h"
#include "ui_autho.h"

Autho::Autho(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Autho),
      phoneValidator(QRegExp("^\\+\\d{1}\\d{10}$"))
{
    ui->setupUi(this);

    usersUi = new UsersWindow();
    worksUi = new WorksWindow();
    regUi = new Registration();
    connect(regUi, &Registration::AuthoWindow, this, &Autho::show);
    ui->loginEdit->setValidator(&phoneValidator);
}

Autho::~Autho()
{
    delete ui;
}


void Autho::on_authButton_clicked()
{
    CustomBox msgbx;
    db = new DataBase();
    db->connectToDataBase();
//  db->deleteTable("Users");
//   db->deleteTable("Roles");
//   db->deleteTable("Works");
//   db->deleteTable("Status");
    if(db->loginExists(ui->loginEdit->text())){
        if (db->pswdCompare(ui->loginEdit->text(), ui->pswdEdit->text())){
          switch(db->getRole(ui->loginEdit->text())){
          case 1:
              worksUi->show();
              this->hide();
              break;
          case 2:
               qDebug()<<"not ready yet";
              break;
          case 3:
                usersUi->updateModel();
                usersUi->show();
                this->hide();
                break;
          }
        }
        else{
            msgbx.showErrorBox("Введите верный пароль");
        }
    }
    else{
        msgbx.showErrorBox("Такого пользователя не существует");
    }

}


void Autho::on_regButton_clicked()
{
    regUi->show();
    this->close();
}


