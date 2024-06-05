#include "autho.h"
#include "ui_autho.h"

Autho::Autho(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Autho),
      regUi(std::make_shared<Registration>()),
      phoneValidator(QRegExp("^\\+7\\d{10}$")),
      adminUi(std::make_shared<AdminWindow>()),
      workerUi(nullptr),
      orgUi(std::make_shared <OrganiserWindow>())
{
    ui->setupUi(this);
    this->setIcons();
    //сигналы между окнами
    connect(regUi.get(), &Registration::AuthoWindow, this, &Autho::show);
    connect(regUi.get(), &Registration::UpdateData, adminUi.get(), &AdminWindow::update_model);
    connect(adminUi.get(), &AdminWindow::AuthoWindow, this, &Autho::show);
    connect(orgUi.get(), &OrganiserWindow::updateUsersAutho, adminUi.get(), &AdminWindow::update_model);
    connect(orgUi.get(), &OrganiserWindow::AuthoWindow, this, &Autho::show);
    ui->loginEdit->setValidator(&phoneValidator);

}

Autho::~Autho()
{
    delete ui;
}

void Autho::setIcons(){
    QPixmap keyIcon(":/iconki/icons/door.png");
    ui->keylabel->setPixmap(keyIcon);
    QPixmap userIcon(":/iconki/icons/user.png");
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
              if (!workerUi) {
                    workerUi = std::make_shared<WorkerWindow>(login);
                    connect(workerUi.get(), &WorkerWindow::AuthoWindow, this, &Autho::show);
              }
              workerUi->show();
              break;
          case 2:
              orgUi->show();
              break;
          case 3:
               adminUi->show();
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


