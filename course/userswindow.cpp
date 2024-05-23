#include "userswindow.h"
#include "ui_userswindow.h"

UsersWindow::UsersWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UsersWindow),
    model(UsersModel::instance())
{
    ui->setupUi(this);
    connect(ui->tableView, &QTableView::doubleClicked, this, &UsersWindow::showUser);
    connect(ui->delButton, &QPushButton::clicked, this, &UsersWindow::removeUser);

    this->createUI();
}

UsersWindow::~UsersWindow()
{
    delete ui;
//    delete editUi;
//    db->closeDataBase();
//    delete db;
//    delete query;
//    delete regUi;
//    delete model;
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


void UsersWindow::showUser(const QModelIndex &index){
    int userID = index.model()->data(index.model()->index(index.row(),0)).toInt();
    EditUser *editUi = new EditUser(userID, this);
    connect(editUi, &EditUser::UsersWindow, this, &UsersWindow::show);
    connect(editUi, &EditUser::updatedRole, this, [this](int id, int role){
        model->updateUserRole(id, role);
    });
    connect(editUi, &EditUser::updatedPost, this, [this](int id, QString post){
        model->updateUserPost(id, post);
    });
    this->close();
    editUi->show();
}



void UsersWindow::removeUser(){
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    if (!selection.isEmpty()){
        int reply = msgbx.showWarningBox("Вы уверены что хотите удалить этих сотрудников?");
        if (reply==QMessageBox::Ok){
            std::sort(selection.rbegin(), selection.rend());
            model->removeUsers(selection);
        }
    }
    else{
        msgbx.showWarningBox("Выберите хотя бы 1 сотрудника");
    }

}

void UsersWindow::on_backButton_clicked()
{
    this->close();
    emit AuthoWindow();
}

void UsersWindow::updateModel(){
    model->updateModel();
}

