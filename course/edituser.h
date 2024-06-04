#ifndef EDITUSER_H
#define EDITUSER_H

#include <QDialog>
#include <QSqlQuery>
#include <QDebug>

#include "db.h"
#include "custombox.h"
#include "usersmodel.h"
#include "qdynamicedit.h"
#include <memory>

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

private slots:
    void on_backButton_clicked();
    void change_field(QString degree, QString rank, QString post);;

private:
    QList<QString> getData();
    void setupFields();
    void updateUser();
    void fillCheckbox();
    void confirmStatus();
    int getRole();

    void clearLayout(QLayout* layout);
    void addWidgetsToLayout(QLayout* layout, const QList<QWidget*>& widgets);

private:
    Ui::EditUser *ui;
    UsersModel *model;
    std::shared_ptr <DataBase> db;
    CustomBox msgbx;
    QDynamicWidget dynamicWidget;
private:
    const int userId;
    const QString teacher = "Преподаватель";
    const QString employee = "Учебно-вспомогательный отдел";
};

#endif // EDITUSER_H
