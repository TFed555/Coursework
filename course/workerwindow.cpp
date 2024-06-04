#include "workerwindow.h"
#include "ui_workerwindow.h"

WorkerWindow::WorkerWindow(QString currentLogin, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WorkerWindow),
    login(currentLogin)
{
    ui->setupUi(this);
    connect(ui->exitButton, SIGNAL(clicked()), qApp, SLOT(closeAllWindows()));
}

WorkerWindow::~WorkerWindow()
{
    delete ui;
}

void WorkerWindow::on_nextButton_clicked()
{
    if (!worksUi){
        worksUi = std::make_shared<WorksWindow>(login);
        connect(worksUi.get(), &WorksWindow::WorkerWindow, this, &WorkerWindow::show);
    }
    this->close();
    worksUi->show();
}


void WorkerWindow::on_backButton_clicked()
{
    emit AuthoWindow();
    this->close();
}

