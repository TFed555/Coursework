#ifndef EDITWORK_H
#define EDITWORK_H

#include <QDialog>
#include <QComboBox>
#include "custombox.h"
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
    void confirmChange(int workID) override;
    void cancelChange(int workID, int status) override;
    void setTextBrowser(QList<QString> data) override;
    void setStatus(int ID);
    void setUsers(QComboBox* box, QComboBox* compareBox);
    void setComboBox(int resp);
    void setComboBox_2(int resp_2);
    void updateResponsibles();

private:
    Ui::EditWork *ui;
    DataBase *db;
    CustomBox msgbx;
};

#endif // EDITWORK_H
