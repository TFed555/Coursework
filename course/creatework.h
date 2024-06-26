#ifndef CREATEWORK_H
#define CREATEWORK_H

#include <QSqlQuery>
#include <QComboBox>
#include <QDialog>
#include <QRegExpValidator>
#include "db.h"
#include "usersmodel.h"
#include "custombox.h"
#include <memory>

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
    void setFilterBoxes();
    void setUsers(QComboBox* box, int unitID);
    bool validateFields();
    QVariantList insertData();

private:
    Ui::CreateWork *ui;
    std::shared_ptr <DataBase> db;
    UsersModel *usersmodel;
    QIntValidator payValidator;
    QRegExpValidator textValidator;
    CustomBox msgbx;
    const QString teacher = "Преподаватель";
    const QString employee = "Учебно-вспомогательный отдел";
};

#endif // CREATEWORK_H
