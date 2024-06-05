#include "editworkswindow.h"
#include "ui_editworkswindow.h"

EditWorksWindow::EditWorksWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditWorksWindow),
    mymodel(WorksModel::instance()),
    proxyModel(std::make_shared <EditsSortFilterProxyModel>(this))
{
    ui->setupUi(this);
    connect(ui->tableView, &QTableView::doubleClicked, this, &EditWorksWindow::showWork);
    this->createUI();
}

EditWorksWindow::~EditWorksWindow()
{
    delete ui;
}



void EditWorksWindow::createUI()
{
    proxyModel->setFilterKeyColumn(0);
    proxyModel->setSourceModel(mymodel);
    ui->tableView->setModel(proxyModel.get());
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    //сигналы для корректного удаления/завершения задач
    connect(mymodel, &QAbstractItemModel::dataChanged, proxyModel.get(), &QSortFilterProxyModel::invalidate);
    connect(mymodel, &QAbstractItemModel::layoutChanged, proxyModel.get(), &QSortFilterProxyModel::invalidate);
    connect(mymodel, &QAbstractItemModel::modelReset, proxyModel.get(), &QSortFilterProxyModel::invalidate);
    setSearchFields();
}

//строка поиска и фильтр по статусам
void EditWorksWindow::setSearchFields(){
    QPixmap userIcon(":/iconki/icons/search.png");
    ui->searchLabel->setPixmap(userIcon);
    QList<QList<QVariant>> status = db->selectAllStatus();
    ui->comboBox->addItem("", 4);
    for (int i = 0; i < status.count(); i++){
        ui->comboBox->addItem(status[i][1].toString(), status[i][0]);
    }
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, [this](const QString& text){
        proxyModel->setSearchString(text);
        proxyModel->setFilterMode(SearchFilter);
    });
    connect(ui->comboBox, &QComboBox::currentTextChanged, this, [this](){
        if (ui->comboBox->currentIndex()!=4){
            proxyModel->setStatusFilter(ui->comboBox->currentText());
            proxyModel->setFilterMode(StatusFilter);
        }
        else proxyModel->setFilterMode(SearchFilter);
    });
}

//открытие окна просмотра работы
void EditWorksWindow::showWork(const QModelIndex &index){
    QModelIndex sourceIndex = proxyModel->mapToSource(index);
    int workID = sourceIndex.model()->data(sourceIndex.model()->index(sourceIndex.row(),0)).toInt();
    editWork = std::make_shared <EditWork>(workID);
    this->close();
    editWork->initialize();
    editWork->open();
    connect(editWork.get(), &EditWork::accepted, this, &EditWorksWindow::show);
    connect(editWork.get(), &EditWork::rejected, this, &EditWorksWindow::show);
    connect(editWork.get(), &EditWork::updatedWorkStatus, this, [this] (int id, int status){
        mymodel->updateWorkStatus(id, status);
    } );
}

//возврат к окну организатора
void EditWorksWindow::on_backButton_clicked()
{
    this->close();
    emit OrganiserWindow();
}

//открытие окна создания задачи
void EditWorksWindow::on_addButton_clicked()
{
    newWork = std::make_shared <CreateWork>();
    this->close();
    newWork->show();
    connect(newWork.get(), &CreateWork::MainWindow, this, [this](){
        mymodel->updateAddWork();
        this->show(); }
    );
}

//удаление работ
void EditWorksWindow::on_delButton_clicked()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    QList<int> sourceRows;
    for (const QModelIndex &proxyIndex : selection) {
        QModelIndex sourceIndex = proxyModel->mapToSource(proxyIndex);
        sourceRows.append(sourceIndex.row());
    }

    if (!selection.isEmpty()){
        int reply = msgbx.showWarningBoxWithCancel("Вы уверены что хотите удалить эти задания?");
        if (reply==QMessageBox::Ok){
            mymodel->removeWorks(sourceRows);
        }
    }
    else{
        msgbx.showWarningBox("Выберите хотя бы 1 задание");
    }
}

//завершение работ
void EditWorksWindow::on_finishButton_clicked()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    QList<int> sourceRows;
    for (const QModelIndex &proxyIndex : selection) {
        QModelIndex sourceIndex = proxyModel->mapToSource(proxyIndex);
        sourceRows.append(sourceIndex.row());
    }
    if (!selection.isEmpty()){
        int reply = msgbx.showWarningBoxWithCancel("Вы уверены что хотите завершить эти задания?");
        if (reply==QMessageBox::Ok){
            mymodel->finishWorks(sourceRows);
            emit updateUsers();
        }
    }
    else{
        msgbx.showWarningBox("Выберите хотя бы 1 задание");
    }
}
