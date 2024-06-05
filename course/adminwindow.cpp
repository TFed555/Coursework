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

//открытие окна просмотра сотрудников
void AdminWindow::on_nextButton_clicked()
{
    connect(adminUi.get(), &UsersWindow::AdminWindow, this, &AdminWindow::show);
    this->close();
    adminUi->show();
}

//возврат к авторизации
void AdminWindow::on_backButton_clicked()
{
    emit AuthoWindow();
    this->close();
}

//обновление модели сотрудников
void AdminWindow::update_model(){
    adminUi->updateModel();
}

