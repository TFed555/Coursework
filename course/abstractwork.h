#ifndef ABSTRACTWORK_H
#define ABSTRACTWORK_H

#include <QString>
#include <QDialog>
#include "db.h"
#include "custombox.h"

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
    virtual void rejectAction();
protected:
    int workID;
    CustomBox msgbx;
    DataBase *db;
    QList<QList<QVariant>> tasks;
};

#endif // ABSTRACTWORK_H
