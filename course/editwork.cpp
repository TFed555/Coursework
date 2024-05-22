#include "editwork.h"
#include "ui_editwork.h"

EditWork::EditWork(int workID, QWidget *parent) :
    AbstractWork(workID, parent),
    ui(new Ui::EditWork)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [this, workID]{
        this->confirmChange(workID);
    });
    connect(ui->comboBox, &QComboBox::currentTextChanged, this, [this](){
        this->setUsers(ui->comboBox_2, ui->comboBox);
    });
    connect(ui->comboBox_2, &QComboBox::currentTextChanged, this, [this](){
        this->setUsers(ui->comboBox, ui->comboBox_2);
    });
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
    setComboBox(resp);
    setComboBox_2(resp_2);
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

void EditWork::confirmChange(int workID){
    int statusID = ui->statusBox->currentIndex();
    qDebug()<<ui->comboBox->currentIndex();
    if (statusID == 1 && (ui->comboBox->currentIndex() == -1 && ui->comboBox_2->currentIndex() == -1)){
        int reply = msgbx.showErrorBox("Вы не можете назначить этот статус. Выберите ответственных.");
        if (reply == QMessageBox::Ok){
            return;
        }
    }
    else{
        if (db->updateWorkStatus(workID, statusID+1)){
            emit updatedWorkStatus(workID, statusID+1);
        }
        updateResponsibles();
        accept();
    }
}

void EditWork::cancelChange(int workID, int status){
    Q_UNUSED(workID);
    Q_UNUSED(status);
}

bool EditWork::insertResponsible1(){
    int ind = ui->comboBox->currentIndex();
    qDebug()<<ind;
    if (ind!=-1 && ind !=0){
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

void EditWork::updateResponsibles(){
    int status = db->getStatus(workID);
    if (status == 1){
        insertResponsible1();
        insertResponsible2();
    }
    else if (status == 2){
        query->prepare("Select Count(ID) From Tasks where Work = :workID");
        query->bindValue(":workID", workID);
        query->exec();
        query->next();
        int tt = query->value(0).toInt();
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

void EditWork::setUsers(QComboBox* box, QComboBox* compareBox){
    query->prepare("Select Surname, Name, Patronymic, ID "
                       "From Users " );
    if(!query->exec()){
            qDebug()<<query->lastError().text();
            return;
    }
    while (query->next()){
        int id = query->value(3).toInt();
        QString user = query->value(0).toString() + " " + query->value(1).toString() + " "
                + query->value(2).toString();
        if (compareBox->currentIndex() == compareBox->findData(id, Qt::UserRole)){
            continue;
        }
        box->addItem(user, QVariant(id));
        qDebug()<< ui->comboBox->currentIndex() << user << " "<< id;
    }
}

void EditWork::setComboBox(int resp){
    ui->comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    setUsers(ui->comboBox, ui->comboBox_2);
    int index = ui->comboBox->findData(resp, Qt::UserRole);
    ui->comboBox->setCurrentIndex(index);

}

void EditWork::setComboBox_2(int resp_2){
    ui->comboBox_2->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    setUsers(ui->comboBox_2, ui->comboBox);
    int index = ui->comboBox_2->findData(resp_2, Qt::UserRole);
    ui->comboBox_2->setCurrentIndex(index);
}


