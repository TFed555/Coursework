#ifndef CREATEWORK_H
#define CREATEWORK_H

#include <QSqlQuery>
#include <QComboBox>
#include <QDialog>
#include <QRegExpValidator>
#include "db.h"
#include "usersmodel.h"
#include "custombox.h"

namespace Ui {
class CreateWork;
}

class CreateWork : public QDialog
{
    Q_OBJECT

public:
    explicit CreateWork(QWidget *parent = nullptr);
    ~CreateWork();

private slots:
    bool on_confirmButton_clicked();
    void on_backButton_clicked();

signals:
    void MainWindow();
private:
    void setUI();
    void setUsers(QComboBox* box);
    bool validateFields();
    QVariantList insertData();
private:
    Ui::CreateWork *ui;
    DataBase *db;
    UsersModel *usersmodel;
    QIntValidator payValidator;
    QRegExpValidator textValidator;
    DataBase conn;
    QSqlQuery* query = new QSqlQuery(conn.db);
    CustomBox msgbx;
};

#endif // CREATEWORK_H
