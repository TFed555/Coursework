#include "autho.h"
#include "ui_autho.h"

Autho::Autho(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Autho),
      regUi(new Registration()),
      phoneValidator(QRegExp("^\\+7\\d{10}$")),
      usersUi(new UsersWindow()),
      worksUi(nullptr),
      editUi(new EditWorksWindow())
{
    ui->setupUi(this);

    connect(regUi, &Registration::AuthoWindow, this, &Autho::show);
    connect(regUi, &Registration::UpdateData, usersUi, &UsersWindow::updateModel);
    connect(usersUi, &UsersWindow::AuthoWindow, this, &Autho::show);
    connect(editUi, &EditWorksWindow::updateUsers, usersUi, &UsersWindow::updateModel);
    connect(editUi, &EditWorksWindow::AuthoWindow, this, &Autho::show);
    ui->loginEdit->setText("+7");
    ui->loginEdit->setValidator(&phoneValidator);

}

Autho::~Autho()
{
    delete ui;
    delete regUi;
    delete usersUi;
    if (worksUi){
        delete worksUi;
    }
    delete editUi;
}


void Autho::on_authButton_clicked()
{
    CustomBox msgbx;
    QString login = ui->loginEdit->text();
    QString password = ui->pswdEdit->text();
    if(db->loginExists(login)){
        if (db->pswdCompare(login, password)){
          switch(db->getRole(login)){
          case 1:
              if (!worksUi) {
                    worksUi = new WorksWindow(login);
                    connect(worksUi, &WorksWindow::AuthoWindow, this, &Autho::show);
              }
              worksUi->show();
              break;
          case 2:
              editUi->show();
              break;
          case 3:
               usersUi->show();
               break;
           default:
              msgbx.showErrorBox("Роль не найдена");
              return;
          }
          this->hide();
        }
        else{
            msgbx.showErrorBox("Введите верный пароль");
        }
    }
    else{
        msgbx.showErrorBox("Такого пользователя не существует");
    }
    ui->loginEdit->clear();
    ui->pswdEdit->clear();
}


void Autho::on_regButton_clicked()
{
    regUi->show();
    ui->loginEdit->clear();
    ui->pswdEdit->clear();
    this->close();
}


