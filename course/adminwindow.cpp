#include "adminwindow.h"
#include "ui_adminwindow.h"

AdminWindow::AdminWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminWindow),
    adminUi(std::make_shared<UsersWindow>())
{
    ui->setupUi(this);
    connect(ui->exitButton, SIGNAL(clicked()), qApp, SLOT(closeAllWindows()));
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

void AdminWindow::on_nextButton_clicked()
{
    connect(adminUi.get(), &UsersWindow::AdminWindow, this, &AdminWindow::show);
    this->close();
    adminUi->show();
}


void AdminWindow::on_backButton_clicked()
{
    emit AuthoWindow();
    this->close();
}

void AdminWindow::update_model(){
    adminUi->updateModel();
}

