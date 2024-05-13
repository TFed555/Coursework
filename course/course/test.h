#ifndef TEST_H
#define TEST_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QDebug>

#include "db.h"
#include "registration.h"

namespace Ui {
class Test;
}

class Test : public QDialog
{
    Q_OBJECT

public:
    explicit Test(QWidget *parent = nullptr);
    ~Test();


public:
    void updateModel();
private:
    Ui::Test *ui;
    DataBase *db;
    QSqlQueryModel *model;
    Registration *regUi;
private:
    void setupModel(const QStringList &headers);
    void createUI();
private slots:

};

#endif // TEST_H
