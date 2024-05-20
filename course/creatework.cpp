#include "creatework.h"
#include "ui_creatework.h"

CreateWork::CreateWork(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateWork),
    payValidator(0, 2147483647, this),
    textValidator(QRegExp("^[А-я]{20}"))
{
    ui->setupUi(this);

    this->setComboBox();
    //в другой метод
    ui->titleEdit->setValidator(&textValidator);
    ui->dateEdit->setMinimumDate(QDate::currentDate());
    ui->dateEdit->setDisplayFormat("yyyy.MM.dd");
    ui->payEdit->setValidator(&payValidator);
    ui->descEdit->setValidator(&textValidator);

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
    data.append(1);
    int ind = ui->comboBox->currentIndex();
    if (db->insertIntoWorksTable(data)){
        if (ind != -1){
            int user = ui->comboBox->itemData(ind).toInt();
            db->insertIntoTasksTable(user, db->getLastWorkID());
        }
        this->close();
        emit MainWindow();
    }
}

void CreateWork::setComboBox(){
    ui->comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);

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
        ui->comboBox->addItem(user, QVariant(id));
    }
}


void CreateWork::on_backButton_clicked()
{
    this->close();
    emit MainWindow();
}

