#include "editworkswindow.h"
#include "ui_editworkswindow.h"

EditWorksWindow::EditWorksWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditWorksWindow)
{
    ui->setupUi(this);
    model = new WorksModel();
    this->createUI();
}

EditWorksWindow::~EditWorksWindow()
{
    delete ui;
//    db->closeDataBase();
//    delete db;
//    delete model;
}



void EditWorksWindow::createUI()
{
    ui->tableView->setModel(model);
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
        model->updateTable();
        this->show(); }
    );
}


void EditWorksWindow::on_delButton_clicked()
{
//    int rowNumber;
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
     std::sort(selection.rbegin(), selection.rend());
    if (selection.isEmpty()){
       qDebug()<<"Строка не выбрана";
    }
    else{
        model->removeWorks(selection);
    }
//    foreach(QModelIndex index, selection) {
//        QSqlRecord record = model->record(index.row());

//        rowNumber = record.value("id").toInt();
//    }
}

