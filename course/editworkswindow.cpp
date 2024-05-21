#include "editworkswindow.h"
#include "ui_editworkswindow.h"

EditWorksWindow::EditWorksWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditWorksWindow),
    mymodel(WorksModel::instance())
{
    ui->setupUi(this);
   // mymodel = new WorksModel(this);
    this->createUI();
}

EditWorksWindow::~EditWorksWindow()
{
    delete ui;
//    db->closeDataBase();
//    delete db;
    //delete mymodel;
}



void EditWorksWindow::createUI()
{
    ui->tableView->setModel(mymodel);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
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
        int reply = msgbx.showWarningBox("Вы уверены что хотите завершить эти задания?");
        if (reply==QMessageBox::Ok){
            std::sort(selection.rbegin(), selection.rend());
            mymodel->removeWorks(selection);
        }
    }
    else{
        msgbx.showWarningBox("Выберите хотя бы 1 задание");
    }
}

