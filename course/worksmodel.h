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
//    WorksModel(QObject* parent = 0);
//    ~WorksModel();
//    static WorksModel* instance_ptr;
    static WorksModel* instance(){
        static WorksModel instance;
        return &instance;
    }

   WorksModel(const WorksModel&) = delete;
   WorksModel& operator = (const WorksModel&) = delete;

public:
    void updateWorkStatus(int id, int status);
    void updateAddWork();
    void removeWorks(const QModelIndexList &indexes);
private:
    void setupModel();
    int rowCount( const QModelIndex& parent ) const override;
    int columnCount( const QModelIndex& parent ) const override;
    QVariant data( const QModelIndex& index, int role ) const override;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const override;
    void appendWork( const int& id, const QString& title,
                          const QString& status );

private:
    WorksModel(QObject *parent = nullptr);
    enum Column {
        ID = 0,
        TITLE,
//        DESC,
//        DEADLINE,
//        PAY,
        STATUS,
//        RESPONSIBLE,
        LAST
    };

    typedef QHash <Column, QVariant> ListData;
    typedef QList <ListData> Works;
    Works works;
private:
     DataBase *db;
};

#endif // WORKSMODEL_H
