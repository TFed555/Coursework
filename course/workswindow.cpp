#include "workswindow.h"
#include "ui_workswindow.h"

WorksWindow::WorksWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WorksWindow)
{
    ui->setupUi(this);
    db = new DataBase();
    db->connectToDataBase();
    connect(ui->tableView, &QTableView::clicked, this, &WorksWindow::showWork);
    this->setupModel(
                     QStringList() << trUtf8("ID")
                                   << trUtf8("Title")
                                   << trUtf8("Description")
                                   << trUtf8("Deadline")
                                   << trUtf8("Pay")
                                   << trUtf8("Status")
                                   << trUtf8("Responsible")
                     );
    this->createUI();
}

WorksWindow::~WorksWindow()
{
    delete ui;
//    db->closeDataBase();
//    delete db;
//    delete model;
}

void WorksWindow::setupModel(const QStringList &headers)
{
   DataBase conn;
   model = new QSqlQueryModel(this);
   QSqlQuery* query = new QSqlQuery(conn.db);
    query->prepare("Select Works.ID, Title, Description, Deadline, Pay, Status.Name, Users.Name "
                   " From Works JOIN Status ON Status.ID = Works.Status "
                   " JOIN Users ON Users.ID = Works.Responsible" );
    if(!query->exec()){
        qDebug()<<"";
        return;
    }
    model->setQuery(*query);

    for(int i = 0, j = 0; i < model->columnCount()-1; i++, j++){
        model->setHeaderData(i,Qt::Horizontal, headers[j]);
    }


}

void WorksWindow::createUI()
{
    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

}

void WorksWindow::showWork(const QModelIndex &index){
    int workID = index.model()->data(index.model()->index(index.row(),0)).toInt();
    DescWork *itemUi = new DescWork(workID, this);
    connect(itemUi, &DescWork::WorksWindow, [this](){
        //this->updateModel();
        this->show();
    });
    this->close();
    itemUi->show();
}
