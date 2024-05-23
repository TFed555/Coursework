#include "editwork.h"
#include "ui_editwork.h"

EditWork::EditWork(int workID, QWidget *parent) :
    AbstractWork(workID, parent),
    ui(new Ui::EditWork),
    usersmodel(UsersModel::instance())
{
    ui->setupUi(this);
//    connect(ui->comboBox, &QComboBox::currentTextChanged, this, &EditWork::updateResponsibles);
//    connect(ui->comboBox_2, &QComboBox::currentTextChanged, this, &EditWork::updateResponsibles);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [this, workID]{
        this->confirmChange(workID);
    });
    ui->comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    ui->comboBox_2->setSizeAdjustPolicy(QComboBox::AdjustToContents);
}

EditWork::~EditWork()
{
    delete ui;
    delete db;
}

void EditWork::initialize(){
    setupData(workID);
}

bool EditWork::setupData(int workID){
    int resp = 0, resp_2 = 0;
    if(!AbstractWork::setupData(workID)){
        return false;
    }
    query->next();
    QString title = "<b>Название</b> " + QString("%1").arg(query->value(0).toString());
    QString desc = "<b>Описание</b> " + QString("%1").arg(query->value(1).toString());
    QString deadline = "<b>Срок</b> " + QString("%1").arg(query->value(2).toString());
    QString pay = "<b>Оплата</b> " + QString("%1").arg(query->value(3).toString());
    int statusID = query->value(8).toInt();
    QList <QString> data = {title, desc, deadline, pay};
    if (!query->value(9).isNull()){
        resp = query->value(9).toInt();
     }
    if (query->next()){
        resp_2 = query->value(9).toInt();
    }
    setTextBrowser(data);
    setStatus(statusID);
    setUsers(ui->comboBox, ui->comboBox_2, resp);
//    setComboBox(resp);
    setUsers(ui->comboBox_2, ui->comboBox, resp_2);
//    setComboBox_2(resp_2);
//        connect(ui->comboBox, &QComboBox::currentTextChanged, this, [this, resp_2](){
//            this->setUsers(ui->comboBox_2, ui->comboBox, resp_2);
//        });
//        connect(ui->comboBox_2, &QComboBox::currentTextChanged, this, [this, resp](){
//            this->setUsers(ui->comboBox, ui->comboBox_2, resp);
//        });
    return true;
}

void EditWork::setTextBrowser(QList<QString> data){
    ui->textBrowser->setFont(QFont("Times", 9));
    for (const QString& item : data){
        ui->textBrowser->append(item);
    }
}

void EditWork::setStatus(int ID){
    query->exec("Select Status.Name, Status.ID "
                    "From Status");
    while(query->next()){
        int id = query->value(1).toInt();
        ui->statusBox->addItem(query->value(0).toString(), QVariant(id));
    }
    //установка текущего статуса
    ui->statusBox->setCurrentIndex(ID-1);
}

bool EditWork::confirmChange(int workID){
    int statusID = ui->statusBox->currentIndex()+1;
    qDebug()<<ui->comboBox->currentIndex();
    if (statusID == 1 && updateResponsibles()){
        statusID = 2;
    }
    if (statusID == 2 && (ui->comboBox->currentIndex() == -1 && ui->comboBox_2->currentIndex() == -1)){
        int reply = msgbx.showErrorBox("Вы не можете назначить этот статус. Выберите ответственных.");
        if (reply == QMessageBox::Ok){
            qDebug() << "Error Box: OK button pressed.";
            return false;
        }
    }
    else if (statusID != 1 && (ui->comboBox->currentIndex() == ui->comboBox_2->currentIndex())){
        msgbx.showErrorBox("Вы не можете назначить одинаковых ответственных.");
//        if (reply == QMessageBox::Ok){
//            qDebug() << "Error Box: OK button pressed.";
//            return false;
//        }
    }
    else {
        updateResponsibles();
        if (db->updateWorkStatus(workID, statusID)){
            emit updatedWorkStatus(workID, statusID);
        }
        //updateResponsibles();
        accept();
    }
    return true;
}

void EditWork::cancelChange(int workID, int status){
    Q_UNUSED(workID);
    Q_UNUSED(status);
}

bool EditWork::insertResponsible1(){
    int ind = ui->comboBox->currentIndex();
    qDebug()<<ind;
    if (ind!=-1){
        int user = ui->comboBox->itemData(ind).toInt();
        qDebug()<<" "<<user;
        db->insertIntoTasksTable(user, workID);
        return true;
    }
    return false;
}

bool EditWork::insertResponsible2(){
    int ind_2 = ui->comboBox_2->currentIndex();
    if(ind_2 != -1 && ind_2 != 0){
        int user_2 = ui->comboBox_2->itemData(ind_2).toInt();
        db->insertIntoTasksTable(user_2, workID);
        return true;
    }
    return false;
}

bool EditWork::updateResponsibles(){
    int status = db->getStatus(workID);
    if (status == 1){
        insertResponsible1();
        insertResponsible2();
        return true;
    }
    else if (status == 2){
        query->prepare("Select Count(ID) From Tasks where Work = :workID");
        query->bindValue(":workID", workID);
        query->exec();
        query->next();
        if (query->value(0).toInt()==1){
            insertResponsible2();
        }
    }
    int ind = ui->comboBox->currentIndex();
    qDebug()<<ind;
    int ind_2 = ui->comboBox_2->currentIndex();
    if (ind!=-1 && ind !=0){
        int user = ui->comboBox->itemData(ind).toInt();
        qDebug()<<" "<<user;
        int taskId = db->getTaskID(workID, "ASC");
        db->updateTaskResponsibles(workID, user, taskId);
    }
    if(ind_2!=-1 && ind != 0){
        int user_2 = ui->comboBox_2->itemData(ind_2).toInt();
        int taskId = db->getTaskID(workID, "DESC");
        db->updateTaskResponsibles(workID, user_2, taskId);
    }
    return true;
}

void EditWork::on_buttonBox_accepted()
{
//    accept();
}


void EditWork::on_buttonBox_rejected()
{
    int reply = msgbx.showWarningBox("Изменения не были сохранены, продолжить?");
    if (reply==QMessageBox::Ok){
        reject();
    }
}

void EditWork::setUsers(QComboBox* box, QComboBox* compareBox, int respID){
    QList<QList<QVariant>> users = usersmodel->getList();
    for(int i = 0; i < users.count(); i++){
        int id = users[i][0].toInt();
        QString role = users[i][1].toString();
        QString user = users[i][2].toString()+" "+users[i][3].toString()+" "+users[i][4].toString();
        if (role!="заведующий" && role!="организатор"){
            box->addItem(user, QVariant(id));
        }
        qDebug()<< ui->comboBox->currentIndex() << user << " "<< id;
    }
    int index = box->findData(respID, Qt::UserRole);
    box->setCurrentIndex(index);
}

void EditWork::updateUser(QComboBox *box, QComboBox *compareBox){
    qDebug()<<box->currentData();
    qDebug()<<compareBox->currentData();
}

void EditWork::setComboBox(int resp){
    ui->comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    int index = ui->comboBox->findData(resp, Qt::UserRole);
    ui->comboBox->setCurrentIndex(index);

}

void EditWork::setComboBox_2(int resp_2){
    ui->comboBox_2->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    int index = ui->comboBox_2->findData(resp_2, Qt::UserRole);
    ui->comboBox_2->setCurrentIndex(index);
}


