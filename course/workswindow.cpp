#include "workswindow.h"
#include "ui_workswindow.h"

WorksWindow::WorksWindow(QString currentLogin, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WorksWindow),
    mymodel(WorksModel::instance()),
    login(currentLogin)
{
    ui->setupUi(this);
 //   mymodel = new WorksModel(this);

    connect(ui->tableView, &QTableView::clicked, this, &WorksWindow::showWork);
    this->createUI();
}

WorksWindow::~WorksWindow()
{
    delete ui;
//    db->closeDataBase();
//    delete db;
 //   delete mymodel;
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
    itemUi = new DescWork(login, workID, this);
    connect(itemUi, &DescWork::accepted, this, [this](){
        this->show();
    });
    connect(itemUi, &DescWork::rejected, this, &WorksWindow::show);
    connect(itemUi, &DescWork::updatedWorkStatus, this, [this](int id, int status){
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

