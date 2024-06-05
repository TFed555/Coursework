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

class EditWork : public Work
{
    Q_OBJECT

public:
    explicit EditWork(int workID, QWidget *parent = nullptr);
    ~EditWork();

    void initialize();

private slots:

    bool insertResponsible(QComboBox* box);

    bool deleteResponsible(QComboBox* box);
signals:
    void updatedWorkStatus(int id, int status);

private:
    bool setupData(int workID) override;
    bool confirmChange(int workID) override;
    void cancelChange(int workID, int status) override;
    void setStatus(int ID);
    void setUsers(QComboBox* box, QComboBox* compareBox, int resp_id);
    bool updateResponsibles();
    void updateUser(QComboBox* box, QComboBox* compareBox);
    bool noResponsiblesSelected() const;
    bool updateSpecificResponsible(QComboBox* box);

private:
    Ui::EditWork *ui;
    UsersModel *usersmodel;
};

#endif // EDITWORK_H
