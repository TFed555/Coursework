#include "workswindow.h"
#include "ui_workswindow.h"

WorksWindow::WorksWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WorksWindow)
{
    ui->setupUi(this);

    connect(ui->tableView, &QTableView::clicked, this, &WorksWindow::showWork);
    mymodel = new WorksModel();
    this->createUI();
}

WorksWindow::~WorksWindow()
{
    delete ui;
//    db->closeDataBase();
//    delete db;
//    delete model;
}


void WorksWindow::createUI()
{
    ui->tableView->setModel(mymodel);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

}

void WorksWindow::showWork(const QModelIndex &index){
    int workID = index.model()->data(index.model()->index(index.row(),0)).toInt();
    itemUi = new DescWork(workID, this);
    connect(itemUi, &DescWork::accepted, [this](){
        this->show();
    });
    connect(itemUi, &DescWork::rejected, this, &WorksWindow::show);
    connect(itemUi, &DescWork::updatedWorkStatus, [this](int id, int status){
        mymodel->updateWorkStatus(id, status);
    });
    this->close();
    itemUi->show();
}

void WorksWindow::on_backButton_clicked()
{
    this->close();
    emit AuthoWindow();
}

