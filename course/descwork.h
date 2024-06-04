#ifndef DESCWORK_H
#define DESCWORK_H

#include <QMessageBox>
#include <QDialog>
#include <QSqlQuery>
#include <QDebug>

#include "db.h"
#include "abstractwork.h"
#include "worksmodel.h"


namespace Ui {
class DescWork;
}

class DescWork : public Work
{
    Q_OBJECT

public:
    explicit DescWork(QString currentLogin, int workID, QWidget *parent = nullptr);
    ~DescWork();

    void initialize();

signals:
    void updatedWorkStatus(int id, int status);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    bool setupData(int workID) override;
    bool confirmChange(int workID) override;
    void cancelChange(int workID, int status) override;
    bool checkUserID(int workID);
    void rejectAction() override;
private:
    Ui::DescWork *ui;
    DataBase *db;
private:
    const QString login;
};

#endif // DESCWORK_H
