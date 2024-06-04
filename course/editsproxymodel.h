#ifndef EDITSPROXYMODEL_H
#define EDITSPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "worksmodel.h"

enum Mode {
    SearchFilter,
    StatusFilter
};

class EditsSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT


public:
    explicit EditsSortFilterProxyModel(QObject *parent = nullptr)
        : QSortFilterProxyModel(parent)
    {}

public:
        void setStatusFilter(const QString& status){
            this->status = status;
            invalidateFilter();
        }

        void setFilterMode(Mode mode) {
             filterMode = mode;
             invalidateFilter();
           }

        void setSearchString(const QString& searchString) {
               this->searchString = searchString;
               invalidateFilter();
          }
private:
    Mode filterMode = SearchFilter;
    QString searchString;
    QString status;

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override
    {
        if (filterMode == SearchFilter) {
                   QModelIndex index = sourceModel()->index(source_row, 1, source_parent);
                   QString value = sourceModel()->data(index).toString();
                   return value.contains(searchString, Qt::CaseInsensitive);
                }
        else if (filterMode == StatusFilter) {
            QModelIndex index = sourceModel()->index(source_row, 2, source_parent);
            QString value = sourceModel()->data(index).toString();
            return value.contains(status);
        }
        return true;
    }
};




#endif // EDITSPROXYMODEL_H
