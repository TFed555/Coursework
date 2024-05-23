#include "autho.h"
#include "ui_autho.h"

Autho::Autho(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Autho),
      phoneValidator(QRegExp("^\\+\\d{1}\\d{10}$"))
{
    ui->setupUi(this);

    usersUi = new UsersWindow();
    editUi = new EditWorksWindow();
    regUi = new Registration();
    connect(regUi, &Registration::AuthoWindow, this, &Autho::show);
    connect(regUi, &Registration::UpdateData, usersUi, &UsersWindow::updateModel);
    //Временно
    connect(usersUi, &UsersWindow::AuthoWindow, this, &Autho::show);
    connect(editUi, &EditWorksWindow::AuthoWindow, this, &Autho::show);
    ui->loginEdit->setText("+7");
    ui->loginEdit->setValidator(&phoneValidator);

   test = new Test();
}

Autho::~Autho()
{
    delete ui;
//    delete usersUi;
//    delete worksUi;
//    delete editUi;
//    delete regUi;
}


void Autho::on_authButton_clicked()
{
    CustomBox msgbx;
    if(db->loginExists(ui->loginEdit->text())){
        QString currentLogin = ui->loginEdit->text();
        if (db->pswdCompare(ui->loginEdit->text(), ui->pswdEdit->text())){
          switch(db->getRole(ui->loginEdit->text())){
          case 1:
              worksUi = new WorksWindow(currentLogin);
              connect(worksUi, &WorksWindow::AuthoWindow, this, &Autho::show);
              worksUi->show();
              this->hide();
              break;
          case 2:
              editUi->show();
              this->hide();
              break;
          case 3:
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


