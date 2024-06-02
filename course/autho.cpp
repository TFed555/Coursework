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
    this->setIcons();
    connect(regUi, &Registration::AuthoWindow, this, &Autho::show);
    connect(regUi, &Registration::UpdateData, usersUi, &UsersWindow::updateModel);
    connect(usersUi, &UsersWindow::AuthoWindow, this, &Autho::show);
    connect(editUi, &EditWorksWindow::updateUsers, usersUi, &UsersWindow::updateModel);
    connect(editUi, &EditWorksWindow::AuthoWindow, this, &Autho::show);
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

void Autho::setIcons(){
    QPixmap keyIcon(":/iconki/Downloads/free-icon-door-key-435466.png");
    ui->keylabel->setPixmap(keyIcon);
    QPixmap userIcon(":/iconki/Downloads/free-icon-user-profile-10336815.png");
    ui->userlabel->setPixmap(userIcon);
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
          ui->loginEdit->clear();
          ui->pswdEdit->clear();
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
    ui->loginEdit->clear();
    ui->pswdEdit->clear();
    this->close();
}


