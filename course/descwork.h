#ifndef DESCWORK_H
#define DESCWORK_H

#include <QDialog>
#include <QSqlQuery>
#include <QDebug>

#include "db.h"

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
    void WorksWindow();
private slots:
    void on_buttonBox_accepted();

private:
    void setupData(int workID);
private:
    Ui::DescWork *ui;
    DataBase *db;
};

#endif // DESCWORK_H
