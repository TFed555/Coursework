#ifndef CUSTOMPROXYMODEL_H
#define CUSTOMPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "worksmodel.h"

enum FilterMode {
    StringSearch,
    IDFilter
};

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

        void setFilterMode(FilterMode mode) {
             filterMode = mode;
             invalidateFilter();
           }

        void setSearchString(const QString& searchString) {
               this->searchString = searchString;
               invalidateFilter();
          }
private:
    QSet<int> workIDs;
    FilterMode filterMode = StringSearch;
    QString searchString;

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
        if (filterMode == StringSearch) {
                   QModelIndex index = sourceModel()->index(source_row, 1, source_parent);
                   QString value = sourceModel()->data(index).toString();
                   return value.contains(searchString, Qt::CaseInsensitive);
                }
        else if (filterMode == IDFilter) {
            QModelIndex index = sourceModel()->index(source_row, filterKeyColumn(), source_parent);
            int id = sourceModel()->data(index).toInt();
            return workIDs.contains(id);
        }
        return true;
    }
};



#endif // CUSTOMPROXYMODEL_H
