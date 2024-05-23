#ifndef EDITWORK_H
#define EDITWORK_H

#include <QDialog>
#include <QComboBox>
#include "custombox.h"
#include "usersmodel.h"
#include "db.h"
#include "abstractwork.h"

namespace Ui {
class EditWork;
}

class EditWork : public AbstractWork
{
    Q_OBJECT

public:
    explicit EditWork(int workID, QWidget *parent = nullptr);
    ~EditWork();

    void initialize();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    bool insertResponsible1();

    bool insertResponsible2();
signals:
    void updatedWorkStatus(int id, int status);
private:
    bool setupData(int workID) override;
    bool confirmChange(int workID) override;
    void cancelChange(int workID, int status) override;
    void setTextBrowser(QList<QString> data) override;
    void setStatus(int ID);
    void setUsers(QComboBox* box, QComboBox* compareBox, int resp_id);
    void setComboBox(int resp);
    void setComboBox_2(int resp_2);
    bool updateResponsibles();
    void updateUser(QComboBox* box, QComboBox* compareBox);

private:
    Ui::EditWork *ui;
    DataBase *db;
    CustomBox msgbx;
    UsersModel *usersmodel;
};

#endif // EDITWORK_H
