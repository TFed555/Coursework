#include "edituser.h"
#include "ui_edituser.h"

EditUser::EditUser(int userID, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditUser),
    model(UsersModel::instance()),
    userId(userID)
{
    ui->setupUi(this);
    this->setupFields();
    connect(ui->confirmButton, &QPushButton::clicked, this, [this](){
        updateUser();
        confirmStatus();
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

QList<QString> EditUser::getData(){
    QList<QString> data;
    QString surname, name, patronymic, phone, unit, degree, rank, post;
     QList<QList<QVariant>> users = model->getList();
       for (auto& user: users){
           if (user[0].toInt() == userId){
               surname = user[2].toString();
               name = user[3].toString();
               patronymic = user[4].toString();
               phone = user[5].toString();
               unit = user[7].toString();
               degree = user[9].toString();
               rank = user[10].toString();
               post = user[11].toString();
               break;
           }
       }
    data = {surname, name, patronymic, phone, unit, degree, rank, post};
    return data;
}


void EditUser::setupFields()
{
       QList<QString> data = getData();
       QString degree = data[5], rank = data[6], post = data[7];
       qDebug()<< degree<<" "<< rank<<" "<< post;
       QList<QLabel*> labels;
       labels << ui->surname << ui->name << ui->patronymic << ui->phoneNumber << ui->post;
       for (int i = 0; i < labels.count(); ++i){
           if (i < data.count()) {
               labels[i]->setText(data[i]);
           }
           else {
               break;
           }
       }
       fillCheckbox();
   connect(ui->comboBox, &QComboBox::currentTextChanged, this, [this, degree, rank, post](){
           this->change_field(degree, rank, post);
     });
}

void EditUser::clearLayout(QLayout *layout){
    while (QLayoutItem* item = layout->takeAt(0)) {
            if (QWidget* widget = item->widget()) {
                widget->deleteLater();
            }
            delete item;
        }
}

void EditUser::addWidgetsToLayout(QLayout *layout, const QList<QWidget *> &widgets){
    for (QWidget* widget : widgets) {
            layout->addWidget(widget);
        }
}

void EditUser::change_field(QString degree, QString rank, QString post){
    clearLayout(ui->verticalLayout);
    QLineEdit* firstEdit = nullptr;
    QLineEdit* secondEdit = nullptr;
    QString dbField;
    QString unit = ui->comboBox->currentText();
    QList<QWidget*> widgets;
    if (unit == teacher){
        QLabel* degreeLabel = dynamicWidget.createLabel("Ученая степень");
        QLineEdit* degreeEdit = dynamicWidget.createEdit(degree);
        QLabel* rankLabel = dynamicWidget.createLabel("Ученое звание");
        QLineEdit* rankEdit = dynamicWidget.createEdit(rank);

        widgets << degreeLabel << degreeEdit << rankLabel << rankEdit;

        dbField = "Degree";
        firstEdit = degreeEdit;
        secondEdit = rankEdit;
    }
    else {
        QLabel* postLabel = dynamicWidget.createLabel("Должность");
        QLineEdit* postEdit = dynamicWidget.createEdit(post);
        widgets << postLabel << postEdit;
        dbField = "Post";
        firstEdit = postEdit;
    }
    QPushButton* confirmButton = dynamicWidget.createButton("Подтвердить");
    widgets << confirmButton;

    addWidgetsToLayout(ui->verticalLayout, widgets);

    ui->verticalLayout->addWidget(confirmButton);
    connect(confirmButton, &QPushButton::clicked, this, [this, firstEdit, secondEdit, dbField](){
        int reply = msgbx.showWarningBoxWithCancel("Вы хотите внести изменения?");
        if (reply == QMessageBox::Ok){
            db->updateUserPost(userId, dbField, firstEdit->text());
            if (dbField == "Degree" && !secondEdit->text().isEmpty()){
                db->updateUserPost(userId, "Rank", secondEdit->text());
            }
            model->updateUserPost(userId, firstEdit->text());
        }
    });
}

int EditUser::getRole(){
    int role = 0;
    QList<QList<QVariant>> users = model->getList();
    for (int i = 0; i < users.count(); i++){
        if (users[i][0].toInt() == userId){
            role = users[i][8].toInt();
            break;
        }
     }
   return role;
}

void EditUser::updateUser(){
    int role = 0;
    if (ui->checkBox->isChecked()){
            role = getRole();
            if (role == 1) role = 2;
            else if (role == 2) role = 1;
            if(db->updateUserRole(userId, role)){
                model->updateUserRole(userId, role);
                fillCheckbox();
            }
       }
}


void EditUser::fillCheckbox(){
    int role = getRole();
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

void EditUser::confirmStatus(){
    if (ui->checkBox_2->isChecked()){
        QString unit = ui->comboBox->currentText();
        db->updateUserUnit(userId, unit);
        ui->post->setText(unit);
        model->updateModel();
//        model->updateUserUnit(userID, unit, salary);
        //emit updatedUnit(userID, unit);
    }
}
