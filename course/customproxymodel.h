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
    {}

public:
        void setWorkIDs(const QSet<int> &ids){
            workIDs = ids;
            invalidateFilter();
        }

        void setFilterEnabled(bool enabled) {
               filterEnabled = enabled;
               invalidateFilter();
           }

private:
    QSet<int> workIDs;
    bool filterEnabled = false;

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override
    {
        QDate leftData = sourceModel()->data(left.siblingAtColumn(3)).toDate();
        QDate rightData = sourceModel()->data(right.siblingAtColumn(3)).toDate();
        if (leftData != rightData){
            return leftData<rightData;
        }
        return QSortFilterProxyModel::lessThan(left, right);
    }
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override
    {
        if (!filterEnabled) return true;
        QModelIndex index = sourceModel()->index(source_row, 0, source_parent);
        int id = sourceModel()->data(index).toInt();
        return workIDs.contains(id);
    }
};



#endif // CUSTOMPROXYMODEL_H
