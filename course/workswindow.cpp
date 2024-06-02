#include "workswindow.h"
#include "ui_workswindow.h"

WorksWindow::WorksWindow(QString currentLogin, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WorksWindow),
    mymodel(WorksModel::instance()),
    proxyModel(new CustomSortFilterProxyModel(this)),
    login(currentLogin)
{
    ui->setupUi(this);

    connect(ui->tableView, &QTableView::doubleClicked, this, &WorksWindow::showWork);
    this->createUI();
}

WorksWindow::~WorksWindow()
{
    delete ui;
 //   delete mymodel;
}


void WorksWindow::createUI()
{
    int userID = db->checkUserID(login);
    QSet<int> workIDs = db->selectWorksForUser(userID);
    proxyModel->setWorkIDs(workIDs);
    proxyModel->setFilterKeyColumn(0);
    proxyModel->setSourceModel(mymodel);
    ui->tableView->setModel(proxyModel);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, [this](const QString& text){
        proxyModel->setSearchString(text);
        proxyModel->setFilterMode(StringSearch);
    });
    connect(ui->radioButton, &QRadioButton::clicked, this, [this](){
        if (ui->radioButton->isChecked()){
            proxyModel->setFilterMode(IDFilter);
        }
        else proxyModel->setFilterMode(StringSearch);
    });
    QPixmap userIcon(":/iconki/icons/search_8788216.png");
    ui->searchLabel->setPixmap(userIcon);
}

void WorksWindow::showWork(const QModelIndex &index){
    QModelIndex sourceIndex = proxyModel->mapToSource(index);
    int workID = sourceIndex.model()->data(sourceIndex.model()->index(sourceIndex.row(),0)).toInt();
    qDebug() << "Showing work with ID:" << workID;
    itemUi = new DescWork(login, workID);
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

