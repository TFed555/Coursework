#include "edituser.h"
#include "ui_edituser.h"

EditUser::EditUser(int userID, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditUser),
    model(UsersModel::instance())
{
    ui->setupUi(this);
    this->setupFields(userID);
    connect(ui->confirmButton, &QPushButton::clicked, this, [this, userID](){
        updateUser(userID);
        confirmStatus(userID);
        ui->checkBox->setChecked(false);
        ui->checkBox_2->setChecked(false);
    });

}

EditUser::~EditUser()
{
    delete ui;
    delete db;
    delete query;
}

void EditUser::on_backButton_clicked()
{
    this->close();
    emit UsersWindow();
}

void EditUser::setupFields(int userID)
{
    QString surname, name, patronymic, phone, post;
       QList<QList<QVariant>> users = model->getList();
       for (int i = 0; i < users.count(); i++){
           if (users[i][0].toInt() == userID){
               surname = users[i][2].toString();
               name = users[i][3].toString();
               patronymic = users[i][4].toString();
               phone = users[i][5].toString();
               post = users[i][7].toString();
               break;
           }
       }
       ui->surname->setText(surname);
       ui->name->setText(name);
       ui->patronymic->setText(patronymic);
       ui->phoneNumber->setText(phone);
       ui->post->setText(post);
       fillCheckbox(userID);

}

void EditUser::updateUser(int userID){
    int role = 0;
    QList<QList<QVariant>> users = model->getList();
    if (ui->checkBox->isChecked()){
        for (int i = 0; i < users.count(); i++){
            if (users[i][0].toInt() == userID){
                role = users[i][6].toInt();
                break;
            }
         }
            if (role == 1) role = 2;
            else if (role == 2) role = 1;
            if(db->updateUserRole(userID, role)){
                emit updatedRole(userID, role);
                fillCheckbox(userID);
            }
       }
}

void EditUser::on_confirmButton_clicked()
{
   // this->close();
}

void EditUser::fillCheckbox(int userID){
    int role = 0;
    QList<QList<QVariant>> users = model->getList();
    for (int i = 0; i < users.count(); i++){
                if (users[i][0].toInt() == userID){
                    role = users[i][6].toInt();
                    break;
                }
     }
    if (role==3) {
        ui->checkBox->setText("Назначить организатором");
        ui->checkBox->setEnabled(false);
    }
    if (role==2){
        ui->checkBox->setText("Назначить сотрудником");
    }
    else {
        ui->checkBox->setText("Назначить организатором");
    }
}

void EditUser::confirmStatus(int userID){
    if (ui->checkBox_2->isChecked()){
        QString post = ui->comboBox->currentText();
        db->updateUserPost(userID, post);
        ui->post->setText(post);
        emit updatedPost(userID, post);
    }
}
