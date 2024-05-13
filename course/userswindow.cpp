#include "userswindow.h"
#include "ui_userswindow.h"

UsersWindow::UsersWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UsersWindow)
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

UsersWindow::~UsersWindow()
{
    delete ui;
}

void UsersWindow::setupModel(const QStringList &headers)
{
    DataBase conn;
   model = new QSqlQueryModel(this);
   QSqlQuery* query = new QSqlQuery(conn.db);
    query->prepare("select ID, Roles.Name, Surname, Users.Name, Patronymic, phoneNumber, Password, Post "
                   " From Users join Roles on Roles.Role_ID = Users.Role" );
    if(!query->exec()){
        qDebug()<<"";
        return;
    }
    model->setQuery(*query);


    for(int i = 0, j = 0; i < model->columnCount(); i++, j++){
        model->setHeaderData(i,Qt::Horizontal, headers[j]);
    }

}

void UsersWindow::createUI()
{
    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);


}

void UsersWindow::updateModel(){
    DataBase conn;
   QSqlQuery* query = new QSqlQuery(conn.db);
    query->prepare("select ID, Roles.Name, Surname, Users.Name, Patronymic, phoneNumber, Password, Post"
                   " From Users join Roles on Roles.Role_ID = Users.Role;" );
    if(!query->exec()){
        qDebug()<<"Error updating model";
        return;
    }
    model->setQuery(*query);
}

