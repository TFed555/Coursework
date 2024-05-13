#include "workswindow.h"
#include "ui_workswindow.h"

WorksWindow::WorksWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WorksWindow)
{
    ui->setupUi(this);
    db = new DataBase();
    db->connectToDataBase();
    this->setupModel(
                     QStringList() << trUtf8("ID")
                                   << trUtf8("Title")
                                   << trUtf8("Description")
                                   << trUtf8("Deadline")
                                   << trUtf8("Pay")
                                   << trUtf8("Description")
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
    query->prepare("select * "
                   " From Works" );
    if(!query->exec()){
        qDebug()<<"";
        return;
    }
    model->setQuery(*query);


    for(int i = 0, j = 0; i < model->columnCount(); i++, j++){
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
