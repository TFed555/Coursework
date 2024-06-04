#include "organiserwindow.h"
#include "ui_organiserwindow.h"

OrganiserWindow::OrganiserWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OrganiserWindow),
    editUi(std::make_shared<EditWorksWindow>())
{
    ui->setupUi(this);
    connect(ui->exitButton, SIGNAL(clicked()), qApp, SLOT(closeAllWindows()));
}

OrganiserWindow::~OrganiserWindow()
{
    delete ui;
}

void OrganiserWindow::on_nextButton_clicked()
{
    connect(editUi.get(), &EditWorksWindow::OrganiserWindow, this, &OrganiserWindow::show);
    connect(editUi.get(), &EditWorksWindow::updateUsers, this, &OrganiserWindow::update_users);
    this->close();
    editUi->show();
}

void OrganiserWindow::update_users(){
    emit updateUsersAutho();
}


void OrganiserWindow::on_backButton_clicked()
{
    this->close();
    emit AuthoWindow();
}


