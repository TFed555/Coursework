#include "creatework.h"
#include "ui_creatework.h"

CreateWork::CreateWork(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateWork),
    payValidator(0, 2147483647, this)
{
    ui->setupUi(this);

    this->setComboBox();
    connect(ui->comboBox, &QComboBox::currentTextChanged, this, &CreateWork::setComboBox_2);
    //в другой метод
    ui->dateEdit->setMinimumDate(QDate::currentDate());
    ui->dateEdit->setDisplayFormat("yyyy.MM.dd");
    ui->payEdit->setValidator(&payValidator);
    ui->comboBox_2->setEnabled(false);

}

CreateWork::~CreateWork()
{
    delete ui;
}

void CreateWork::on_confirmButton_clicked()
{
    QVariantList data;
    data.append(ui->titleEdit->text());
    data.append(ui->dateEdit->text());
    data.append(ui->payEdit->text());
    data.append(ui->descEdit->text());
    int ind = ui->comboBox->currentIndex();
    int ind_2 = ui->comboBox_2->currentIndex();
    data.append(ind != 0 ? 2 : 1);
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

void CreateWork::setUsers(QComboBox* box){
    DataBase conn;
    QSqlQuery* query = new QSqlQuery(conn.db);
    query->prepare("Select Name, Surname, Patronymic, ID "
                       "From Users " );
    if(!query->exec()){
            qDebug()<<query->lastError().text();
            return;
    }
    while (query->next()){
        int id = query->value(3).toInt();
        QString user = query->value(0).toString() + " " + query->value(1).toString() + " "
                + query->value(2).toString();
        box->addItem(user, QVariant(id));
    }
}

void CreateWork::setComboBox(){
    ui->comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    setUsers(ui->comboBox);
}

void CreateWork::setComboBox_2(){
    ui->comboBox_2->setEnabled(true);
    setUsers(ui->comboBox_2);
}

void CreateWork::on_backButton_clicked()
{
    this->close();
    emit MainWindow();
}

