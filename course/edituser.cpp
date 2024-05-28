#include "edituser.h"
#include "ui_edituser.h"

EditUser::EditUser(int userID, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditUser),
    model(UsersModel::instance()),
    userId(userID)
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
}

void EditUser::on_backButton_clicked()
{
    this->close();
    emit UsersWindow();
}

//вынести метод getData
void EditUser::setupFields(int userID)
{
    QString surname, name, patronymic, phone, unit, degree, rank, post;
    //QVector вместо QList ???
       QList<QList<QVariant>> users = model->getList();
       for (int i = 0; i < users.count(); i++){
           if (users[i][0].toInt() == userID){
               surname = users[i][2].toString();
               name = users[i][3].toString();
               patronymic = users[i][4].toString();
               phone = users[i][5].toString();
               unit = users[i][7].toString();
               //убрать проверку isNull() или добавить везде
               degree = users[i][8].isNull() ? "": users[i][8].toString();
               rank = users[i][9].isNull() ? "": users[i][9].toString();
               post = users[i][10].isNull() ? "": users[i][10].toString();
               break;
           }
       }
       ui->surname->setText(surname);
       ui->name->setText(name);
       ui->patronymic->setText(patronymic);
       ui->phoneNumber->setText(phone);
       ui->post->setText(unit);
       fillCheckbox(userID);
       connect(ui->comboBox, &QComboBox::currentTextChanged, this, [this, degree, rank, post](){
           this->add_field(degree, rank, post);
       });
}


//создать create_field
void EditUser::add_field(QString degree, QString rank, QString post){
    while (QLayoutItem *item = ui->verticalLayout->takeAt(0)) {
            if (QWidget *widget = item->widget()) {
                widget->deleteLater();
            }
            delete item;
        }
    QString dbField;
    QString unit = ui->comboBox->currentText();
    QLabel *label = new QLabel(this);
    QLabel *label_2 = new QLabel(this);
    QDynamicEdit *edit = new QDynamicEdit(this);
    QDynamicEdit *edit_2 = new QDynamicEdit(this);
    QPushButton *button = new QPushButton(this);
    if (unit == "Преподаватель"){
        label->setText("Ученая степень");
        edit->setText(degree);
        label_2->setText("Ученое звание");
        edit_2->setText(rank);
        dbField = "Degree";
        ui->verticalLayout->addWidget(label_2);
        ui->verticalLayout->addWidget(edit_2);
    }
    else {
        label->setText("Должность");
        edit->setText(post);
        dbField = "Post";
        delete label_2;
        delete edit_2;
    }
    button->setText("Подтвердить");
    ui->verticalLayout->addWidget(label);
    ui->verticalLayout->addWidget(edit);
    ui->verticalLayout->addWidget(button);
    connect(button, &QPushButton::clicked, this, [this, edit, edit_2, dbField](){
        int reply = msgbx.showWarningBoxWithCancel("Вы хотите внести изменения?");
        if (reply == QMessageBox::Ok){
            db->updateUserPost(userId, dbField, edit->text());
            if (dbField == "Degree"){
                if (edit_2->text()!="") db->updateUserPost(userId, "Rank", edit->text());
            }
            model->updateUserPost(userId, edit->text());
        }
    });
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
        QString unit = ui->comboBox->currentText();
        db->updateUserUnit(userID, unit);
        ui->post->setText(unit);
        emit updatedUnit(userID, unit);
    }
}
