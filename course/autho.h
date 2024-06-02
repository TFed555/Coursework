#ifndef AUTHO_H
#define AUTHO_H

#include <QDebug>
#include <QRegExpValidator>
#include <QMessageBox>
#include <QMainWindow>
#include "custombox.h"
#include "registration.h"
#include "db.h"
#include "userswindow.h"
#include "workswindow.h"
#include "editworkswindow.h"


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
    Registration *regUi;
    DataBase *db;
    QSqlQuery *query;
    QRegExpValidator phoneValidator;
    UsersWindow *usersUi;
    WorksWindow *worksUi;
    EditWorksWindow *editUi;
};
#endif // AUTHO_H
