#include "creatework.h"
#include "ui_creatework.h"

CreateWork::CreateWork(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateWork),
    usersmodel(UsersModel::instance()),
    payValidator(0, 2147483647, this)
{
    ui->setupUi(this);
    this->setComboBox();
    this->setComboBox_2();
    ui->comboBox_2->setEnabled(false);
    connect(ui->comboBox, &QComboBox::currentTextChanged, this, [this](){
            ui->comboBox_2->setEnabled(true);
    });
    //в другой метод
    ui->dateEdit->setMinimumDate(QDate::currentDate());
    ui->dateEdit->setDisplayFormat("yyyy.MM.dd");
    ui->payEdit->setValidator(&payValidator);

}

CreateWork::~CreateWork()
{
    delete ui;
    delete db;
    delete query;
}

void CreateWork::on_confirmButton_clicked()
{
    QVariantList data;
    if (validateFields()){
    data.append(ui->titleEdit->text());
    data.append(ui->dateEdit->text());
    data.append(ui->payEdit->text());
    data.append(ui->descEdit->text());
    int ind = ui->comboBox->currentIndex();
    int ind_2 = ui->comboBox_2->currentIndex();
    data.append(ind != 0 ? 2 : 1);
    if (ind == ind_2 && ind != 0 && ind_2 != 0){
        msgbx.showErrorBox("Ответственные должны быть разными");
    }
    if (db->insertIntoWorksTable(data)){
        if (ind != 0){
            int user = ui->comboBox->itemData(ind).toInt();
            db->insertIntoTasksTable(user, db->getLastWorkID());
        }
        if (ind_2 != 0){
            int user_2 = ui->comboBox_2->itemData(ind_2).toInt();
            db->insertIntoTasksTable(user_2, db->getLastWorkID());
        }
        this->close();
        emit MainWindow();
    }
    }
}

void CreateWork::setUsers(QComboBox* box){
    QList<QList<QVariant>> users = usersmodel->getList();
    for(int i = 0; i < users.count(); i++){
        int id = users[i][0].toInt();
        QString role = users[i][1].toString();
        QString user = users[i][2].toString()+" "+users[i][3].toString()+" "+users[i][4].toString();
        if (role!="заведующий" && role!="организатор"){
            box->addItem(user, QVariant(id));
        }
    }
}

bool CreateWork::validateFields(){
    if (ui->titleEdit->text()==""){
        msgbx.showWarningBox("Введите заголовок");
        return false;
    }
    return true;
}

void CreateWork::setComboBox(){
    ui->comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    setUsers(ui->comboBox);
}

void CreateWork::setComboBox_2(){
    ui->comboBox_2->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    setUsers(ui->comboBox_2);
}

void CreateWork::on_backButton_clicked()
{
    this->close();
    emit MainWindow();
}

