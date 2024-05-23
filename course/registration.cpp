#include "registration.h"
#include "ui_registration.h"
#include "autho.h"

Registration::Registration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Registration),
    phoneValidator(QRegExp("^\\+\\d{1}\\d{10}$")),
    textValidator(QRegExp("^[А-я]{20}"))
{
    ui->setupUi(this);
    connect(this, &Registration::clearValid, this, &Registration::clearFields);
    connect(ui->backButton, &QPushButton::clicked, this, &Registration::clearFields);
    ui->phoneEdit->setValidator(&phoneValidator);
    ui->nameEdit->setValidator(&textValidator);
    ui->surnameEdit->setValidator(&textValidator);
    ui->patronymicEdit->setValidator(&textValidator);

}

Registration::~Registration()
{
    delete ui;
    delete db;
}

//Возврат к окну авторизации
void Registration::on_backButton_clicked()
{
    this->close();
    emit AuthoWindow();
}

void Registration::clearFields(){

    ui->nameEdit->clear();
    ui->surnameEdit->clear();
    ui->patronymicEdit->clear();
    ui->phoneEdit->clear();
    ui->pswdEdit->clear();
}



//Занесение данных пользователя в бд
bool Registration::on_confirmButton_clicked()
{
    CustomBox msgbx;
    if (db->loginExists(ui->phoneEdit->text())){
        msgbx.showErrorBox("Такой телефон уже существует");
        return false;
    }
    else{
        if (validateFields()){
            QVariantList data;
            data.append(1);
            data.append(ui->surnameEdit->text());
            data.append(ui->nameEdit->text());
            data.append(ui->patronymicEdit->text());
            data.append(ui->phoneEdit->text());
            QString password = ui->pswdEdit->text().toLatin1().toHex();
            data.append(password);
            data.append(ui->comboBox->currentText());
            if(db->insertIntoUsersTable(data)){
                this->close();
                emit UpdateData(), emit clearValid(), emit AuthoWindow();
                return true;
            }
            else{
                return false;
            }
        }
    }
    return false;
}

bool Registration::validateFields(){
    CustomBox msgbx;
    int len = ui->phoneEdit->text().length();
    QString nameEdit = ui->nameEdit->text();
    QString surnameEdit = ui->surnameEdit->text();
    QString pswdEdit = ui->pswdEdit->text();
    if (nameEdit.isEmpty()&surnameEdit.isEmpty()&pswdEdit.isEmpty()){
        msgbx.showErrorBox("Поля не могут быть пустыми");
        return false;
    }
    else {
    if (nameEdit.isEmpty()){
        msgbx.showWarningBox("Введите имя");
        return false;
    }
    if (surnameEdit.isEmpty()){
        msgbx.showWarningBox("Введите фамилию");
        return false;
    }
    if (pswdEdit.isEmpty()){
        msgbx.showWarningBox("Введите пароль");
        return false;
    }
    if (len<12){
        msgbx.showWarningBox("Неверный номер");
        return false;
    }
    }
    return true;
}




void Registration::on_pswdEdit_textChanged(const QString &arg1)
{
    validatePassword(ui->pswdEdit->text());
}

//дописать
void Registration::validatePassword(QString pswd){
    if (pswd.length()<6){
        ui->pswdLabel->setText("Пароль должен содержать\n 6 символов");
    }
    else{
        ui->pswdLabel->clear();
    }
    //else if (!pswd.contains(QChar()))
}
