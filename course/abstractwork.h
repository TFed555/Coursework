#ifndef ABSTRACTWORK_H
#define ABSTRACTWORK_H

#include <QString>
#include <QDialog>
#include "db.h"

class AbstractWork : public QDialog
{
    Q_OBJECT

public:
    explicit AbstractWork(int workID, QWidget *parent = nullptr);
    virtual ~AbstractWork() = 0;

    virtual bool setupData(int workID) = 0;
    virtual bool confirmChange(int workID) = 0;
    virtual void cancelChange(int workID, int status) = 0;
    virtual void setTextBrowser(QList<QString> data) = 0;
protected:
    int workID;
    DataBase *db;
    DataBase conn;
    QSqlQuery* query = new QSqlQuery(conn.db);
};

#endif // ABSTRACTWORK_H
