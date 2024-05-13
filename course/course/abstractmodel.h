#ifndef ABSTRACTMODEL_H
#define ABSTRACTMODEL_H

#include <QSqlQueryModel>
#include <QDebug>

class AbstractModel {
public:
    virtual void setupModel(const QStringList &headers) = 0;
    virtual void createUI() = 0;
};

#endif // ABSTRACTMODEL_H
