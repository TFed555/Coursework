#ifndef USERSWINDOW_H
#define USERSWINDOW_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QDebug>

#include "db.h"
#include "registration.h"
#include "edituser.h"

namespace Ui {
class UsersWindow;
}

class UsersWindow : public QDialog
{
    Q_OBJECT

public:
    explicit UsersWindow(QWidget *parent = nullptr);
    ~UsersWindow();

public:
    void updateModel();
private slots:
    void showUser(const QModelIndex &index);

private:
    void setupModel(const QStringList &headers);
    void createUI();
private:
    Ui::UsersWindow *ui;
    DataBase *db;
    QSqlQueryModel *model;
    Registration *regUi;
    EditUser *editUi;
};

#endif // USERSWINDOW_H
