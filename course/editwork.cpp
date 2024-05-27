#include "editwork.h"
#include "ui_editwork.h"

EditWork::EditWork(int workID, QWidget *parent) :
    AbstractWork(workID, parent),
    ui(new Ui::EditWork),
    usersmodel(UsersModel::instance())
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &EditWork::rejectAction);
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
    //формирование textbrowser
    QString title = "<b>Название</b> " + QString("%1").arg(tasks[0][0].toString());
    QString desc = "<b>Описание</b> " + QString("%1").arg(tasks[0][1].toString());
    QString deadline = "<b>Срок</b> " + QString("%1").arg(tasks[0][2].toString());
    QString pay = "<b>Оплата</b> " + QString("%1").arg(tasks[0][3].toString());
    int statusID = tasks[0][4].toInt();
    QList <QString> data = {title, desc, deadline, pay};
    if (!tasks[0][8].isNull()){
        resp = tasks[0][8].toInt();
     }
    if (tasks.length()>1){
        resp_2 = tasks[1][8].toInt();
    }
    setTextBrowser(data);
    setStatus(statusID);
    setUsers(ui->comboBox, ui->comboBox_2, resp);
    setUsers(ui->comboBox_2, ui->comboBox, resp_2);
    connect(ui->comboBox, &QComboBox::currentTextChanged, this, [this, resp_2]{
        this->setUsers(ui->comboBox_2, ui->comboBox, resp_2);
    });
    return true;
}

void EditWork::setTextBrowser(QList<QString> data){
    ui->textBrowser->setFont(QFont("Times", 9));
    for (const QString& item : data){
        ui->textBrowser->append(item);
    }
}

void EditWork::setStatus(int ID){
    QList<QList<QVariant>> status = db->selectAllStatus();
    for (int i = 0; i < status.count(); i++){
        ui->statusBox->addItem(status[i][1].toString(), status[i][0]);
    }
    //установка текущего статуса
    ui->statusBox->setCurrentIndex(ID-1);
}

bool EditWork::confirmChange(int workID){
    bool updated = updateResponsibles();
    int statusID = ui->statusBox->currentIndex()+1;
    if (statusID == 1 && updated){
        statusID = 2;
    }
    if (statusID == 2 && (ui->comboBox->currentIndex() == -1 && ui->comboBox_2->currentIndex() == -1)){
        int reply = msgbx.showErrorBox("Вы не можете назначить этот статус. Выберите ответственных.");
        if (reply == QMessageBox::Ok){
            return false;
        }
    }
    else {
        if (db->updateWorkStatus(workID, statusID)){
            emit updatedWorkStatus(workID, statusID);
        }
        accept();
    }
    return true;
}

void EditWork::cancelChange(int workID, int status){
    Q_UNUSED(workID);
    Q_UNUSED(status);
}

bool EditWork::insertResponsible(QComboBox* box){
    int ind = box->currentIndex();
    if (ind!=-1){
        int user = box->itemData(ind).toInt();
        db->insertIntoTasksTable(user, workID);
        return true;
    }
    return false;
}



bool EditWork::updateResponsibles(){
    int status = db->getStatus(workID);
    if (status == 1 && ui->comboBox->currentIndex()!=-1 && ui->comboBox_2->currentIndex()!=-1){
        insertResponsible(ui->comboBox);
        insertResponsible(ui->comboBox_2);
        return true;
    }
    else if (status == 1 && ui->comboBox_2->currentIndex()==-1){
        insertResponsible(ui->comboBox);
        return true;
    }
    else if (status == 2){
        if (db->getCountTasks(workID) == 1){
            insertResponsible(ui->comboBox_2);
        }
        if (ui->comboBox_2->currentIndex() == -1 && db->getCountTasks(workID)!=1){
            deleteResponsible(ui->comboBox_2);
        }
    }
    int ind = ui->comboBox->currentIndex();
    int ind_2 = ui->comboBox_2->currentIndex();
    if (ind!=-1){
        int user = ui->comboBox->itemData(ind).toInt();
        int taskId = db->getTaskID(workID, "ASC");
        db->updateTaskResponsibles(workID, user, taskId);
    }
    if(ind_2!=-1){
        int user_2 = ui->comboBox_2->itemData(ind_2).toInt();
        int taskId = db->getTaskID(workID, "DESC");
        db->updateTaskResponsibles(workID, user_2, taskId);
    }
    return true;
}


bool EditWork::deleteResponsible(QComboBox* box){
    int index = box->currentIndex();
    if (index == -1){
        int taskID = db->getTaskID(workID, "DESC");
        db->deleteResponsible(taskID);
        return true;
    }
    return false;
}


void EditWork::setUsers(QComboBox* box, QComboBox* compareBox, int respID){
    if (box->count()!=0){
        box->clear();
    }
    QList<QList<QVariant>> users = usersmodel->getList();
    for(int i = 0; i < users.count(); i++){
        int id = users[i][0].toInt();
        QString role = users[i][1].toString();
        QString user = users[i][2].toString()+" "+users[i][3].toString()+" "+users[i][4].toString();
        if(compareBox->currentIndex() != compareBox->findData(id)){
            if (role!="заведующий" && role!="организатор"){
                box->addItem(user, QVariant(id));
            }
        }
    }
    int index = box->findData(respID, Qt::UserRole);
    box->setCurrentIndex(index);
}



