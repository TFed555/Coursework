#ifndef EDITUSER_H
#define EDITUSER_H

#include <QDialog>
#include <QSqlQuery>
#include <QDebug>

#include "db.h"
#include "custombox.h"
#include "usersmodel.h"
#include "qdynamicedit.h"

namespace Ui {
class EditUser;
}

class EditUser : public QDialog
{
    Q_OBJECT

public:
    explicit EditUser(int userID, QWidget *parent = nullptr);
    ~EditUser();

signals:
    void UsersWindow();
    void updatedRole(int id, int role);
    void updatedUnit(int id, QString post);

private slots:
    void on_backButton_clicked();
    void on_confirmButton_clicked();
    void add_field(QString degree, QString rank, QString post);

private:
    void setupFields(int userID);
    void updateUser(int userID);
    void fillCheckbox(int userID);
    void confirmStatus(int userID);
private:
    Ui::EditUser *ui;
    UsersModel *model;
    DataBase *db;
    CustomBox msgbx;
private:
    const int userId;
};

#endif // EDITUSER_H
