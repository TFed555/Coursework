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
}


void UsersWindow::createUI()
{
    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setColumnHidden(6, true);
    ui->tableView->setColumnHidden(5, true);
    ui->tableView->setColumnHidden(8, true);
    ui->tableView->setColumnHidden(9, true);
    ui->tableView->setColumnHidden(10, true);
    ui->tableView->setColumnHidden(11, true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

}


void UsersWindow::showUser(const QModelIndex &index){
    int userID = index.model()->data(index.model()->index(index.row(),0)).toInt();
    editUi = std::make_shared <EditUser>(userID);
    connect(editUi.get(), &EditUser::UsersWindow, this, &UsersWindow::show);
    connect(editUi.get(), &EditUser::updatedRole, this, [this](int id, int role){
        model->updateUserRole(id, role);
    });
    this->close();
    editUi->show();
}



void UsersWindow::removeUser(){
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    if (!selection.isEmpty()){
        int reply = msgbx.showWarningBoxWithCancel("Вы уверены что хотите удалить этих сотрудников?");
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
    emit AdminWindow();
}

void UsersWindow::updateModel(){
    model->updateModel();
}

