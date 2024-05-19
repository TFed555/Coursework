#ifndef WORKSMODEL_H
#define WORKSMODEL_H

#include <QVariant>
#include <QAbstractTableModel>
#include <QSqlQuery>
#include <QDebug>
#include "db.h"

class WorksModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    WorksModel(QObject* parent = 0);
    ~WorksModel();

public:
    void updateWorkStatus(int id, int status);
//    QList<QHash<WorksModel::Column, QVariant>> getList(){
//        return works;
//    }
private:
    void setupModel();
    int rowCount( const QModelIndex& parent ) const;
    int columnCount( const QModelIndex& parent ) const;
    QVariant data( const QModelIndex& index, int role ) const;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    void appendWork(  const QString& id, const QString& title, const QString& desc,
                          const QString& deadline, const QString& pay, const QString& status, const QString& responsible );


private:
    enum Column {
        ID = 0,
        TITLE,
        DESC,
        DEADLINE,
        PAY,
        STATUS,
        RESPONSIBLE,
        LAST
    };

    typedef QHash <Column, QVariant> ListData;
    typedef QList <ListData> Works;
    Works works;
private:
     DataBase *db;
};

#endif // WORKSMODEL_H
