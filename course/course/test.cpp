#include "test.h"
#include "ui_test.h"

Test::Test(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test)
{
    ui->setupUi(this);
    db = new DataBase();
    db->connectToDataBase();
    this->setupModel(
                     QStringList() << trUtf8("ID")
                                   << trUtf8("Role")
                                   << trUtf8("Surname")
                                   << trUtf8("Name")
                                   << trUtf8("Patronymic")
                                   << trUtf8("phoneNumber")
                                   << trUtf8("Password")
                                   << trUtf8("Post")
                     );
    this->createUI();

}

Test::~Test()
{
    delete ui;
}

void Test::setupModel(const QStringList &headers)
{
    DataBase conn;
 // conn.connectToDataBase();
   model = new QSqlQueryModel(this);
   QSqlQuery* query = new QSqlQuery(conn.db);
    query->prepare("select ID, Roles.Name, Surname, Users.Name, Patronymic, phoneNumber, Password, Post "
                   " From Users join Roles on Roles.Role_ID = Users.Role" );
    if(!query->exec()){
        qDebug()<<"";
        return;
    }
    model->setQuery(*query);
   // model->setTable(tableName);

    for(int i = 0, j = 0; i < model->columnCount(); i++, j++){
        model->setHeaderData(i,Qt::Horizontal, headers[j]);
    }

  //  model->setSort(0,Qt::AscendingOrder);
}

void Test::createUI()
{
    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    //model->select();
}

void Test::updateModel(){
    DataBase conn;
   QSqlQuery* query = new QSqlQuery(conn.db);
    query->prepare("select ID, Roles.Name, Surname, Users.Name, Patronymic, phoneNumber, Password, Post"
                   " From Users join Roles on Roles.Role_ID = Users.Role;" );
    if(!query->exec()){
        qDebug()<<"";
        return;
    }
    model->setQuery(*query);
}
