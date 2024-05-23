#ifndef EDITUSER_H
#define EDITUSER_H

#include <QDialog>
#include <QSqlQuery>
#include <QDebug>

#include "db.h"

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
    void updatedPost(int id, QString post);

private slots:
    void on_backButton_clicked();
    void on_confirmButton_clicked();

private:
    void setupFields(int userID);
    void updateUser(int userID);
    void fillCheckbox(int userID);
    void confirmStatus(int userID);
private:
    Ui::EditUser *ui;
    DataBase *db;
    DataBase conn;
    QSqlQuery* query = new QSqlQuery(conn.db);
};

#endif // EDITUSER_H
