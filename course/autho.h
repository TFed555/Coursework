#ifndef AUTHO_H
#define AUTHO_H

#include <QDebug>
#include <QRegExpValidator>
#include <QMessageBox>
#include <QMainWindow>
#include "custombox.h"
#include "registration.h"
#include "db.h"

#include "adminwindow.h"
#include "organiserwindow.h"
#include "workerwindow.h"
#include <memory>


QT_BEGIN_NAMESPACE
namespace Ui { class Autho; }
QT_END_NAMESPACE

class Autho : public QMainWindow
{
    Q_OBJECT

public:
    Autho(QWidget *parent = nullptr);
    ~Autho();

private slots:
    void on_authButton_clicked();

    void on_regButton_clicked();

private:
    void setIcons();

private:
    Ui::Autho *ui;
    std::shared_ptr <Registration> regUi;
    std::shared_ptr <DataBase> db;
    QRegExpValidator phoneValidator;
    std::shared_ptr <AdminWindow> adminUi;
    std::shared_ptr <WorkerWindow> workerUi;
    std::shared_ptr <OrganiserWindow> orgUi;
};
#endif // AUTHO_H
