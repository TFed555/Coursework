#ifndef DESCWORK_H
#define DESCWORK_H

#include <QDialog>
#include <QSqlQuery>
#include <QDebug>

#include "db.h"
#include "worksmodel.h"

namespace Ui {
class DescWork;
}

class DescWork : public QDialog
{
    Q_OBJECT

public:
    explicit DescWork(int workID, QWidget *parent = nullptr);
    ~DescWork();

signals:
//    void WorksWindow();
    void updatedWorkStatus(int id, int status);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    void setupData(int workID);
    void confirmChange(int workID);
private:
    Ui::DescWork *ui;
    DataBase *db;
//    WorksModel *model;
};

#endif // DESCWORK_H
