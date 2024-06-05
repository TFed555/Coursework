#include "creatework.h"
#include "ui_creatework.h"

CreateWork::CreateWork(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateWork),
    usersmodel(UsersModel::instance()),
    payValidator(0, 2147483647, this)
{
    ui->setupUi(this);
    this->setUI();
}

CreateWork::~CreateWork()
{
    delete ui;
}

void CreateWork::setUI(){
    this->setUsers(ui->comboBox, 0);
    this->setUsers(ui->comboBox_2, 0);
    setFilterBoxes();
    ui->comboBox_2->setEnabled(false);
    connect(ui->comboBox, &QComboBox::currentTextChanged, this, [this](){
            ui->comboBox_2->setEnabled(true);
    });
    ui->dateEdit->setMinimumDate(QDate::currentDate());
    ui->dateEdit->setDisplayFormat("yyyy.MM.dd");
    ui->payEdit->setValidator(&payValidator);
}

//установка сигналов между чекбоксами
void CreateWork::setFilterBoxes(){
    connect(ui->teacherBox, &QCheckBox::stateChanged, this, [this](){
        if (ui->teacherBox->isChecked()){
            ui->employeeBox->setDisabled(true);
            this->setUsers(ui->comboBox, 1);
            this->setUsers(ui->comboBox_2, 1);
        }
        else{
        ui->employeeBox->setEnabled(true);
        this->setUsers(ui->comboBox, 0);
        this->setUsers(ui->comboBox_2, 0);
        }
    });
    connect(ui->employeeBox, &QCheckBox::stateChanged, this, [this](){
        if (ui->employeeBox->isChecked()){
            ui->teacherBox->setDisabled(true);
            this->setUsers(ui->comboBox, 2);
            this->setUsers(ui->comboBox_2, 2);
        }
        else{
        ui->teacherBox->setEnabled(true);
        this->setUsers(ui->comboBox, 0);
        this->setUsers(ui->comboBox_2, 0);
        }
    });
}

bool CreateWork::on_confirmButton_clicked()
{
    if (insertData().isEmpty()){
        return false;
    }
    QVariantList data = insertData();
    int selectedIndex = ui->comboBox->currentIndex();
    int secondSelectedIndex = ui->comboBox_2->currentIndex();
    if (selectedIndex == secondSelectedIndex && selectedIndex != 0 && secondSelectedIndex != 0){
        msgbx.showErrorBox("Ответственные должны быть разными");
        return false;
    }
    if (db->insertIntoWorksTable(data)){
        if (selectedIndex != 0){
            int user = ui->comboBox->itemData(selectedIndex).toInt();
            db->insertIntoTasksTable(user, db->getLastWorkID());
        }
        if (secondSelectedIndex != 0){
            int user_2 = ui->comboBox_2->itemData(secondSelectedIndex).toInt();
            db->insertIntoTasksTable(user_2, db->getLastWorkID());
        }
        this->close();
        emit MainWindow();
    }
    return true;
}

QVariantList CreateWork::insertData(){

    QVariantList data;
    if (validateFields()){
    data.append(ui->titleEdit->text());
    data.append(ui->dateEdit->text());
    data.append(ui->payEdit->text());
    data.append(ui->descEdit->toPlainText());
    int selectedIndex = ui->comboBox->currentIndex();
    data.append(selectedIndex != 0 ? 2 : 1);
    }
    return data;
}

//Добавление пользователей в комбобоксы
void CreateWork::setUsers(QComboBox* box, int unitID){
    box->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    if (box->count()!=0){
        box->clear();
    }
    box->addItem("");
    QList<QList<QVariant>> users = usersmodel->getList();
    QString currentUnit;
    currentUnit = unitID == 1 ? teacher : employee;
    for(int i = 0; i < users.count(); i++){
        QString unit = users[i][7].toString();
        int id = users[i][0].toInt();
        int role = users[i][8].toInt();
        QString user = users[i][2].toString()+" "+users[i][3].toString()+" "+users[i][4].toString();
        if (role!=3){
            if (unitID == 0){
                box->addItem(user, QVariant(id));
            }
            else if (currentUnit == unit){
                box->addItem(user, QVariant(id));
            }
        }
    }
}

//валидация
bool CreateWork::validateFields(){
    if (ui->titleEdit->text()==""){
        msgbx.showWarningBox("Введите заголовок");
        return false;
    }
    if (ui->payEdit->text()==0){
        msgbx.showWarningBox("Введите оплату");
        return false;
    }
    return true;
}



void CreateWork::on_backButton_clicked()
{
    this->close();
    emit MainWindow();
}

