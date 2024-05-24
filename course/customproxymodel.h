#ifndef CUSTOMPROXYMODEL_H
#define CUSTOMPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "worksmodel.h"

class CustomSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT


public:
    explicit CustomSortFilterProxyModel(QObject *parent = nullptr)
        : QSortFilterProxyModel(parent)
    {
    }

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override
    {
        QString leftData = sourceModel()->data(left.siblingAtColumn(2)).toString();
        QString rightData = sourceModel()->data(right.siblingAtColumn(2)).toString();
        if (leftData != rightData){
            return leftData<rightData;
        }
        return QSortFilterProxyModel::lessThan(left, right);
    }
};



#endif // CUSTOMPROXYMODEL_H
