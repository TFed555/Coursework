#ifndef USERSWINDOW_H
#define USERSWINDOW_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QDebug>

#include "custombox.h"
#include "usersmodel.h"
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

    void on_backButton_clicked();
signals:
    void AdminWindow();
private:
    void createUI();
    void removeUser();
private:
    Ui::UsersWindow *ui;
    UsersModel *model;
    std::shared_ptr <EditUser> editUi;
    CustomBox msgbx;
};

#endif // USERSWINDOW_H
