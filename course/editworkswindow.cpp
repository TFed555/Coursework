#include "editworkswindow.h"
#include "ui_editworkswindow.h"

EditWorksWindow::EditWorksWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditWorksWindow),
    mymodel(WorksModel::instance()),
    proxyModel(new QSortFilterProxyModel(this))
{
    ui->setupUi(this);
    connect(ui->tableView, &QTableView::doubleClicked, this, &EditWorksWindow::showWork);
    this->createUI();
}

EditWorksWindow::~EditWorksWindow()
{
    delete ui;
    //delete mymodel;
}



void EditWorksWindow::createUI()
{
    ui->tableView->setModel(mymodel);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
}

void EditWorksWindow::showWork(const QModelIndex &index){
    int workID = index.model()->data(index.model()->index(index.row(),0)).toInt();
    editWork = new EditWork(workID);
    this->close();
    editWork->initialize();
    editWork->open();
    connect(editWork, &EditWork::accepted, this, &EditWorksWindow::show);
    connect(editWork, &EditWork::rejected, this, &EditWorksWindow::show);
    connect(editWork, &EditWork::updatedWorkStatus, this, [this] (int id, int status){
        mymodel->updateWorkStatus(id, status);
    } );
}

void EditWorksWindow::on_backButton_clicked()
{
    this->close();
    emit AuthoWindow();
}


void EditWorksWindow::on_addButton_clicked()
{
    newWork = new CreateWork();
    this->close();
    newWork->show();
    connect(newWork, &CreateWork::MainWindow, this, [this](){
        mymodel->updateAddWork();
        this->show(); }
    );
}


void EditWorksWindow::on_delButton_clicked()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    if (!selection.isEmpty()){
        int reply = msgbx.showWarningBoxWithCancel("Вы уверены что хотите удалить эти задания?");
        if (reply==QMessageBox::Ok){
            std::sort(selection.rbegin(), selection.rend());
            mymodel->removeWorks(selection);
        }
    }
    else{
        msgbx.showWarningBox("Выберите хотя бы 1 задание");
    }
}



void EditWorksWindow::on_finishButton_clicked()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    if (!selection.isEmpty()){
        int reply = msgbx.showWarningBoxWithCancel("Вы уверены что хотите завершить эти задания?");
        if (reply==QMessageBox::Ok){
            std::sort(selection.rbegin(), selection.rend());
            mymodel->finishWorks(selection);
            emit updateUsers();
        }
    }
    else{
        msgbx.showWarningBox("Выберите хотя бы 1 задание");
    }
}

