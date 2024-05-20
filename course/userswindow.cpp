#include "userswindow.h"
#include "ui_userswindow.h"

UsersWindow::UsersWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UsersWindow)
{
    ui->setupUi(this);
    connect(ui->tableView, &QTableView::doubleClicked, this, &UsersWindow::showUser);
    connect(ui->delButton, &QPushButton::clicked, this, &UsersWindow::removeUser);

    this->setupModel(
                     QStringList() << trUtf8("ID")
                                   << trUtf8("Роль")
                                   << trUtf8("Фамилия")
                                   << trUtf8("Имя")
                                   << trUtf8("Отчество")
                                   << trUtf8("Телефон")
                                   << trUtf8("Должность")
                     );
    this->createUI();
}

UsersWindow::~UsersWindow()
{
    delete ui;
//    delete editUi;
//    db->closeDataBase();
//    delete db;
//    delete regUi;
//    delete model;
}

void UsersWindow::setupModel(const QStringList &headers)
{
   //DataBase conn;
   model = new QSqlQueryModel(this);
   //QSqlQuery* query = new QSqlQuery(conn.db);
//    query->prepare("select ID, Roles.Name, Surname, Users.Name, Patronymic, phoneNumber, Post "
//                   " From Users join Roles on Roles.Role_ID = Users.Role" );
//    if(!query->exec()){
//        qDebug()<<"err";
//        return;
//    }
//    model->setQuery(*query);
   model->setQuery("select ID, Roles.Name, Surname, Users.Name, Patronymic, phoneNumber, Post "
                                      " From Users join Roles on Roles.Role_ID = Users.Role");


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
//    connect(ui->tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(UsersWindow::showUser()));

}

void UsersWindow::updateModel(){
   DataBase conn;
   QSqlQuery* query = new QSqlQuery(conn.db);
    query->prepare("Select ID, Roles.Name, Surname, Users.Name, Patronymic, phoneNumber, Post"
                   " From Users join Roles on Roles.Role_ID = Users.Role;" );
    if(!query->exec()){
        qDebug()<<"Error updating model";
        return;
    }
    model->setQuery(*query);
}

void UsersWindow::showUser(const QModelIndex &index){
    int userID = index.model()->data(index.model()->index(index.row(),0)).toInt();
    EditUser *editUi = new EditUser(userID, this);
    connect(editUi, &EditUser::UsersWindow, this, [this](){
        this->updateModel();
        this->show();
    });
    this->close();
    editUi->show();
}



void UsersWindow::removeUser(){
    int rowNumber;
    QModelIndexList selection = ui->tableView->selectionModel()->selection().indexes();
    if (selection.isEmpty()){
       qDebug()<<"строка не выбрана";
    }
    foreach(QModelIndex index, selection) {
        QSqlRecord record = model->record(index.row());
        rowNumber = record.value("id").toInt();
    }

    DataBase conn;
    QSqlQuery* query = new QSqlQuery(conn.db);
    query->prepare("Delete from Users "
                   "Where Users.ID = :userID");
    query->bindValue(":userID", rowNumber);
    if (!query->exec()){
        qDebug()<<query->lastError().text();
    }
    else {
        qDebug()<<endl<<"deleted";
        this->updateModel();
    }
}

void UsersWindow::on_backButton_clicked()
{
    this->close();
    emit AuthoWindow();
}

