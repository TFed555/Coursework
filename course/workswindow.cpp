#include "workswindow.h"
#include "ui_workswindow.h"

WorksWindow::WorksWindow(QString currentLogin, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WorksWindow),
    mymodel(WorksModel::instance()),
    proxyModel(new QSortFilterProxyModel(this)),
    login(currentLogin)
{
    ui->setupUi(this);
    proxyModel->setSourceModel(mymodel);

    connect(ui->tableView, &QTableView::clicked, this, &WorksWindow::showWork);
    this->createUI();
}

WorksWindow::~WorksWindow()
{
    delete ui;
 //   delete mymodel;
}


void WorksWindow::createUI()
{
    ui->tableView->setModel(proxyModel);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

}

void WorksWindow::showWork(const QModelIndex &index){
    //QModelIndex sourceIndex = proxyModel->mapToSource(index);
    int workID = index.model()->data(index.model()->index(index.row(),0)).toInt();
    itemUi = new DescWork(login, workID, this);
    connect(itemUi, &DescWork::accepted, this, &WorksWindow::show);
    connect(itemUi, &DescWork::rejected, this, &WorksWindow::show);
    connect(itemUi, &DescWork::updatedWorkStatus, this, [this](int id, int status){
        mymodel->updateWorkStatus(id, status);
    });
    this->close();
    itemUi->initialize();
    itemUi->show();
}

void WorksWindow::on_backButton_clicked()
{
    this->close();
    emit AuthoWindow();
}

